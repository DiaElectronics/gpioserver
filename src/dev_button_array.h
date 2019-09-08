#ifndef diae_gpio_server_dev_button_array_h
#define diae_gpio_server_dev_button_array_h

#include "dev_button.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_BUTTONS 9
#define NOT_CLICKED -1
#define BTN_CHECK_INTERVAL_NANO 10 * 1000000;

namespace DiaDevice {
    class ButtonArray {
    private:
        int lastClicked;
        int mustBeStopped;
        int alreadyStarted;
        Button *Buttons[MAX_BUTTONS];        
        pthread_t WorkingThread;
        
        static void * buttonLoop(void * args) {
            if (!args) {
                fprintf(stderr, "ERROR: ButtonLoop args passed null\n");
                return 0;
            }
            ButtonArray * b = (ButtonArray*)args;
            
            timespec required_time;
            timespec remaining_time;
            required_time.tv_sec = 0;
            required_time.tv_nsec = BTN_CHECK_INTERVAL_NANO;
            for(;!b->mustBeStopped;) {
                for (int i=0;i<MAX_BUTTONS;i++) {
                    if(b->Buttons[i] && b->Buttons[i]->Clicked()) {
                        b->lastClicked = i;
                        continue;
                    }
                }
                nanosleep(&required_time, &remaining_time);
            }
            return 0;
        }
    public:
        ButtonArray() {
            for (int i=0;i<MAX_BUTTONS;i++) Buttons[i] = 0;
            mustBeStopped = 0;
            alreadyStarted = 0;
            lastClicked = NOT_CLICKED;
        }
        
        void AddButton(int buttonNumber, unsigned char pin, GPIOWrapper *gpio, int normallyOn) {
            if (Buttons[buttonNumber]!=0) {
                fprintf(stderr, "ERROR: button %d was already assigned\n", buttonNumber);
            }
            Buttons[buttonNumber] = new Button();
            Buttons[buttonNumber]->Init(pin, gpio, normallyOn);
        }
        
        void Start() {
            int err = pthread_create(&WorkingThread, 0, ButtonArray::buttonLoop, this);
            if (!err) {
                alreadyStarted = 1;
            } else {
                fprintf(stderr, "ERROR: can't start button thread\n");
            }
        }
        

        
        int LastClicked() {
            int res = lastClicked;
            lastClicked = NOT_CLICKED;
            return res;
        }
        
        ~ButtonArray() {
            void *res;
            if(alreadyStarted) {
                mustBeStopped = 1;
                pthread_join(WorkingThread, &res);
            }
            for(int i=0;i<MAX_BUTTONS;i++) {
                if(Buttons[i]) {
                    delete Buttons[i];
                    Buttons[i] = 0;
                }
            }
        }
    };
}

#endif
