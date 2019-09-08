#ifndef diae_gpio_server_dev_outpin_h
#define diae_gpio_server_dev_outpin_h

#include "dev_timer.h"
#include "dev_gpiolib.h"

namespace DiaDevice {
    class OutPin {
        private: 
        unsigned char physicalPin;
        long changedOn;
        char active;
        long activeMS;
        int totalSwitches;
        int direction;
        Timer timer;
        GPIOWrapper * gpio;
        int gpioSelfInitilized;
        
        public:
        int inline Enabled() {
            return physicalPin>=0;
        }
        
        OutPin() {
            physicalPin = -1;
            activeMS = 0;
            totalSwitches = 0;
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
            
            gpio->SetDirection(physicalPin, PIN_FOR_OUTPUT);
            gpio->Turn(physicalPin, 0);
            active = 0;
            changedOn = timer.CurrentTimeMS();  
        }
        
        void Reverse() {
            totalSwitches++;
            long currentTime = timer.CurrentTimeMS();
            if(active) {
                activeMS += currentTime - changedOn;
                active = 0;
            } else {
                active = 1;
            }
            gpio->Turn(physicalPin, active);
            changedOn = currentTime;
        }
        
        void TurnOff() {
            if(active) Reverse();
        }
        
        void TurnOn() {
            if(!active) Reverse();
        }
        
        int TotalSwitches() {
            return totalSwitches;
        }
        
        long ActiveTimeMS() {
            return activeMS;
        }
        
        ~OutPin() {
            if(gpioSelfInitilized) {
                delete gpio;
                gpio = 0;
            }
        }
    };
}
#endif
