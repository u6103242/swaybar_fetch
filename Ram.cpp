//
// Created by adri on 2/4/26.
//

#include "Ram.h"

std::string Ram::get_status(STATUS_MODE mode) {
    _get_ram();

    double scaled_used;
    const char* unit;

    if (_used_kb >= 1048576) {
        scaled_used = _used_kb / 1024.0 / 1024.0;
        unit = "Gb";
    } else if (_used_kb >= 1024) {
        scaled_used = _used_kb / 1024.0;
        unit = "Mb";
    } else {
        scaled_used = static_cast<double>(_used_kb);
        unit = "kb";
    }

    // TODO: formatting depending on status mode
    char buf[64];
    snprintf(buf, sizeof(buf), "RAM: %5.2f%s", scaled_used, unit);

    _formatted_status = buf;

    return _formatted_status;
}

Ram::Ram() {
    _free_kb = _total_kb = _used_kb = 0;
}

void Ram::_get_ram() {
    string line;

    while (getline(_file_ram, line)) {
        if (line.compare(0, 9, "MemTotal:") == 0)
            _total_kb = stoul(line.substr(10));
        else if (line.compare(0, 13, "MemAvailable:") == 0) {
            _free_kb = stoul(line.substr(14));
            break;
        }
        _file_ram.ignore(1024, '\n');
    }

    _file_ram.clear();
    _file_ram.seekg(0, ios::beg);

    _used_kb = _total_kb - _free_kb;
}
