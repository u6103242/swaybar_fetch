//
// Created by adri on 2/4/26.
//

#include "Cpu.h"

#include <iomanip>

#include "Debug.h"

std::string Cpu::get_status(const STATUS_MODE mode) {
    _fetch_data(mode);
    _interpret_data(mode);

    std::ostringstream oss;

    // Aggregate CPU first
    oss << "CPU: " << std::fixed << std::setprecision(2) << _cpu_delta.perc << "%";

    if (mode == STATUS_MODE::HARDWARE_ADVANCED) {
        for (size_t core = 0; core < _per_core_delta.size(); core++) {
            oss << " [" << core << ": "
                << std::fixed << std::setprecision(2)
                << _per_core_delta[core].perc << "%]";
        }
    }

    return oss.str();
}

// TODO: Optimize CPU parsing in the future:
//   - Read entire /proc/stat into a static buffer
//   - Parse numbers directly with pointers or std::from_chars
//   - Avoid operator>> / iostream overhead
//   - Reuse buffer every tick to eliminate allocations
//   - Stops at non-"cpu*" lines (robust against format changes)

void Cpu::_fetch_data(const STATUS_MODE mode) {
    std::string label;

    DBG(" --- CPU --- ");
    _file_cpu >> label >> _cpu_curr_state.user >> _cpu_curr_state.nice >> _cpu_curr_state.system >> _cpu_curr_state.idle
        >> _cpu_curr_state.iowait >> _cpu_curr_state.irq >> _cpu_curr_state.softirq >> _cpu_curr_state.steal;
    DBG(" - " << label << _cpu_curr_state.user << _cpu_curr_state.nice << _cpu_curr_state.system << _cpu_curr_state.idle
        << _cpu_curr_state.iowait << _cpu_curr_state.irq << _cpu_curr_state.softirq << _cpu_curr_state.steal);

    if (mode == STATUS_MODE::HARDWARE_ADVANCED) {
        size_t n_core = 0;
        _file_cpu >> label;
        while (label.compare(0, 3, "cpu") == 0) {
            _cpu_state& core = _per_core_curr_state[n_core];
            _file_cpu >> core.user >> core.nice >> core.system >> core.idle >> core.iowait >> core.irq >> core.softirq >> core.steal;
            n_core++;

            _file_cpu >> label;
        }
    }

    _file_cpu.clear();
    _file_cpu.seekg(0, std::ios::beg);
}

void Cpu::_interpret_data(const STATUS_MODE mode) {
    // Delta calculation
    _cpu_delta.total_dif = _cpu_curr_state.get_state() - _cpu_prev_state.get_state();
    _cpu_delta.idle_dif = _cpu_curr_state.idle - _cpu_prev_state.idle;
    _cpu_delta.perc = 100.0 * (1.0 - static_cast<double>(_cpu_delta.idle_dif) / _cpu_delta.total_dif);

    _cpu_prev_state = _cpu_curr_state;

    if (mode == STATUS_MODE::HARDWARE_ADVANCED) {
        _per_core_prev_state = _per_core_curr_state;

        for (size_t core = 0; core < _per_core_curr_state.size(); core++) {
            _per_core_delta[core].total_dif = _per_core_curr_state[core].get_state() - _per_core_prev_state[core].get_state();
            _per_core_delta[core].idle_dif = _per_core_curr_state[core].idle - _per_core_prev_state[core].idle;
            _per_core_delta[core].perc = 100.0 * (1.0 - static_cast<double>(_per_core_delta[core].idle_dif) / _per_core_delta[core].total_dif);

            _cpu_prev_state = _cpu_curr_state;
        }
    }
}
