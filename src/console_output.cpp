#include "console_output.hpp"

void ConsoleOutput::display(const DataPoint& data) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "CPU Usage: " << data.cpu_usage << "%" << std::endl;
}