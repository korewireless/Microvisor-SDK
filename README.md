# Microvisor System Call Stubs

This repository contains the following artifacts for building user applications under Microvisor:

- A linker script which includes the memory map user applications have access to.
- A header file declaring the [non-secure callable (NSC) functions provided by Microvisor](https://www.twilio.com/docs/iot/microvisor/syscalls) and all relevant data structure definitions.
- The object file used at runtime which points to the addresses of the NSC functions.

**Note** This repo is provided primarily as a source for our [Microvisor STM32U5 HAL](https://github.com/korewireless/Microvisor-HAL-STM32U5), [Microvisor FreeRTOS demo](https://github.com/korewireless/Microvisor-Demo-CMSIS-Freertos) and other projects. It is also intended for developers with very specific project configuration requirements. We recommend all other developers start with one of the above projects.

If you intend to install and use a HAL too, you must use our [Microvisor STM32U5 HAL](https://github.com/korewireless/Microvisor-HAL-STM32U5) project as STMicro’s generic U5 Cube HAL will NOT work with Microvisor.

## Usage

A `CMakeLists.txt` file is provided which exports a library and linker flags to use in your application. This project is consumed by both our [Microvisor STM32U5 HAL](https://github.com/korewireless/Microvisor-HAL-STM32U5), [Microvisor FreeRTOS demo](https://github.com/korewireless/Microvisor-Demo-CMSIS-Freertos) and other projects.

The environment variable `MV_ARCH` is required to be set by your consuming project. The only currently valid value for this is `"stm32u5"`.

You can also consume the included artifacts directly:

- `stm32u5/mv_syscalls.h` should be included in your project to provide declarations for Microvisor’s NSC functions.
- `stm32u5/mv_syscalls.o` should be linked against your binary to provide addresses for the NSC functions defined in `mv_syscalls.h`.
- `stm32u5/STM32U585xx_FLASH_mv.ld` defines the memory map where your program should be loaded and should be passed to the linker flags in your project.

## Breaking Changes

- During development of MQTT features, we altered the C representation of `MvHttpRequest` to put
  buffers and their associated lengths in one structure. This was not an ABI change, but did
  change the C API.

## Support and Feedback

[Please contact Microvisor Support](mailto:support@microvisor.com).

## Copyright

This code is © 2021-24, KORE Wireless.
