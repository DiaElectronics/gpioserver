#ifndef diae_gpio_server_dev_outpin_h
#define diae_gpio_server_dev_outpin_h

#include "dev_gpiolib.h"

namespace DiaDevice {
    class OutPin {
        private: 
        unsigned char physicalPin;
        long long changedOn;
        char active;
        long long activeMS;
        
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
            long long currentTimeMS = timer.CurrentTimeMS();
            totalSwitches++;
            if(active) {
                activeMS += currentTimeMS - changedOn;
                active = 0;
            } else {
                active = 1;
            }
            gpio->Turn(physicalPin, active);
            changedOn = currentTimeMS;
        }
        
        long long ChangedOn() {
            return changedOn;
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
        
        int Active() {
            return active;
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
