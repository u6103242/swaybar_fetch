//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_TIME_H
#define SWAYBAR_FETCH_TIME_H

#include <string>
#include <ctime>

#include "Constants.h"

class Time {
public:
    std::string get_status(STATUS_MODE mode);

private:
    time_t _time_t{};
    tm _time_tm{};

    std::string _formatted_status;

    void _get_time(STATUS_MODE mode);
};


#endif //SWAYBAR_FETCH_TIME_H