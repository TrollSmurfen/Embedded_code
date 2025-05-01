cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Project name
project(lcd_distance_sensor)

# Initialize the Pico SDK
pico_sdk_init()

# Add the executable
add_executable(${PROJECT_NAME}
    main.cpp
    lcd.cpp
)

# Include directories (for the lcd.h file)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link the necessary libraries
target_link_libraries(${PROJECT_NAME}
    pico_stdlib          # Standard Pico libraries
    hardware_i2c         # I2C hardware library
)

# Add extra outputs (e.g., UF2 file)
pico_add_extra_outputs(${PROJECT_NAME})
