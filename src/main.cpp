#include "collectors/cpu_collector.hpp"
#include "data_aggregator.hpp"
#include "console_output.hpp"
#include "collectors/memory_collector.hpp"
#include "collectors/network_collector.hpp"
#include "config.hpp"
#include "ncurses_output.hpp"
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    // std::string interface = (argc > 1) ? argv[1] : "wlp0s20f3";


    Config config = load_config("../config.ini");
    auto aggregator = std::make_unique<DataAggregator>();
    // auto output = std::make_unique<ConsoleOutput>();
    auto output = std::make_unique<NcursesOutput>();
    std::string interface = config.interface;


    aggregator->addCollector(std::make_unique<CpuCollector>());
    aggregator->addCollector(std::make_unique<MemoryCollector>());
    aggregator->addCollector(std::make_unique<NetworkCollector>("wlp0s20f3"));

    aggregator->subscribe([&](const DataPoint& data) {
        output->display(data, interface);
    });

    aggregator->start();

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(config.update_interval_ms / 50));
    }

    aggregator->stop();

    return 0;
}