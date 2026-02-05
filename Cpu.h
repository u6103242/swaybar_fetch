//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_CPU_H
#define SWAYBAR_FETCH_CPU_H

#include <string>
#include <fstream>
#include <vector>

#include "Constants.h"

class Cpu {
public:
    /// Returns cpu status
    std::string get_status(STATUS_MODE mode);

private:
    /// Values from /proc/stat at a given point in time
    struct _cpu_state {
        long long user, nice, system, idle, iowait, irq, softirq, steal;
        long long get_state() const { return user + nice + system + idle + iowait + irq + softirq + steal; }
    };

    /// Difference between two states
    struct _delta_state {
        long long total_dif, idle_dif;
        double perc;
    };

    /// Cpu previous state
    _cpu_state _cpu_prev_state;
    /// Cpu current state
    _cpu_state _cpu_curr_state;

    /// Cpu delta
    _delta_state _cpu_delta;

    /// Values per core
    std::vector<_cpu_state> _per_core_prev_state;
    std::vector<_cpu_state> _per_core_curr_state;
    std::vector<_delta_state> _per_core_delta;

    /// File stream of /proc/stat
    std::ifstream _file_cpu = std::ifstream("/proc/stat");

    /// Fetches the cpu status
    void _fetch_data(STATUS_MODE mode);

    /// Processes the fetched data
    void _interpret_data(STATUS_MODE mode);
};


#endif //SWAYBAR_FETCH_CPU_H