//
// Created by adri on 2/4/26.
//

#include "Ram.h"

#include "Debug.h"

std::string Ram::get_status(const STATUS_MODE mode) {
    _fetch_data(mode);
    _interpret_data(mode);

    double ram_used_formatted;
    const char* ram_units;

    if (_ram_used_kb >= 1048576) {
        ram_used_formatted = _ram_used_kb / 1024.0 / 1024.0;
        ram_units = "Gb";
    } else if (_ram_used_kb >= 1024) {
        ram_used_formatted = _ram_used_kb / 1024.0;
        ram_units = "Mb";
    } else {
        ram_used_formatted = static_cast<double>(_ram_used_kb);
        ram_units = "kb";
    }

    char buf[64];

    // This can be turned into a switch case if more formatting options need to be added.
    if (mode == STATUS_MODE::HARDWARE_ADVANCED) {
        const char* swap_units;
        double swap_used_formatted;
        if (_ram_used_kb >= 1048576) {
            swap_used_formatted = _ram_used_kb / 1024.0 / 1024.0;
            swap_units = "Gb";
        } else if (_ram_used_kb >= 1024) {
            swap_used_formatted = _ram_used_kb / 1024.0;
            swap_units = "Mb";
        } else {
            swap_used_formatted = static_cast<double>(_ram_used_kb);
            swap_units = "kb";
        }

        snprintf(buf, sizeof(buf), "RAM: %5.2f%s SWAP: %5.2f%s", ram_used_formatted, ram_units, swap_used_formatted, swap_units);
    } else {
        snprintf(buf, sizeof(buf), "RAM: %5.2f%s", ram_used_formatted, ram_units);
    }

    return buf;
}

void Ram::_fetch_data(const STATUS_MODE mode) {
    std::string line;

    DBG(" --- Getting ram ---");
    while (getline(_file_mem, line)) {
        if (line.compare(0, 9, "MemTotal:") == 0) {
            DBG(" - " << line);
            _ram_total_kb = stoul(line.substr(10));
        } else if (line.compare(0, 13, "MemAvailable:") == 0) {
            DBG(" - " << line);
            _ram_free_kb = stoul(line.substr(14));
            break;
        }

        _file_mem.ignore(1024, '\n');
    }

    // Fetch swap only if needed
    if (mode == STATUS_MODE::HARDWARE_ADVANCED) {
        DBG(" --- Getting swap ---");
        while (getline(_file_mem, line)) {
            if (line.compare(0, 10, "SwapTotal:") == 0) {
                DBG(" - " << line);
                _swap_total_kb = stoul(line.substr(11));
            } else if (line.compare(0, 9, "SwapFree:") == 0) {
                DBG(" - " << line);
                _swap_free_kb = stoul(line.substr(10));
                break;
            }

            _file_mem.ignore(1024, '\n');
        }
    }

    _file_mem.clear();
    _file_mem.seekg(0, std::ios::beg);
}

void Ram::_interpret_data(const STATUS_MODE mode) {
    _ram_used_kb = _ram_total_kb - _ram_free_kb;
    if (mode == STATUS_MODE::HARDWARE_ADVANCED)
        _swap_used_kb = _swap_total_kb - _swap_free_kb;
}