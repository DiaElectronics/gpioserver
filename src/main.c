#include "dia_gpio.h"

int main(int argc, char ** argv) {
    DiaGPIO gpio;
    int key = gpio.LastKey();
    printf("last pressed key = %d\n", key);
    return 0;
}
