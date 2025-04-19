#pragma once
#include <chrono>

struct DataPoint
{
    double cpu_usage = 0.0;
};


class DataCollector
{
public:
    virtual ~DataCollector() = default;
    virtual void collect() = 0;
    virtual const DataPoint& getData() const = 0;
};
