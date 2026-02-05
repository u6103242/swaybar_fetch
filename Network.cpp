//
// Created by adri on 2/4/26.
//

#include "Network.h"

#include <iostream>
#include <algorithm>

#include "Constants.h"
#include "Debug.h"

Network::Network() {
    // Get interfaces
    DBG(" --- Getting interfaces --- ");
    for (const auto & entry : std::filesystem::directory_iterator(_interfaces_dir)) {
        net_interface aux_interface;

        aux_interface.path = entry.path().string();
        aux_interface.interface_name = entry.path().filename().string();
        DBG("Found interface: " << aux_interface.interface_name << " at '" << aux_interface.path << "'");
        if (aux_interface.interface_name == "lo") {
            DBG(" - Skiped 'lo' interface");
            continue;
        }
        aux_interface.type = std::filesystem::directory_entry(aux_interface.path + "/wireless").exists() ? WIRELESS : WIRED;
        DBG(" - [" << aux_interface.interface_name << "] Type: " << (aux_interface.type == WIRELESS ? "Wireless" : "Wired"));

        std::ifstream file_operstate(aux_interface.path + "/operstate");
        if (not file_operstate.is_open())
            DBG(" - Couldn't open operstate file");
        else {
            std::string operstate_val;
            file_operstate >> operstate_val;
            aux_interface.oper_up = operstate_val == "up";
            DBG(" - [" << aux_interface.interface_name << "] Operstate: " << operstate_val);
            file_operstate.close();
            if (not aux_interface.oper_up) {
                DBG(" - Skiped interface");
                continue;
            }
        }

        std::ifstream file_carrier(aux_interface.path + "/carrier");
        if (not file_carrier.is_open())
            DBG(" - Couldn't open carrier file");
        else {
            std::string carrier_val;
            file_carrier >> carrier_val;
            aux_interface.oper_up = carrier_val == "1";
            DBG(" - [" << aux_interface.interface_name << "] Carrier: " << carrier_val);
            file_carrier.close();
            if (not aux_interface.oper_up) {
                DBG(" - Skiped interface");
                continue;
            }
        }

        interfaces.push_back(aux_interface);
        DBG(" - Added interface");
    }

    // Get states
    DBG(" --- Getting states --- ");
    _file_interfaces_state.ignore(1024, '\n'); // header 1
    _file_interfaces_state.ignore(1024, '\n'); // header 2
    _file_interfaces_state.ignore(1024, '\n'); // lo

    net_interface_state state;
    std::string interface_name;
    while (_file_interfaces_state >> interface_name) {
        interface_name = interface_name.erase(interface_name.size() - 1);
        DBG("Interface name: " << interface_name);

        _file_interfaces_state
            >> state.rx_bytes >> state.rx_packets >> state.rx_errs >> state.rx_drop
            >> state.rx_fifo >> state.rx_frame >> state.rx_compressed >> state.rx_multicast
            >> state.tx_bytes >> state.tx_packets >> state.tx_errs >> state.tx_drop
            >> state.tx_fifo >> state.tx_frame >> state.tx_compressed >> state.tx_multicast;

        auto it = std::find_if( interfaces.begin(), interfaces.end(),
            [&](const net_interface& net_interface) {
                return net_interface.interface_name == interface_name;
            }
        );

        if (it == interfaces.end()) {
            DBG(" - Skipped");
            continue;
        }

        DBG(" - Active interface's state fetched");
        it->state = state;
    }
}

std::string Network::get_status(STATUS_MODE mode) {
    return "";
}