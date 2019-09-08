#ifndef diae_gpio_server_dev_button_h
#define diae_gpio_server_dev_button_h

#include "dev_inpin.h"
#include "dev_gpiolib.h"

namespace DiaDevice {
    class Button
    {
    private:
        InPin *inPin;
        int normallyOn;
        int prevStatus;
    public:
        Button() {
            inPin = 0;
        }
        void Init(unsigned char pin, GPIOWrapper * gpio, int normallyOn) {
            inPin = new InPin();
            inPin->Init(pin, gpio);
            this->normallyOn = normallyOn;
            prevStatus = 0;
        }
        ~Button() {
            if (inPin) {
                delete inPin;
                inPin = 0;
            }
        }
        int Pushed() {
            if (!inPin->Enabled()) return 0;
            if (normallyOn) {
                return !inPin->Read();
            } else {
                return inPin->Read();
            }
        }
        int Clicked() {
            if(Pushed() && !prevStatus) {
                prevStatus = 1;
                return 1;
            }
            prevStatus = Pushed();
            return 0;
        }
    };
}

#endif
