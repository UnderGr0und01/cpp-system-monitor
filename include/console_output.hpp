#pragma once
#include "data_collector.hpp"
#include <iostream>

class ConsoleOutput {
public:
    void display(const DataPoint& data);
};