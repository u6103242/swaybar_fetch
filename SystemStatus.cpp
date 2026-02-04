//
// Created by adri on 2/4/26.
//

#include "SystemStatus.h"

void SystemStatus::tick() {
    _updated = false;
    if (_nupd % _TUU_LANG == 0) _get_lang();
    //if (_nupd % _TUU_NET == 0) _get_net();
    if (_nupd % _TUU_CPU == 0) _get_cpu();
    if (_nupd % _TUU_RAM == 0) _get_ram();
    if (_nupd % _TUU_AUD == 0) _get_aud();
    if (_nupd % _TUU_POW == 0) _get_pow();
    if (_nupd % _TUU_TIME == 0) _get_time();

    _nupd++;
};

void SystemStatus::force_update() {
    _get_lang();
    _get_net();
    _get_cpu();
    _get_ram();
    _get_aud();
    _get_pow();
    _get_time();
}

string SystemStatus::get_status() const {
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

////////// LANG /////////
void SystemStatus::_get_lang() {
    _updated = true;
}
/////////////////////////