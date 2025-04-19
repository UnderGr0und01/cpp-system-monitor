#pragma once
#include "data_collector.hpp"
#include <fstream>

class CpuCollector : public DataCollector {
public:
    CpuCollector() = default;
    void collect() override;
    const DataPoint& getData() const override { return data_; }

private:
    DataPoint data_;
    struct CpuStats {
        long user, nice, system, idle, iowait, irq, softirq;
        long total() const { return user + nice + system + idle + iowait + irq + softirq; }
    };
    CpuStats read_cpu_stats();
    double calculate_usage(const CpuStats& prev, const CpuStats& curr);
};