/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <kernel.h>
#include <console/console.h>
#include <string.h>
#include <sys/printk.h>
#include <zephyr/types.h>
#include <irq.h>

#include <mpsl_timeslot.h>
#include <mpsl.h>
#include <hal/nrf_timer.h>

#define TIMESLOT_REQUEST_DISTANCE_US (1000000)
#define TIMESLOT_LENGTH_US           (200)
#define TIMER_EXPIRY_US (TIMESLOT_LENGTH_US - 50)

#define MPSL_THREAD_PRIO             CONFIG_MPSL_THREAD_COOP_PRIO
#define STACKSIZE                    CONFIG_MAIN_STACK_SIZE
#define THREAD_PRIORITY              K_LOWEST_APPLICATION_THREAD_PRIO

static bool request_in_cb = true;

/* MPSL API calls that can be requested for the non-preemptible thread */
enum mpsl_timeslot_call {
	OPEN_SESSION,
	MAKE_REQUEST,
	CLOSE_SESSION,
};

/* Timeslot requests */
static mpsl_timeslot_request_t timeslot_request_earliest = {
	.request_type = MPSL_TIMESLOT_REQ_TYPE_EARLIEST,
	.params.earliest.hfclk = MPSL_TIMESLOT_HFCLK_CFG_NO_GUARANTEE,
	.params.earliest.priority = MPSL_TIMESLOT_PRIORITY_NORMAL,
	.params.earliest.length_us = TIMESLOT_LENGTH_US,
	.params.earliest.timeout_us = 1000000
};
static mpsl_timeslot_request_t timeslot_request_normal = {
	.request_type = MPSL_TIMESLOT_REQ_TYPE_NORMAL,
	.params.normal.hfclk = MPSL_TIMESLOT_HFCLK_CFG_NO_GUARANTEE,
	.params.normal.priority = MPSL_TIMESLOT_PRIORITY_NORMAL,
	.params.normal.distance_us = TIMESLOT_REQUEST_DISTANCE_US,
	.params.normal.length_us = TIMESLOT_LENGTH_US
};

static mpsl_timeslot_signal_return_param_t signal_callback_return_param;

/* Message queue for printing the signal type from timeslot callback */
K_MSGQ_DEFINE(callback_msgq, sizeof(uint32_t), 10, 4);

/* Message queue for requesting MPSL API calls to non-preemptible thread */
K_MSGQ_DEFINE(mpsl_api_msgq, sizeof(enum mpsl_timeslot_call), 10, 4);

static void error(void)
{
	printk("ERROR!\n");
	while (true) {
		/* Spin for ever */
		k_sleep(K_MSEC(1000));
	}
}

static mpsl_timeslot_signal_return_param_t *mpsl_timeslot_callback(
	mpsl_timeslot_session_id_t session_id,
	uint32_t signal_type)
{
	(void) session_id; /* unused parameter */

	mpsl_timeslot_signal_return_param_t *p_ret_val = NULL;

	switch (signal_type) {

	case MPSL_TIMESLOT_SIGNAL_START:
		/* No return action */
		signal_callback_return_param.callback_action =
			MPSL_TIMESLOT_SIGNAL_ACTION_NONE;
		p_ret_val = &signal_callback_return_param;

		/* Setup timer to trigger an interrupt (and thus the TIMER0
		 * signal) before timeslot end.
		 */
		nrf_timer_cc_set(NRF_TIMER0, NRF_TIMER_CC_CHANNEL0,
			TIMER_EXPIRY_US);
		nrf_timer_int_enable(NRF_TIMER0, NRF_TIMER_INT_COMPARE0_MASK);

		break;
	case MPSL_TIMESLOT_SIGNAL_TIMER0:

		/* Clear event */
		nrf_timer_int_disable(NRF_TIMER0, NRF_TIMER_INT_COMPARE0_MASK);
		nrf_timer_event_clear(NRF_TIMER0, NRF_TIMER_EVENT_COMPARE0);

		if (request_in_cb) {
			/* Request new timeslot when callback returns */
			signal_callback_return_param.params.request.p_next =
				&timeslot_request_normal;
			signal_callback_return_param.callback_action =
				MPSL_TIMESLOT_SIGNAL_ACTION_REQUEST;
		} else {
			/* Timeslot will be ended */
			signal_callback_return_param.callback_action =
				MPSL_TIMESLOT_SIGNAL_ACTION_END;
		}

		p_ret_val = &signal_callback_return_param;

		break;
	case MPSL_TIMESLOT_SIGNAL_SESSION_IDLE:
		break;
	case MPSL_TIMESLOT_SIGNAL_SESSION_CLOSED:
		break;
	default:
		printk("unexpected signal: %u", signal_type);
		error();
		break;
	}

	/* Put callback info in the message queue. */
	int err = k_msgq_put(&callback_msgq, &signal_type, K_NO_WAIT);

	if (err) {
		error();
	}

	return p_ret_val;
}

