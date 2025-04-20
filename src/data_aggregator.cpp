#include "data_aggregator.hpp"
#include "collectors/cpu_collector.hpp"
#include "collectors/memory_collector.hpp"
#include "collectors/network_collector.hpp"

void DataAggregator::addCollector(std::unique_ptr<DataCollector> collector) {
    collectors_.push_back(std::move(collector));
}

void DataAggregator::start() {
    Config config = load_config("../config.ini");
    running_ = true;
    for (auto& collector : collectors_) {
        threads_.emplace_back([&] {
            while (running_) {
                collector->collect();
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    if (auto* cpu = dynamic_cast<CpuCollector*>(collector.get())) {
                        cached_data_.cpu_usage = collector->getData().cpu_usage;
                    } else if (auto* mem = dynamic_cast<MemoryCollector*>(collector.get())) {
                        cached_data_.mem_usage = collector->getData().mem_usage;
                    } else if (auto* net = dynamic_cast<NetworkCollector*>(collector.get())) {
                        cached_data_.net_rx = collector->getData().net_rx;
                        cached_data_.net_tx = collector->getData().net_tx;
                        cached_data_.net_total_rx = collector->getData().net_total_rx;
                        cached_data_.net_total_tx = collector->getData().net_total_tx;
                    }
                    for (const auto& subscriber : subscribers_) {
                        subscriber(cached_data_);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(config.update_interval_ms));
            }
        });
    }
}

void DataAggregator::stop() {
    running_ = false;
    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void DataAggregator::subscribe(std::function<void(const DataPoint&)> callback) {
    subscribers_.push_back(std::move(callback));
}