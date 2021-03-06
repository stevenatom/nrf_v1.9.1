**Models of some of the HW present in a NRF52xxx.**<br>
Where relevant differences exist, these models try to align with a NRF52382.

This repo contains both models of the NRF52 HW as well as some replacement nrfx
HAL functions. When used in combination with the real nrfx, these should enable code
meant for the nrfx to run without needing further changes.
This includes Zephyr SW.

When compiling this component using the provided Makefile (not with Zephyr's build system),
the environment variable `NRFX_BASE` must be set to the path where a nrfx has been cloned. 
The nrfx must be at least version 2.3.0.
So for example, if the nrfx has been cloned as:

```
cd /some_path/nrfx/
git clone git@github.com:NordicSemiconductor/nrfx.git .
```
`NRFX_BASE` must be set as:

```
export NRFX_BASE=/some_path/nrfx/
```

See the [nrfx/hal/README.md](../src/nrfx/hal/README.md) for more details.

This models can be used directly with
[Zephyr's nrf52_bsim target](https://docs.zephyrproject.org/latest/boards/posix/nrf52_bsim/doc/index.html).

The NRF_RADIO peripheral model uses [BabbleSim](http://babblesim.github.io)
for the radio environment simulation.

For more information about the HW models, or how to use them without the
nrf52_bsim please refer to [README_HW_models.md](./README_HW_models.md)

## Faithfullness of these models

These models are accurate enough to allow the current Zephyr to run, and
its BLE stack to function. For more details please see the notes on the source
files for each peripheral model.

These models are based solely on
[the public SOC specifications](https://infocenter.nordicsemi.com/topic/struct_nrf52/struct/nrf52832.html?cp=3_1)

They have been developed without any other knowledge and probably contain
inacuracies, and defects.

Several of the peripherals which are not necessary for typical BLE applications
are not modelled. Moreover, in the modelled peripherals, functionality which is
not used in the Zephyr drivers is normally not modelled either.
The [TODO](../TODO.txt) file list some mayor omissions.
It is certainly possible to expand these models to include the rest.
