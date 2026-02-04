//
// Created by adri on 2/4/26.
//

#include <iomanip>

#include "Time.h"

string Time::get_status(const STATUS_MODE mode) {
        _get_time(mode);
        return _formatted_status;
}

void Time::_get_time(const STATUS_MODE mode) {
        _time_t = time(nullptr);
        _time_tm = *localtime(&_time_t);

        ostringstream oss;
        if (mode == STATUS_MODE::DEFAULT or mode == STATUS_MODE::MINIMAL)
                oss << put_time(&_time_tm, "%d/%m/%Y %H-%M");
        else
                oss << put_time(&_time_tm, "%d/%m/%Y %H-%M-%S");

        _formatted_status = oss.str();
}


