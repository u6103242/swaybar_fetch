//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_CPU_H
#define SWAYBAR_FETCH_CPU_H

#include <string>
#include <fstream>

#include "Constants.h"

class Cpu {
public:
    /// Values from /proc/stat at a given point in time
    struct _cpu_state {
        long long user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0;
        long long get_state() const { return user + nice + system + idle + iowait + irq + softirq + steal; }
    };

    /// Returns cpu status
    std::string get_status(STATUS_MODE mode);

    /// Initializes cpu values
    Cpu();
private:
    /// File stream of /proc/stat
    ifstream _file_cpu = ifstream("/proc/stat");
    /// Cpu state at previous cpu tick
    _cpu_state _prev_state;

    /// Formatted data from latest cpu status
    std::string _formatted_status;

    /// Fetches the cpu status
    void _get_cpu();
};


#endif //SWAYBAR_FETCH_CPU_H