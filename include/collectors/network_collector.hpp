#pragma once
#include "data_collector.hpp"
#include <fstream>
#include <string>

class NetworkCollector : public DataCollector {
public:
    NetworkCollector(const std::string& interface = "eth0");
    void collect() override;
    const DataPoint& getData() const override { return data_; }

private:
    DataPoint data_;
    std::string interface_;
    struct NetStats {
        long rx_bytes, tx_bytes;
    };
    void read_net_stats(NetStats& specific, NetStats& total);
    void calculate_speeds(const NetStats& prev_specific, const NetStats& curr_specific,
                         const NetStats& prev_total, const NetStats& curr_total, double interval);
};