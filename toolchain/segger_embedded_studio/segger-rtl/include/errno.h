/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef __SEGGER_RTL_ERRNO_H
#define __SEGGER_RTL_ERRNO_H

#include "__SEGGER_RTL.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#if defined(__ARM_EABI__) && !defined(__NO_AEABI_ERR__)

extern const int __aeabi_EDOM;
extern const int __aeabi_EILSEQ;
extern const int __aeabi_ERANGE;

#define EDOM     (__aeabi_EDOM)
#define EILSEQ   (__aeabi_EILSEQ)
#define ERANGE   (__aeabi_ERANGE)

#define EHEAP     0x04
#define ENOMEM    0x05
#define EINVAL    0x06

#else

/*********************************************************************
*
*       Error names
*
*  Description
*    Symbolic error names for raised errors.
*/
#define EDOM      0x01   // Domain error
#define EILSEQ    0x02   // Illegal multibyte sequence in conversion
#define ERANGE    0x03   // Range error
#define EHEAP     0x04   // Heap is corrupt
#define ENOMEM    0x05   // Out of memory
#define EINVAL    0x06   // Invalid parameter

#endif

#define EAGAIN    0x07
#define ECHILD    0x08
#define EMLINK    0x09
#define ENOENT    0x0A

/*********************************************************************
*
*       errno
*
*  Description
*    Macro returning the current error.
*
*  Additional information
*    The value in errno is significant only when the return value of the
*    call indicated an error.  A function that succeeds is allowed to
*    change errno.  The value of errno is never set to zero by a library
*    function.
*/
#define errno   (*__SEGGER_RTL_X_errno_addr())

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

#ifdef __cplusplus
}
#endif

#endif

/*************************** End of file ****************************/
