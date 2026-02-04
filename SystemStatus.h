//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_SYSTEMSTATUS_H
#define SWAYBAR_FETCH_SYSTEMSTATUS_H

#include <iostream>
#include <string>

#include "Constants.h"
#include "Network.h"
#include "Cpu.h"
#include "Ram.h"
#include "Time.h"

using namespace std;

class SystemStatus {
public:
	// Status mode
	enum MODE {
		DEFAULT,			// Displays general information
		HARDWARE_ADVANCED,	// Displays advanced hardware-only information
		MINIMAL,			// Displays date and time
		SERVER,				// Displays information from a mounted server
		DISABLED			// Completely disables the logic
	};

	/// Ticks, if a service needs updating, it fetches the necessary information and updates the cache
	void tick();
	void force_update();

	string get_status() const;
private:
	// Current mode
	MODE _mode = DEFAULT;

	// Services
	Network _sys_net;
	Cpu _sys_cpu;
	Ram _sys_ram;
	Time _sys_time;

	// Number of updates(ticks) passed
	unsigned short int _nupd = 0;

	// Tracks if any value has been updated
	bool _updated = true;

	// Cached values
	string _cache_lang;
	string _cache_net;
	string _cache_cpu = "CPU:   0%";
	string _cache_ram = "RAM:   0Mb";
	string _cache_aud;
	string _cache_pow;
	string _cache_time;

	string _cache_out;


	void _get_lang();

	_cpu_state _cpu_prev_state;
	void _get_cpu() {
		_updated = true;

		string label;
		_cpu_state _cpu_current_state;

		_file_cpu >> label >> _cpu_current_state.user >> _cpu_current_state.nice >> _cpu_current_state.system >> _cpu_current_state.idle >> _cpu_current_state.iowait >> _cpu_current_state.irq >> _cpu_current_state.softirq >> _cpu_current_state.steal;
		_file_cpu.clear();
		_file_cpu.seekg(0, ios::beg);

		const long long total_dif = _cpu_current_state.get_state() - _cpu_prev_state.get_state();
		const long long idle_dif = _cpu_current_state.idle - _cpu_prev_state.idle;

		_cpu_prev_state = _cpu_current_state;

		const double perc = 100.0 * (1.0 - static_cast<double>(idle_dif) / total_dif);
		char buf[32];
		snprintf(buf, sizeof(buf), "CPU: %6.2f%%", perc);
		_cache_cpu = buf;
	}

	////////// RAM //////////
	void _get_ram() {
		_updated = true;

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

		_cache_ram = buf;
	}
	/////////////////////////

	////////// AUD //////////
	void _get_aud() {
		_updated = true;

	}
	/////////////////////////

	////////// POW //////////
	void _get_pow() {
		_updated = true;

	}
	/////////////////////////

	////////// TIME /////////
	time_t _time_t{};
	tm _time_tm{};
	void _get_time() {
		_updated = true;

		_time_t = time(nullptr);
		_time_tm = *localtime(&_time_t);

		ostringstream oss;
		oss << put_time(&_time_tm, "%d/%m/%Y %H-%M-%S");
		_cache_time = oss.str();
	}
	/////////////////////////
};


#endif //SWAYBAR_FETCH_SYSTEMSTATUS_H