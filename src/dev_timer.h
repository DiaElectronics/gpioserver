#ifndef diae_gpio_server_dev_timer_h
#define diae_gpio_server_dev_timer_h

#define ONE_MILLION 1000000

#include <time.h>

namespace DiaDevice {
    class Timer {
    private:
        long long           ms; // Milliseconds
        time_t              s;  // Seconds
        struct timespec     spec;
        long long           coef;
        
    public:
        long long CurrentTimeMS() {
            clock_gettime(CLOCK_REALTIME, &spec);
            s  = spec.tv_sec;
            ms = (long long)spec.tv_nsec / ONE_MILLION; // Convert nanoseconds to milliseconds
            return s*(long long)1000 + ms;
        }
    };
}
#endif
