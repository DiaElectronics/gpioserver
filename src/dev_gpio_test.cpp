#include <stdio.h>

#include "dev_button_array.h"
#include "dev_button_factory.h"
#include "dev_relay_array.h"
#include "dev_relay_factory.h"
#include "dev_gpiolib.h"

#include <unistd.h>

int main(int argc, char ** argv) {    
    DiaDevice::GPIOWrapper * gpio = new DiaDevice::GPIOWrapperWiringPi();
    //DiaDevice::GPIOWrapper * gpio = new DiaDevice::GPIOWrapperEmpty();
    DiaDevice::ButtonArray btnArray;
    DiaDevice::ButtonsFactory::FillDefault(&btnArray, gpio, 1);
    DiaDevice::RelayArray relArray;
    DiaDevice::RelayFactory::FillDefault(&relArray, gpio);
    
    btnArray.Start();
    relArray.Start();
    timespec required;
    timespec actual;
    required.tv_sec = 0;
    required.tv_nsec = 100000000;
    for(;;) {
        int clicked=btnArray.LastClicked();
        if (clicked!=NOT_CLICKED) {
            fprintf(stdout, "clicked: %d \n", clicked);
            for (int i=1;i<MAX_RELAYS;i++) {
                if(i==clicked) {
                    relArray.SetRelay(i, 50);
                } else {
                    relArray.SetRelay(i, 0);
                }
            }
        }
        nanosleep(&required, &actual);
    }
    return 0;
}