static void mpsl_timeslot_demo(void)
{
	int err;
	char input_char;
	enum mpsl_timeslot_call api_call;

	printk("-----------------------------------------------------\n");
	printk("Press a key to open session and request timeslots:\n");
	printk("* 'a' for a session where each timeslot makes a new request\n");
	printk("* 'b' for a session with a single timeslot request\n");
	input_char = console_getchar();
	printk("%c\n", input_char);

	if (input_char == 'a') {
		request_in_cb = true;
	} else if (input_char == 'b') {
		request_in_cb = false;
	} else {
		return;
	}

	api_call = OPEN_SESSION;
	err = k_msgq_put(&mpsl_api_msgq, &api_call, K_FOREVER);
	if (err) {
		error();
	}

	api_call = MAKE_REQUEST;
	err = k_msgq_put(&mpsl_api_msgq, &api_call, K_FOREVER);
	if (err) {
		error();
	}

	printk("Press any key to close the session.\n");
	console_getchar();

	api_call = CLOSE_SESSION;
	err = k_msgq_put(&mpsl_api_msgq, &api_call, K_FOREVER);
	if (err) {
		error();
	}
}

/* To ensure thread safe operation, call all MPSL APIs from a non-preemptible
 * thread.
 */
static void mpsl_nonpreemptible_thread(void)
{
	int err;
	enum mpsl_timeslot_call api_call = 0;

	/* Initialize to invalid session id */
	mpsl_timeslot_session_id_t session_id = 0xFFu;

	while (1) {
		if (k_msgq_get(&mpsl_api_msgq, &api_call, K_FOREVER) == 0) {
			switch (api_call) {
			case OPEN_SESSION:
				err = mpsl_timeslot_session_open(
					mpsl_timeslot_callback,
					&session_id);
				if (err) {
					error();
				}
				break;
			case MAKE_REQUEST:
				err = mpsl_timeslot_request(
					session_id,
					&timeslot_request_earliest);
				if (err) {
					error();
				}
				break;
			case CLOSE_SESSION:
				err = mpsl_timeslot_session_close(session_id);
				if (err) {
					error();
				}
				break;
			default:
				error();
				break;
			}
		}
	}
}

static void console_print_thread(void)
{
	uint32_t signal_type = 0;

	while (1) {
		if (k_msgq_get(&callback_msgq, &signal_type, K_FOREVER) == 0) {
			switch (signal_type) {
			case MPSL_TIMESLOT_SIGNAL_START:
				printk("Callback: Timeslot start\n");
				break;
			case MPSL_TIMESLOT_SIGNAL_TIMER0:
				printk("Callback: Timer0 signal\n");
				break;
			case MPSL_TIMESLOT_SIGNAL_SESSION_IDLE:
				printk("Callback: Session idle\n");
				break;
			case MPSL_TIMESLOT_SIGNAL_SESSION_CLOSED:
				printk("Callback: Session closed\n");
				break;
			default:
				printk("Callback: Other signal: %d\n",
				       signal_type);
				break;
			}
		}
	}
}

void main(void)
{

	int err = console_init();

	if (err) {
		error();
	}

	printk("-----------------------------------------------------\n");
	printk("             Nordic MPSL Timeslot sample\n");

	while (1) {
		mpsl_timeslot_demo();
		k_sleep(K_MSEC(1000));
	}
}

K_THREAD_DEFINE(console_print_thread_id, STACKSIZE, console_print_thread,
		NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);

K_THREAD_DEFINE(mpsl_nonpreemptible_thread_id, STACKSIZE,
		mpsl_nonpreemptible_thread, NULL, NULL, NULL,
		K_PRIO_COOP(MPSL_THREAD_PRIO), 0, 0);
