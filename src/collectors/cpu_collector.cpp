#include "collectors/cpu_collector.hpp"
#include <thread>
#include <stdexcept>
#include <iostream>

CpuCollector::CpuStats CpuCollector::read_cpu_stats() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/stat");
    }
    std::string line;
    std::getline(file, line);
    CpuStats stats;
    sscanf(line.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld",
           &stats.user, &stats.nice, &stats.system, &stats.idle,
           &stats.iowait, &stats.irq, &stats.softirq);
    return stats;
}

double CpuCollector::calculate_usage(const CpuStats& prev, const CpuStats& curr) {
    long prev_total = prev.total(), curr_total = curr.total();
    long prev_idle = prev.idle, curr_idle = curr.idle;
    if (curr_total == prev_total) return 0.0;
    return 100.0 * (curr_total - curr_idle - (prev_total - prev_idle)) /
                   (curr_total - prev_total);
}

void CpuCollector::collect() {
    try {
        auto prev = read_cpu_stats();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto curr = read_cpu_stats();
        data_.cpu_usage = calculate_usage(prev, curr);
    } catch (const std::exception& e) {
        data_.cpu_usage = 0.0;
        std::cerr << "Error in CpuCollector: " << e.what() << std::endl;
    }
}