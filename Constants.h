//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_CONSTANTS_H
#define SWAYBAR_FETCH_CONSTANTS_H

#include <string>

using namespace std;

const string sep = " | ";

/// Ticks per second
constexpr unsigned short int TPS = 5;
/// Milliseconds per tick
constexpr unsigned long MSPT = 1000.0 / TPS;

/// Milliseconds per update
constexpr unsigned short int _MSPU_LANG = 5000; // 15s
constexpr unsigned short int _MSPU_NET = 10000; // 10s
constexpr unsigned short int _MSPU_CPU = 500; // 0.5s
constexpr unsigned short int _MSPU_RAM = 1000; // 0.5s
constexpr unsigned short int _MSPU_AUD = 500; // 0.5s
constexpr unsigned short int _MSPU_POW = 500; // 0.5s
constexpr unsigned short int _MSPU_TIME = 1000; // 1s

/// Ticks until update
constexpr unsigned short int _TUU_LANG = _MSPU_LANG / MSPT;
constexpr unsigned short int _TUU_NET = _MSPU_NET / MSPT;
constexpr unsigned short int _TUU_CPU = _MSPU_CPU / MSPT;
constexpr unsigned short int _TUU_RAM = _MSPU_RAM / MSPT;
constexpr unsigned short int _TUU_AUD = _MSPU_AUD / MSPT;
constexpr unsigned short int _TUU_POW = _MSPU_POW / MSPT;
constexpr unsigned short int _TUU_TIME = _MSPU_TIME / MSPT;

/// Status modes
enum class STATUS_MODE {
    DEFAULT,			// Displays general information
    HARDWARE_ADVANCED,	// Displays advanced hardware-only information
    MINIMAL,			// Displays date and time
    SERVER,				// Displays information from a mounted server
    DISABLED			// Completely disables the logic
};

#endif //SWAYBAR_FETCH_CONSTANTS_H