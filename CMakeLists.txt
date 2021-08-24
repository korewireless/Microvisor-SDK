cmake_minimum_required(VERSION 3.12)

add_library(twilio-microvisor-sdk mv_implib.o)

set_target_properties(twilio-microvisor-sdk PROPERTIES LINKER_LANGUAGE C)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/STM32U585xx_FLASH_mv.ld")

target_include_directories(twilio-microvisor-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
