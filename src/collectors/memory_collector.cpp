#include "collectors/memory_collector.hpp"
#include <stdexcept>
#include <iostream>

MemoryCollector::MemStats MemoryCollector::read_memory_stats() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/meminfo");
    }
    MemStats stats;
    stats.memTotal = 0;
    stats.memAvailable = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("MemTotal") == 0) {
            sscanf(line.c_str(), "MemTotal: %ld kB", &stats.memTotal);
        } else if (line.find("MemAvailable") == 0) {
            sscanf(line.c_str(), "MemAvailable: %ld kB", &stats.memAvailable);
        }
    }
    return stats;
}

double MemoryCollector::calculate_usage(const MemStats& stats) {
    if (stats.memTotal == 0) {
        return 0.0;
    }
    return 100.0 * (stats.memTotal - stats.memAvailable) / stats.memTotal;
}

void MemoryCollector::collect() {
    try {
        auto stats = read_memory_stats();
        data_.mem_usage = calculate_usage(stats);
    } catch (const std::exception& e) {
        data_.mem_usage = 0.0;
        std::cerr << "Error in MemoryCollector: " << e.what() << std::endl;
    }
}