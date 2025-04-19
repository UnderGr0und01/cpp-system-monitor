#include "console_output.hpp"

void ConsoleOutput::display(const DataPoint& data) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "CPU Usage: " << data.cpu_usage << "%" << std::endl;
    std::cout << "Memory Usage: " << data.mem_usage << "%" << std::endl;
    std::cout << "Network: RX " << data.net_rx << " KB/s, TX " << data.net_tx << " KB/s" << std::endl;
    std::cout << "Network (Total): RX " << data.net_total_rx << " KB/s, TX " << data.net_total_tx << " KB/s" << std::endl;
}