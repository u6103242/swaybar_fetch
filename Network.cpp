//
// Created by adri on 2/4/26.
//

#include "Network.h"

#include <iostream>
#include <algorithm>

Network::Network() {
    // Get interfaces
    for (const auto & entry : std::filesystem::directory_iterator(_interfaces_dir)) {
        net_interface aux_interface;

        aux_interface.path = entry.path().string();
        aux_interface.interface_name = aux_interface.path.erase(0, _interfaces_dir.length());
        if (aux_interface.interface_name == "lo") continue;

        aux_interface.type = std::filesystem::directory_entry(aux_interface.path + "/wireless").exists() ? WIRELESS : WIRED;

        std::ifstream operstate(aux_interface.path + "/operstate");
        std::string operstate_val;
        operstate >> operstate_val;
        aux_interface.oper_up = operstate_val == "up";
        operstate.close();
        if (not aux_interface.oper_up) continue;

        std::ifstream carrier(aux_interface.path + "/carrier");
        std::string carrier_val;
        operstate >> carrier_val;
        aux_interface.oper_up = carrier_val == "1";
        operstate.close();
        if (not aux_interface.oper_up) continue;

        interfaces.push_back(aux_interface);
    }

    // Get interfaces' state
    _file_interfaces_state.ignore(1024, '\n'); // header 1
    _file_interfaces_state.ignore(1024, '\n'); // header 2
    _file_interfaces_state.ignore(1024, '\n'); // lo

    net_interface_state state;
    std::string interface_name;
    while (_file_interfaces_state >> interface_name) {
        interface_name = interface_name.erase(_interfaces_dir.length() - 1, 1);
        _file_interfaces_state
            >> state.rx_bytes >> state.rx_packets >> state.rx_errs >> state.rx_fifo
            >> state.rx_frame >> state.rx_compressed >> state.rx_multicast
            >> state.tx_bytes >> state.tx_packets >> state.tx_errs >> state.tx_fifo
            >> state.tx_frame >> state.tx_compressed >> state.tx_multicast;

        auto it = std::find_if( interfaces.begin(), interfaces.end(),
            [&](const net_interface& net_interface) {
                return net_interface.interface_name == interface_name;
            }
        );
        it->state = state;
    }


}
