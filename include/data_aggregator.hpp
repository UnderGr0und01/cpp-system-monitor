#pragma once
#include "collectors/data_collector.hpp"
#include "config.hpp"
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>

class DataAggregator {
public:
    DataAggregator() = default;
    ~DataAggregator() { stop(); }
    void addCollector(std::unique_ptr<DataCollector> collector);
    void start();
    void stop();
    void subscribe(std::function<void(const DataPoint&)> callback);

private:
    std::vector<std::unique_ptr<DataCollector>> collectors_;
    std::vector<std::thread> threads_;
    std::vector<std::function<void(const DataPoint&)>> subscribers_;
    DataPoint cached_data_;
    std::mutex mutex_;
    bool running_ = false;
};