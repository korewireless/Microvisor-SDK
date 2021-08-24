# Twilio Microvisor API Stubs

This repository contains the following artifacts for building user applications:

- Linker script which includes the memory map user applications have access to
- Header file declaring all non-secure callable (NSC) function and struct definitions
- Object file used at runtime which points to the addresses of NSC functions

## Usage

A `CMakeLists.txt` file is provided which exports a library and linker flags to use in 
your application.  This project is consumed by both our [twilio-microvisor-hal](https://github.com/twilio/twilio-microvisor-hal) and 
[twilio-microvisor-freertos](https://github.com/twilio/twilio-microvisor-freertos) projects.

You can also consume the included artifacts directly:

- `mv_api.h` should be included in your project to provide declarations for NSC functions
  into the Microvisor secure side
- `mv_implib.o` should be linked against your binary to provide addresses for the NSC
  functions defined in `mv_api.h`
- `STM32U585xx_FLASH_mv.ld` defines the memory map where your program should be loaded and
  should be passed to the linker flags in your project
