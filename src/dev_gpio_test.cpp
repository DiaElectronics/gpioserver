#include <stdio.h>

#include "dev_button_array.h"
#include "dev_button_factory.h"

#include <unistd.h>

int main(int argc, char ** argv) {
    DiaDevice::ButtonArray btnArray;
    DiaDevice::ButtonsFactory::FillDefaultButtons(&btnArray, 1);
    btnArray.Start();
    timespec required;
    timespec actual;
    required.tv_sec = 0;
    required.tv_nsec = 100000000;
    for(;;) {
        int clicked=btnArray.LastClicked();
        if (clicked!=NOT_CLICKED) {
            fprintf(stdout, "clicked: %d \n", clicked);
        }
        nanosleep(&required, &actual);
    }
    return 0;
}
