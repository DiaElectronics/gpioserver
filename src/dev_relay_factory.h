#ifndef diae_gpio_server_dev_relay_factory_h
#define diae_gpio_server_dev_relay_factory_h

#include "dev_relay_array.h"
#include "dev_gpiolib.h"

namespace DiaDevice {
    class RelayFactory {
        public:
        static void FillDefault(RelayArray *relArray, GPIOWrapper * gpio) {
            if (!relArray) {
                fprintf(stderr, "relArray is null at fill default relays\n");
                exit(1);
            }
            relArray->AddRelay(1, 15, gpio);
            relArray->AddRelay(2, 16, gpio);
            relArray->AddRelay(3, 1, gpio);
            relArray->AddRelay(4, 4, gpio);
            relArray->AddRelay(5, 5, gpio);
            relArray->AddRelay(6, 6, gpio);
            relArray->AddRelay(7, 27, gpio);
        }
    };
}

#endif
