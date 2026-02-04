//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_SYSTEMSTATUS_H
#define SWAYBAR_FETCH_SYSTEMSTATUS_H

#include <string>

#include "Constants.h"
#include "Network.h"
#include "Cpu.h"
#include "Ram.h"
#include "Time.h"

// TODO: (IMPORTANT) Should divide all services into three main functions:
//  -  fetch data
//  -  interpret data
//  -  format and return data

class SystemStatus {
public:
	/// System status constructor
	explicit SystemStatus(STATUS_MODE mode);

	/// Ticks, if a service needs updating, it fetches the necessary information and updates the cache
	void tick();

	/// Forces and update of all services
	void force_update();

	/// Returns the system status
	std::string get_status() const;
private:
	/// Current mode
	STATUS_MODE _mode = STATUS_MODE::DEFAULT;

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
	std::string _cache_lang;
	std::string _cache_net;
	std::string _cache_cpu = "CPU:   0%";
	std::string _cache_ram = "RAM:   0Mb";
	std::string _cache_aud;
	std::string _cache_pow;
	std::string _cache_time;

	std::string _cache_out;

	void _get_lang();
	void _get_net();
	void _get_cpu();
	void _get_ram();
	void _get_aud();
	void _get_pow();
	void _get_time();
};


#endif //SWAYBAR_FETCH_SYSTEMSTATUS_H