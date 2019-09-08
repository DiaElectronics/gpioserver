#!/bin/bash

g++ dev_test.cpp -lpthread -o dev_test.exe
g++ dev_gpio_test.cpp -lpthread -lwiringPi -o dev_gpio_test.exe
