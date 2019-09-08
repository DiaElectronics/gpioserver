#include <stdio.h>
#include "dev_timer.h"
#include "dev_outpin.h"
#include "dev_inpin.h"
#include "dev_button_array.h"
#include <unistd.h>

int main(int argc, char ** argv) {
    DiaDevice::Timer timer;
    long time_of_library_creation = 1567872233377;
    if (timer.CurrentTimeMS()<1567872233377) {
        fprintf(stderr, "ERR: current time is not correct (less than library creation date)\n");
    }
    DiaDevice::OutPin pin;
    pin.Init(10, 0);
    pin.Reverse();
    usleep(1000);
    pin.Reverse();
    long actTime = pin.ActiveTimeMS();
    if (actTime<1 || actTime >2) {
        fprintf(stderr, "ERR: active time is not correct\n");
    }
    if (pin.TotalSwitches()!=2) {
        fprintf(stderr, "ERR: total switches count is not correct\n");
    }
    
    DiaDevice::InPin inPinOn;
    inPinOn.Init(11, 0);
    if (!inPinOn.Read()) {
        fprintf(stderr, "ERR: read must be 1 for test gpio 11th pin\n");
    }
    
    DiaDevice::InPin inPinOff;
    inPinOff.Init(12, 0);
    if (inPinOff.Read()) {
        fprintf(stderr, "ERR: read must be 0 for test gpio 12th pin \n");
    }
    
    DiaDevice::ButtonArray buttons;
    buttons.AddButton(1, 11, 0, 0);
    buttons.AddButton(2, 12, 0, 0);
    buttons.Start();
    
    if (argc > 1) {
        
    }
    
    return 0;
}
