cmake_minimum_required(VERSION 3.20.0)

set(BOARD arduino_nano_33_ble)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(soia_weather_station_firmware)

target_sources(app PRIVATE 
    src/bluetooth.c
    src/main.c
)