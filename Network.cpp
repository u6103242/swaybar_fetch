//
// Created by adri on 2/4/26.
//

#include "Network.h"

#include <iostream>

Network::Network() {
    // Get interfaces
    for (const auto & entry : filesystem::directory_iterator(Network::_interfaces_dir)) {
        net_interface aux_interface;

        aux_interface.path = entry.path().string();
        aux_interface.interface_name = aux_interface.path.erase(0, _interfaces_dir.length());
        if (aux_interface.interface_name == "lo") continue;

        aux_interface.type = filesystem::directory_entry(aux_interface.path + "/wireless").exists() ? WIRELESS : WIRED;

        ifstream operstate(aux_interface.path + "/operstate");
        string val;
        operstate >> val;
        aux_interface.active = val == "up";
        operstate.close();
        if (not aux_interface.active) continue;

        interfaces.push_back(aux_interface);
    }

    // Get interfaces' state
    _file_interfaces_state.ignore(1024, '\n');
    _file_interfaces_state.ignore(1024, '\n');
    _file_interfaces_state.ignore(1024, '\n');

    net_interface_state state;
    string interface_name;
    _file_interfaces_state >> interface_name;

    /*>> _net_state.rx_bytes >> _net_state.rx_packets >> _net_state.rx_errs >> _net_state.rx_fifo
    >> _net_state.rx_frame >> _net_state.rx_compressed >> _net_state.rx_multicast
    >> _net_state.tx_bytes >> _net_state.tx_packets >> _net_state.tx_errs >> _net_state.tx_fifo
    >> _net_state.tx_frame >> _net_state.tx_compressed >> _net_state.tx_multicast;*/

}
