//
// Created by adri on 2/4/26.
//

#ifndef SWAYBAR_FETCH_NETWORK_H
#define SWAYBAR_FETCH_NETWORK_H

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <optional>

class Network {
public:
    enum interface_type : bool {
        WIRED,
        WIRELESS
    };

    struct wifi_connection {
        string ssid;
        string quality;
        string strength;
    };

    struct net_interface_state {
        long long rx_bytes, rx_packets, rx_errs, rx_drop, rx_fifo, rx_frame, rx_compressed, rx_multicast;
        long long tx_bytes, tx_packets, tx_errs, tx_drop, tx_fifo, tx_frame, tx_compressed, tx_multicast;
    };

    struct net_interface_rate {
        long long rx_rate, tx_rate;
    };

    struct net_interface {
        string path;
        string interface_name;
        interface_type type;
        bool oper_up;
        bool carrier;

        net_interface_state state;
        net_interface_rate rate;

        optional<wifi_state> wifi;
    };

    vector<net_interface> interfaces;

    Network();

private:
    const string _interfaces_dir = "/sys/class/net/";
    ifstream _file_interfaces_state = ifstream("/proc/net/dev");
};


#endif //SWAYBAR_FETCH_NETWORK_H