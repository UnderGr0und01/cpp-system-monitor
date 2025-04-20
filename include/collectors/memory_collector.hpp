#pragma once
#include "data_collector.hpp"
#include <fstream>

class MemoryCollector : public DataCollector {
public:
    MemoryCollector() = default;
    void collect() override;
    const DataPoint& getData() const override { return data_; }

private:
    DataPoint data_;
    struct MemStats {
        long memTotal, memAvailable;
    };
    MemStats read_memory_stats();
    double calculate_usage(const MemStats& stats);
};