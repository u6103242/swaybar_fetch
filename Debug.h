//
// Created by adri on 2/5/26.
//

#ifndef SWAYBAR_FETCH_DEBUG_H
#define SWAYBAR_FETCH_DEBUG_H

#ifdef DEBUG
    #include <iostream>
    #define DBG(x) \
    do { std::cerr << __FILE__ << ":" << __LINE__ << " | " << x << '\n' << std::flush; } while (0)
#else
    #define DBG(x) do {} while (0)
#endif

#endif //SWAYBAR_FETCH_DEBUG_H