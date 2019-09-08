#ifndef diae_gpio_server_dev_inpin_h
#define diae_gpio_server_dev_inpin_h

#define PIN_COUNT 9
#include "dev_inpin.h"
#include "dev_outpin.h"

namespace DiaDevice {    
    class Device {
        public:
        InPin Btn[PIN_COUNT];
        OutPin BtnLED[PIN_COUNT];
        OutPin Relay[PIN_COUNT];
        InPin CoinPin;
        InPin BanknotePin;
        
        Device() {           
        }
        
        void InitFake() {            
        }
        
        void InitDefaults() {
        }
        
        void InitConfig() {
        }
        
        void Relay(int percent) {
            
        }
    };
}
#endif
