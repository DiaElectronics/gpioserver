#ifndef diae_gpio_server_dev_inpin_h
#define diae_gpio_server_dev_inpin_h

#include "dev_timer.h"
#include "dev_gpiolib.h"

namespace DiaDevice {
    class InPin {
        private:
        unsigned char physicalPin;
        GPIOWrapper * gpio;
        int gpioSelfInitilized;
        
        public:
        int inline Enabled() {
            return physicalPin>=0;
        }
        
        InPin() {
            physicalPin = -1;
        }
        
        void Init(unsigned char physicalPinNumber, GPIOWrapper *gpioWrapper) {
            if (!gpioWrapper) {
                gpioWrapper = new GPIOWrapperEmpty();
                gpioSelfInitilized = 1;
            } else {
                gpioSelfInitilized = 0;
            }
            
            gpio = gpioWrapper;
            physicalPin = physicalPinNumber;
            
            gpio->SetDirection(physicalPin, PIN_FOR_INPUT);
        }
        
        int Read() {
            return gpio->Read(physicalPin);            
        }
        
        ~InPin() {
            if(gpioSelfInitilized) {
                delete gpio;
                gpio = 0;
            }
        }
    };
}
#endif
