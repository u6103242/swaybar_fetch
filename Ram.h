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
    /// Returns cpu status
    std::string get_status(STATUS_MODE mode);

private:
    /// File stream of /proc/stat
    ifstream _file_ram = ifstream("/proc/meminfo");
    /// Formatted data from latest cpu status
    std::string _formatted_status;

    /// Fetches the cpu status
    void _get_ram();
};


#endif //SWAYBAR_FETCH_RAM_H