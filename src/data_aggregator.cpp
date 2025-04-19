#include "data_aggregator.hpp"

void DataAggregator::addCollector(std::unique_ptr<DataCollector> collector) {
    collectors_.push_back(std::move(collector));
}

void DataAggregator::start() {
    running_ = true;
    for (auto& collector : collectors_) {
        threads_.emplace_back([&] {
            while (running_) {
                collector->collect();
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    cached_data_ = collector->getData(); // Пока только CPU
                    for (const auto& subscriber : subscribers_) {
                        subscriber(cached_data_);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }
}

void DataAggregator::stop() {
    running_ = false;
    for (auto& thread : threads_) {
        if (thread.joinable()) thread.join();
    }
}

void DataAggregator::subscribe(std::function<void(const DataPoint&)> callback) {
    subscribers_.push_back(std::move(callback));
}