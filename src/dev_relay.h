#ifndef diae_gpio_server_dev_relay_h
#define diae_gpio_server_dev_relay_h

#include "dev_outpin.h"
#include "dev_gpiolib.h"

// TOTAL_PERIOD_MS MUST BE POSSIBLE TO DIVIDE BY 100
#define TOTAL_PERIOD_MS 500
namespace DiaDevice {
    class Relay
    {
    private:
        OutPin *outPin;
        long long onTime;
        long long offTime;
    public:
        Relay() {
            outPin = 0;
            offTime = 500;
            onTime = 0;
        }
        void Init(unsigned char pin, GPIOWrapper * gpio) {
            outPin = new OutPin();
            outPin->Init(pin, gpio);
        }
        ~Relay() {
            if (outPin) {
                delete outPin;
                outPin = 0;
            }
        }
        void SetActivePercent(int percent) {
            if (percent<0) {
                fprintf(stderr, "ERROR: active percent [%d] < 0, set to 0 \n", percent);
                percent = 0;
            }
            if (percent>100) {
                fprintf(stderr, "ERROR: active percent [%d] > 100, set to 0 \n", percent);
                percent = 100;
            }
            onTime = (percent * TOTAL_PERIOD_MS)/100;
            offTime = TOTAL_PERIOD_MS - onTime;
        }
        
        void Tick (long long timeInMS) {
            outPin->Tick(timeInMS);
            if (!onTime) {
                // must be turned off
                if(outPin->Active()) {
                    outPin->Reverse();
                }
                return;
            }
            if (!offTime) {
                // must be turned on
                if(!outPin->Active()) {
                    outPin->Reverse();
                }
                return;
            }
            
            // switching back and forth
            if (outPin->Active()) {
                if(outPin->ChangedOn() + onTime >= timeInMS) {
                    outPin->Reverse();
                }
            } else {
                if(outPin->ChangedOn() + offTime >= timeInMS) {
                    outPin->Reverse();
                }
            }
        }
    };
}

#endif
