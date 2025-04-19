#include "network_collector.hpp"
#include <stdexcept>
#include <thread>
#include <iostream>

NetworkCollector::NetworkCollector(const std::string& interface) : interface_(interface) {}

void NetworkCollector::read_net_stats(NetStats& specific, NetStats& total) {
    std::ifstream file("/proc/net/dev");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/net/dev");
    }
    specific = {0, 0};
    total = {0, 0};
    std::string line;
    bool found_interface = false;
    std::getline(file, line);
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.find(interface_ + ":") != std::string::npos) {
            sscanf(line.c_str(), "%*s %ld %*d %*d %*d %*d %*d %*d %*d %ld",
                   &specific.rx_bytes, &specific.tx_bytes);
            found_interface = true;
        }
        if (line.find(":") != std::string::npos) {
            long rx, tx;
            sscanf(line.c_str(), "%*s %ld %*d %*d %*d %*d %*d %*d %*d %ld", &rx, &tx);
            total.rx_bytes += rx;
            total.tx_bytes += tx;
        }
    }
    if (!found_interface) {
        throw std::runtime_error("Interface " + interface_ + " not found");
    }
}

void NetworkCollector::calculate_speeds(const NetStats& prev_specific, const NetStats& curr_specific,
                                       const NetStats& prev_total, const NetStats& curr_total, double interval) {
    if (interval <= 0) {
        data_.net_rx = 0.0;
        data_.net_tx = 0.0;
        data_.net_total_rx = 0.0;
        data_.net_total_tx = 0.0;
        return;
    }
    data_.net_rx = (curr_specific.rx_bytes - prev_specific.rx_bytes) / (interval * 1024.0); 
    data_.net_tx = (curr_specific.tx_bytes - prev_specific.tx_bytes) / (interval * 1024.0); 
    data_.net_total_rx = (curr_total.rx_bytes - prev_total.rx_bytes) / (interval * 1024.0); 
    data_.net_total_tx = (curr_total.tx_bytes - prev_total.tx_bytes) / (interval * 1024.0); 
}

void NetworkCollector::collect() {
    try {
        NetStats prev_specific, prev_total, curr_specific, curr_total;
        read_net_stats(prev_specific, prev_total);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        read_net_stats(curr_specific, curr_total);
        calculate_speeds(prev_specific, curr_specific, prev_total, curr_total, 1.0);
    } catch (const std::exception& e) {
        data_.net_rx = 0.0;
        data_.net_tx = 0.0;
        data_.net_total_rx = 0.0;
        data_.net_total_tx = 0.0;
        std::cerr << "Error in NetworkCollector: " << e.what() << std::endl;
    }
}