#include "ncurses_output.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <locale.h>

NcursesOutput::NcursesOutput() {
    setlocale(LC_ALL, "");
    init_ncurses();
}

NcursesOutput::~NcursesOutput() {
    endwin();
}

void NcursesOutput::init_ncurses() {
    initscr();
    if (!has_colors()) {
        endwin();
        throw std::runtime_error("Terminal does not support colors");
    }
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    curs_set(0);
    timeout(50);
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_YELLOW, -1);
}

void NcursesOutput::draw_progress_bar(int y, int x, double percentage, int width) {
    int filled = static_cast<int>(percentage * width / 100.0);
    attron(COLOR_PAIR(1));
    mvprintw(y, x, "[");
    for (int i = 0; i < width; ++i) {
        mvprintw(y, x + 1 + i, i < filled ? "#" : " ");
    }
    mvprintw(y, x + width + 1, "]");
    attroff(COLOR_PAIR(1));
}

void NcursesOutput::draw_network_graph(int y, int x, const std::deque<double>& history) {
    const char* graph_chars = "-=+#";
    attron(COLOR_PAIR(3));
    if (history.empty() || history.size() < 4) {
        mvprintw(y, x, "----");
    } else {
        double max_val = *std::max_element(history.begin(), history.end());
        if (max_val == 0) max_val = 1.0;
        for (size_t i = history.size() - 4; i < history.size(); ++i) {
            int level = static_cast<int>((history[i] / max_val) * 3);
            mvprintw(y, x + (i - (history.size() - 4)), "%c", graph_chars[level]);
        }
    }
    attroff(COLOR_PAIR(3));
}

void NcursesOutput::display(const DataPoint& data, const std::string& interface) {
    clear();
    attron(COLOR_PAIR(2));
    mvprintw(0, 0, "+ System Monitor ----------------------+");
    for (int i = 1; i < 7; ++i) {
        mvprintw(i, 0, "|                                     |");
    }
    mvprintw(7, 0, "+-------------------------------------+");
    attroff(COLOR_PAIR(2));

    rx_history_.push_back(data.net_rx);
    tx_history_.push_back(data.net_tx);
    total_rx_history_.push_back(data.net_total_rx);
    total_tx_history_.push_back(data.net_total_tx);
    if (rx_history_.size() > HISTORY_SIZE) rx_history_.pop_front();
    if (tx_history_.size() > HISTORY_SIZE) tx_history_.pop_front();
    if (total_rx_history_.size() > HISTORY_SIZE) total_rx_history_.pop_front();
    if (total_tx_history_.size() > HISTORY_SIZE) total_tx_history_.pop_front();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << std::setw(5) << std::right << data.cpu_usage;
    std::string cpu_str = ss.str();
    ss.str("");
    ss << std::setw(5) << std::right << data.mem_usage;
    std::string mem_str = ss.str();
    ss.str("");
    ss << std::setw(5) << std::right << data.net_rx;
    std::string rx_str = ss.str();
    ss.str("");
    ss << std::setw(5) << std::right << data.net_tx;
    std::string tx_str = ss.str();
    ss.str("");
    ss << std::setw(5) << std::right << data.net_total_rx;
    std::string total_rx_str = ss.str();
    ss.str("");
    ss << std::setw(5) << std::right << data.net_total_tx;
    std::string total_tx_str = ss.str();

    // CPU Usage
    std::string cpu_line = "CPU Usage:    [" + std::string(10, ' ') + "] " + cpu_str + "%";
    cpu_line.resize(36, ' ');
    mvprintw(1, 1, cpu_line.c_str());
    draw_progress_bar(1, 16, data.cpu_usage, 10);

    // Memory Usage
    std::string mem_line = "Memory Usage: [" + std::string(10, ' ') + "] " + mem_str + "%";
    mem_line.resize(36, ' ');
    mvprintw(2, 1, mem_line.c_str());
    draw_progress_bar(2, 16, data.mem_usage, 10);

    // Network (interface)
    std::string net_rx_line = "Network (" + interface + "): RX " + rx_str + " KB/s ";
    net_rx_line.resize(32, ' ');
    mvprintw(3, 1, net_rx_line.c_str());
    draw_network_graph(3, 32, rx_history_);

    std::string net_tx_line = "              TX " + tx_str + " KB/s ";
    net_tx_line.resize(32, ' ');
    mvprintw(4, 1, net_tx_line.c_str());
    draw_network_graph(4, 32, tx_history_);

    // Network (Total)
    std::string total_rx_line = "Network (Total): RX " + total_rx_str + " KB/s ";
    total_rx_line.resize(32, ' ');
    mvprintw(5, 1, total_rx_line.c_str());
    draw_network_graph(5, 32, total_rx_history_);

    std::string total_tx_line = "                 TX " + total_tx_str + " KB/s ";
    total_tx_line.resize(32, ' ');
    mvprintw(6, 1, total_tx_line.c_str());
    draw_network_graph(6, 32, total_tx_history_);

    refresh();
}

bool NcursesOutput::handle_input(std::string& interface, const std::vector<std::string>& interfaces) {
    int ch = getch();
    if (ch == 'q') {
        return false;
    }
    if (ch == 'n' && !interfaces.empty()) {
        auto it = std::find(interfaces.begin(), interfaces.end(), interface);
        if (it != interfaces.end()) {
            size_t idx = std::distance(interfaces.begin(), it);
            interface = interfaces[(idx + 1) % interfaces.size()];
        }
    }
    if (ch == 'p' && !interfaces.empty()) {
        auto it = std::find(interfaces.begin(), interfaces.end(), interface);
        if (it != interfaces.end()) {
            size_t idx = std::distance(interfaces.begin(), it);
            interface = interfaces[(idx - 1 + interfaces.size()) % interfaces.size()];
        }
    }
    return true;
}