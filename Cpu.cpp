//
// Created by adri on 2/4/26.
//

#include "Cpu.h"

Cpu::Cpu() {
    _get_cpu();
}

std::string Cpu::get_status(const STATUS_MODE mode) {
    _get_cpu();
    return _formatted_status;
}

void Cpu::_get_cpu() {
    std::string label;
    _cpu_state _cpu_current_state;

    _file_cpu >> label >> _cpu_current_state.user >> _cpu_current_state.nice >> _cpu_current_state.system >> _cpu_current_state.idle >> _cpu_current_state.iowait >> _cpu_current_state.irq >> _cpu_current_state.softirq >> _cpu_current_state.steal;
    _file_cpu.clear();
    _file_cpu.seekg(0, std::ios::beg);

    const long long total_dif = _cpu_current_state.get_state() - _prev_state.get_state();
    const long long idle_dif = _cpu_current_state.idle - _prev_state.idle;

    _prev_state = _cpu_current_state;

    // TODO: handle status modes on get_status

    const double perc = 100.0 * (1.0 - static_cast<double>(idle_dif) / total_dif);
    char buf[32];
    snprintf(buf, sizeof(buf), "CPU: %6.2f%%", perc);

    _formatted_status = buf;
}
