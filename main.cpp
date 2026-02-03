#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>

const std::string sep = " | ";

// Ticks per second
constexpr unsigned short int TPS = 5;
// Milliseconds per tick
constexpr unsigned long MSPT = 1000.0 / TPS;

// Milliseconds per update
constexpr unsigned short int _MSPU_LANG = 5000; // 15s
constexpr unsigned short int _MSPU_NET = 10000; // 10s
constexpr unsigned short int _MSPU_CPU = 500;	// 0.5s
constexpr unsigned short int _MSPU_RAM = 1000;	// 0.5s 
constexpr unsigned short int _MSPU_AUD = 500;   // 0.5s
constexpr unsigned short int _MSPU_POW = 500;   // 0.5s
constexpr unsigned short int _MSPU_TIME = 1000; // 1s

// Ticks until update
constexpr unsigned short int _TUU_LANG = _MSPU_LANG / MSPT;
constexpr unsigned short int _TUU_NET = _MSPU_NET / MSPT;
constexpr unsigned short int _TUU_CPU = _MSPU_CPU / MSPT;
constexpr unsigned short int _TUU_RAM = _MSPU_RAM / MSPT;
constexpr unsigned short int _TUU_AUD = _MSPU_AUD / MSPT;
constexpr unsigned short int _TUU_POW = _MSPU_POW / MSPT;
constexpr unsigned short int _TUU_TIME = _MSPU_TIME / MSPT;

struct SystemStatus {
public:
	// Information mode
	enum MODE {
		DEFAULT,			// Displays general information
		HARDWARE_ADVANCED,	// Displays advanced hardware-only information
		MINIMAL,			// Displays date and time
		SERVER				// Displays information from a mounted server
	};

	void tick() {
		_updated = false;
		if (_nupd % _TUU_LANG == 0) _get_lang();
		if (_nupd % _TUU_NET == 0) _get_net();
		if (_nupd % _TUU_CPU == 0) _get_cpu();
		if (_nupd % _TUU_RAM == 0) _get_ram();
		if (_nupd % _TUU_AUD == 0) _get_aud();
		if (_nupd % _TUU_POW == 0) _get_pow();
		if (_nupd % _TUU_TIME == 0) _get_time();

		_nupd++;
	};

	void force_update() {
		_get_lang();
		_get_net();
		_get_cpu();
		_get_ram();
		_get_aud();
		_get_pow();
		_get_time();
	}

	std::string get_status() const {
		if (not _updated) return _cache_out;

		// Reload output
		return
			_cache_lang + sep +
			_cache_net + sep +
			_cache_cpu + sep +
			_cache_ram + sep +
			_cache_aud + sep +
			_cache_pow + sep +
			_cache_time + sep;
	}

private:
	// Current mode
	MODE _mode = DEFAULT;

	// Number of updates done
	unsigned short int _nupd = 0; 
	
	// Tracks if any value has been updated
	bool _updated = true;

	// Cached values
	std::string _cache_lang;
	std::string _cache_net;
	std::string _cache_cpu = "CPU: 0%";
	std::string _cache_ram = "RAM: 0Mb";
	std::string _cache_aud;
	std::string _cache_pow;
	std::string _cache_time;

	std::string _cache_out;
	
	// File streams
	std::ifstream _file_cpu = std::ifstream("/proc/stat");
	std::ifstream _file_ram = std::ifstream("/proc/meminfo");

	////////// LANG /////////
	void _get_lang() {
		_updated = true;
	}
	/////////////////////////

	////////// NET //////////
	void _get_net() {
		_updated = true;
	}
	/////////////////////////

	////////// CPU //////////
	struct _cpu_state {
		long long user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0;
		long long get_state() const { return user + nice + system + idle + iowait + irq + softirq + steal; }
	};
	_cpu_state _cpu_prev_state ;
	void _get_cpu() {
		_updated = true;

		std::string label;
		_cpu_state _cpu_current_state;

		_file_cpu >> label >> _cpu_current_state.user >> _cpu_current_state.nice >> _cpu_current_state.system >> _cpu_current_state.idle >> _cpu_current_state.iowait >> _cpu_current_state.irq >> _cpu_current_state.softirq >> _cpu_current_state.steal;
		_file_cpu.clear();
		_file_cpu.seekg(0, std::ios::beg);

		const long long total_dif = _cpu_current_state.get_state() - _cpu_prev_state.get_state();
		const long long idle_dif = _cpu_current_state.idle - _cpu_prev_state.idle;

		_cpu_prev_state = _cpu_current_state;

		const double perc = 100.0 * (1.0 - static_cast<double>(idle_dif) / total_dif);
		char buf[32];
		std::snprintf(buf, sizeof(buf), "CPU: %6.2f%%", perc);
		_cache_cpu = buf;
	}
	/////////////////////////

	////////// RAM //////////
	void _get_ram() {
		_updated = true;
		
		std::string line;
		unsigned long free_kb = 0, total_kb = 0, val;

		while (std::getline(_file_ram, line)) {
			if (line.compare(0, 9, "MemTotal:") == 0) 
				total_kb = std::stoul(line.substr(10));
			else if (line.compare(0, 13, "MemAvailable:") == 0) {
				free_kb = std::stoul(line.substr(14));
				break;
			} 
			_file_ram.ignore(1024, '\n');
		}

		_file_ram.clear();
		_file_ram.seekg(0, std::ios::beg);

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
		std::snprintf(buf, sizeof(buf), "RAM: %5.2f%s", scaled_used, unit);

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

		_time_t = std::time(nullptr);
		_time_tm = *std::localtime(&_time_t);

		std::ostringstream oss;
		oss << std::put_time(&_time_tm, "%d/%m/%Y %H-%M-%S");
		_cache_time = oss.str();
	}
	/////////////////////////
};

[[noreturn]] int main(int argc, char const *argv[])
{
	SystemStatus sys;
	auto next_tick = std::chrono::steady_clock::now();

	while (true) {
		next_tick += std::chrono::milliseconds(MSPT);

		sys.tick();
		std::cout << sys.get_status() << std::endl;

		std::this_thread::sleep_until(next_tick);
	}
}