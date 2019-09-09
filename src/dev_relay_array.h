#ifndef diae_gpio_server_dev_relay_array_h
#define diae_gpio_server_dev_relay_array_h

#include "dev_relay.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "dev_timer.h"
#include "dev_relay.h"

#define MAX_RELAYS 9
#define REL_CHECK_INTERVAL_NANO 1 * 1000000

namespace DiaDevice {
    class RelayArray {
    private:
        int mustBeStopped;
        int alreadyStarted;
        Timer timer;
        Relay *Relays[MAX_RELAYS];        
        pthread_t WorkingThread;
        
        static void * relayLoop(void * args) {
            if (!args) {
                fprintf(stderr, "ERROR: relayLoop args passed null\n");
                return 0;
            }
            RelayArray * r = (RelayArray*)args;
            
            timespec required_time;
            timespec remaining_time;
            required_time.tv_sec = 0;
            required_time.tv_nsec = REL_CHECK_INTERVAL_NANO;
            for(;!r->mustBeStopped;) {
                long long currentTime = r->timer.CurrentTimeMS();
                for (int i=0;i<MAX_RELAYS;i++) {
                    if(r->Relays[i]) {
                        r->Relays[i]->Tick(currentTime);
                    }
                }
                nanosleep(&required_time, &remaining_time);
            }
            return 0;
        }
    public:
        RelayArray() {
            for (int i=0;i<MAX_RELAYS;i++) Relays[i] = 0;
            mustBeStopped = 0;
            alreadyStarted = 0;
        }
        
        void AddRelay(int relayNumber, unsigned char pin, GPIOWrapper *gpio) {
            if (Relays[relayNumber]) {
                fprintf(stderr, "ERROR: button %d was already assigned\n", relayNumber);
            }
            Relays[relayNumber] = new Relay();
            Relays[relayNumber]->Init(pin, gpio);
        }
        
        void Start() {
            int err = pthread_create(&WorkingThread, 0, RelayArray::relayLoop, this);
            if (!err) {
                alreadyStarted = 1;
            } else {
                fprintf(stderr, "ERROR: can't start button thread\n");
            }
        }
        
        inline void SetRelay(int rel, int percent) {
            if(rel<0 || rel>=MAX_RELAYS) {
                fprintf(stderr, "ERROR, setting up program for not existing %d relay\n", rel);
                return;
            }
            if(Relays[rel]) {
                Relays[rel]->SetActivePercent(percent);
                return;
            } 
            fprintf(stderr, "ERROR, NULL relay accessed %d\n", rel);
        }
        
        ~RelayArray() {
            void *res;
            if(alreadyStarted) {
                mustBeStopped = 1;
                pthread_join(WorkingThread, &res);
            }
            for(int i=0;i<MAX_RELAYS;i++) {
                if(Relays[i]) {
                    delete Relays[i];
                    Relays[i] = 0;
                }
            }
        }
    };
}

#endif
