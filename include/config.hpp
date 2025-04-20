#pragma once
#include <string>

struct Config {
    std::string interface = "wlp0s20f3";
    int update_interval_ms = 1000;
};

Config load_config(const std::string& filename);