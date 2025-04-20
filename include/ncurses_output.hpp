#pragma once
#include "collectors/data_collector.hpp"
#include <ncurses.h>
#include <string>
#include <vector>
#include <deque>

class Output {
public:
    virtual ~Output() = default;
    virtual void display(const DataPoint& data, const std::string& interface = "wlp0s20f3") = 0;
};

class NcursesOutput : public Output {
public:
    NcursesOutput();
    ~NcursesOutput();
    void display(const DataPoint& data, const std::string& interface) override;
    bool handle_input(std::string& interface, const std::vector<std::string>& interfaces);

private:
    void init_ncurses();
    void draw_progress_bar(int y, int x, double percentage, int width);
    void draw_network_graph(int y, int x, const std::deque<double>& history);
    std::deque<double> rx_history_;
    std::deque<double> tx_history_;
    std::deque<double> total_rx_history_;
    std::deque<double> total_tx_history_;
    static constexpr size_t HISTORY_SIZE = 4;
};