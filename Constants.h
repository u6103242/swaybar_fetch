//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_CONSTANTS_H
#define SWAYBAR_FETCH_CONSTANTS_H

#include <string>

const std::string sep = " | ";

/// Ticks per second
constexpr unsigned short int TPS = 5;
/// Milliseconds per tick
constexpr unsigned long MSPT = 1000.0 / TPS;

/// Milliseconds per update
constexpr unsigned int _MSPU_LANG = 5000; // 15s
constexpr unsigned int _MSPU_NET = 10000; // 10s
constexpr unsigned int _MSPU_DISK = 300000; // 5min
constexpr unsigned int _MSPU_CPU = 500; // 0.5s
constexpr unsigned int _MSPU_RAM = 1000; // 0.5s
constexpr unsigned int _MSPU_AUD = 500; // 0.5s
constexpr unsigned int _MSPU_POW = 500; // 0.5s
constexpr unsigned int _MSPU_TIME = 1000; // 1s

/// Ticks until update
constexpr unsigned short int ms_to_ticks(const unsigned long ms) {
    return (ms / MSPT < 1 ? 1 : ms / MSPT);
}

constexpr unsigned short int _TUU_LANG = ms_to_ticks(_MSPU_LANG);
constexpr unsigned short int _TUU_NET = ms_to_ticks(_MSPU_NET);
constexpr unsigned short int _TUU_DISK = ms_to_ticks(_MSPU_DISK);
constexpr unsigned short int _TUU_CPU = ms_to_ticks(_MSPU_CPU);
constexpr unsigned short int _TUU_RAM = ms_to_ticks(_MSPU_RAM);
constexpr unsigned short int _TUU_AUD = ms_to_ticks(_MSPU_AUD);
constexpr unsigned short int _TUU_POW = ms_to_ticks(_MSPU_POW);
constexpr unsigned short int _TUU_TIME = ms_to_ticks(_MSPU_TIME);

/// Status modes
enum class STATUS_MODE {
    DEFAULT,			// Displays general information
    HARDWARE_ADVANCED,	// Displays advanced hardware-only information
    MINIMAL,			// Displays date and time
    SERVER,				// Displays information from a mounted server
    DISABLED			// Completely disables the logic
};
constexpr STATUS_MODE DEFAULT_MODE = STATUS_MODE::DEFAULT;

#endif //SWAYBAR_FETCH_CONSTANTS_H