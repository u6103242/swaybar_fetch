//
// Created by adri on 2/4/26.
//

#include "Ram.h"

void Ram::_get_ram() {
    string line;
    unsigned long free_kb = 0, total_kb = 0;

    while (getline(_file_ram, line)) {
        if (line.compare(0, 9, "MemTotal:") == 0)
            total_kb = stoul(line.substr(10));
        else if (line.compare(0, 13, "MemAvailable:") == 0) {
            free_kb = stoul(line.substr(14));
            break;
        }
        _file_ram.ignore(1024, '\n');
    }

    _file_ram.clear();
    _file_ram.seekg(0, ios::beg);

    const unsigned long used_kb = total_kb - free_kb;
    double scaled_used;
    const char* unit;

    if (used_kb >= 1048576) {
        scaled_used = used_kb / 1024.0 / 1024.0;
        unit = "Gb";
    } else if (used_kb >= 1024) {
        scaled_used = used_kb / 1024.0;
        unit = "Mb";
    } else {
        scaled_used = static_cast<double>(used_kb);
        unit = "kb";
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "RAM: %5.2f%s", scaled_used, unit);

    _formatted_status = buf;
}