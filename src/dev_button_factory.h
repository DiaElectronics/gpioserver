#ifndef diae_gpio_server_dev_button_factory_h
#define diae_gpio_server_dev_button_factory_h

#include "dev_button_array.h"
#include "dev_gpiolib.h"

namespace DiaDevice {
    class ButtonsFactory {
        public:
        static void FillDefault(ButtonArray *btnArray, int normallyOn) {
            if (!btnArray) {
                fprintf(stderr, "btnArray is null at fill default buttons\n");
                exit(1);
            }
            GPIOWrapper * wiringPi = new GPIOWrapperWiringPi();
            btnArray->AddButton(1, 13, wiringPi, normallyOn);
            btnArray->AddButton(2, 14, wiringPi, normallyOn);
            btnArray->AddButton(3, 21, wiringPi, normallyOn);
            btnArray->AddButton(4, 22, wiringPi, normallyOn);
            btnArray->AddButton(5, 23, wiringPi, normallyOn);
            btnArray->AddButton(6, 24, wiringPi, normallyOn);
            btnArray->AddButton(7, 25, wiringPi, normallyOn);
        }
    };
}

#endif
