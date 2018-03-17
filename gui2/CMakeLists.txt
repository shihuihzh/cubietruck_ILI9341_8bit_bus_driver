cmake_minimum_required(VERSION 3.9)
project(CB_LCD)

set(CMAKE_CXX_STANDARD 11)

add_executable(CB_LCD
        LCD/common.c
        LCD/common.h
        LCD/gpio_lib.c
        LCD/gpio_lib.h
        LCD/lcd.c
        LCD/lcd.h
        font.h
        gpio_lib.c
        gpio_lib.h
        gui.c
        gui.h
        main.c
        pic.h
        pysunxi.c
        test.c
        test.h
        tft.c)
