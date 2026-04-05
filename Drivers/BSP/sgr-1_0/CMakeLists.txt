cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(BSP_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Components/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Components/M24xx
)

set(BSP_Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_eeprom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_temperature.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sgr-1_0_wheatstone_bridge.c
)

add_library(bsp_sgr-1_0)
target_include_directories(bsp_sgr-1_0 PUBLIC ${BSP_Include_Dirs})
target_sources(bsp_sgr-1_0 PUBLIC ${BSP_Drivers_Src})
target_link_libraries(bsp_sgr-1_0 PUBLIC stm32cubemx)
