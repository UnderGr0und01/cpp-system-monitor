#pragma once
#include <chrono>

struct DataPoint
{
    double cpu_usage = 0.0;
    double mem_usage = 0.0;
    double net_rx = 0.0;
    double net_tx = 0.0;
    double net_total_rx = 0.0;
    double net_total_tx = 0.0;
};


class DataCollector
{
public:
    virtual ~DataCollector() = default;
    virtual void collect() = 0;
    virtual const DataPoint& getData() const = 0;
};
