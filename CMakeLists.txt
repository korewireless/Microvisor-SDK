cmake_minimum_required(VERSION 3.12)

add_library(microvisor-sdk ${MV_ARCH}/mv_syscalls.o)

set_target_properties(microvisor-sdk PROPERTIES LINKER_LANGUAGE C)

if(NOT CMAKE_EXE_LINKER_FLAGS MATCHES "STM32U585xx_FLASH_mv.ld")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/${MV_ARCH}/STM32U585xx_FLASH_mv.ld" CACHE INTERNAL "" FORCE)
endif()

target_include_directories(microvisor-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${MV_ARCH})
