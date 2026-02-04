//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_CPU_H
#define SWAYBAR_FETCH_CPU_H


class Cpu {
    struct _cpu_state {
        long long user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0;
        long long get_state() const { return user + nice + system + idle + iowait + irq + softirq + steal; }
    };
    ifstream _file_cpu = ifstream("/proc/stat");
};


#endif //SWAYBAR_FETCH_CPU_H