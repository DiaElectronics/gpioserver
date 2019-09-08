#ifndef diae_gpio_server_dev_gpiolib_h
#define diae_gpio_server_dev_gpiolib_h

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define PIN_UNKNOWN_DIRECTION 0
#define PIN_FOR_OUTPUT 1
#define PIN_FOR_INPUT 2

#define PIN_LOW 0
#define PIN_HIGH 1

namespace DiaDevice {
    // Let's use inheritance here to avoid:
    // (a) ifdef
    // (b) creation of interface structures manually
    // if anybody knows how to do that simpler, please let me knows
    
    class GPIOWrapper {
        public:
        virtual void SetDirection(unsigned char pin, char direction) = 0;
        virtual void Turn(unsigned char pin, char isActive) = 0;
        virtual int  Read(unsigned char pin) = 0;
    };
    
    class GPIOWrapperEmpty: public GPIOWrapper {
    public:
        void SetDirection(unsigned char pin, char direction) override {
            fprintf(stdout, "pin:%d, direction:%d\n", (int)pin, (int)direction);
        }
        
        void Turn(unsigned char pin, char isActive) override {
            fprintf(stdout, "turn pin:%d, act:%d\n", (int)pin, (int)isActive);
        }
        
        int Read(unsigned char pin) override {
            // only 11's pin is on (dont forget its fake)
            unsigned char turnedOnPin = 11;
            return pin == turnedOnPin;
        }
        ~GPIOWrapperEmpty() {
        }
    };
    
    class GPIOWrapperWiringPi: public GPIOWrapper {
    public:
        ~GPIOWrapperWiringPi() {
        }
        
        void SetDirection(unsigned char pin, char direction) override {
            switch (direction)
            {
            case PIN_FOR_INPUT:
                pinMode(pin, INPUT);
                break;
                
            case PIN_FOR_OUTPUT:
                pinMode(pin, OUTPUT);
                break;
            
            default:
                fprintf(stderr, "Something wrong with the code. Unknown pin direction passed\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
        
        void Turn(unsigned char pin, char isActive) override {
            if (isActive) {
                digitalWrite(pin, HIGH);
            } else {
                digitalWrite(pin, LOW);
            }
        }
        
        int Read(unsigned char pin) override {
            if (digitalRead(pin)) {
                return PIN_HIGH;                
            } 
            return PIN_LOW;
        }
    };
}
#endif
