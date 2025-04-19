#include "cpu_collector.hpp"
#include "data_aggregator.hpp"
#include "console_output.hpp"
#include "memory_collector.hpp"
#include "network_collector.hpp"
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    std::string interface = (argc > 1) ? argv[1] : "wlp0s20f3";
    signal(SIGINT, signal_handler);

    auto aggregator = std::make_unique<DataAggregator>();
    auto output = std::make_unique<ConsoleOutput>();

    aggregator->addCollector(std::make_unique<CpuCollector>());
    aggregator->addCollector(std::make_unique<MemoryCollector>());
    aggregator->addCollector(std::make_unique<NetworkCollector>("wlp0s20f3"));

    aggregator->subscribe([&](const DataPoint& data) {
        output->display(data);
    });

    aggregator->start();

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    aggregator->stop();

    return 0;
}