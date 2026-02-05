//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_RAM_H
#define SWAYBAR_FETCH_RAM_H

#include <string>
#include <fstream>

#include "Constants.h"

class Ram {
public:
    /// Returns ram status
    std::string get_status(STATUS_MODE mode);

private:
    /// File stream of /proc/meminfo
    std::ifstream _file_mem = std::ifstream("/proc/meminfo");

    unsigned long _ram_free_kb, _ram_total_kb, _ram_used_kb;
    unsigned long _swap_free_kb, _swap_total_kb, _swap_used_kb;

    /// Fetches the ram status
    void _fetch_data(STATUS_MODE mode);

    /// Processes the fetched data
    void _interpret_data(STATUS_MODE mode);
};


#endif //SWAYBAR_FETCH_RAM_H