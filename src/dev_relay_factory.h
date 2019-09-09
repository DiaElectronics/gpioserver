#ifndef diae_gpio_server_dev_relay_factory_h
#define diae_gpio_server_dev_relay_factory_h

#include "dev_relay_array.h"
#include "dev_gpiolib.h"

namespace DiaDevice {
    class RelayFactory {
        public:
        static void FillDefault(RelayArray *relArray) {
            if (!relArray) {
                fprintf(stderr, "relArray is null at fill default relays\n");
                exit(1);
            }
            GPIOWrapper * wiringPi = new GPIOWrapperWiringPi();
            relArray->AddRelay(1, 15, wiringPi);
            relArray->AddRelay(2, 16, wiringPi);
            relArray->AddRelay(3, 1, wiringPi);
            relArray->AddRelay(4, 4, wiringPi);
            relArray->AddRelay(5, 5, wiringPi);
            relArray->AddRelay(6, 6, wiringPi);
            relArray->AddRelay(7, 27, wiringPi);
        }
    };
}

#endif
