#include "cpu_collector.hpp"
#include "data_aggregator.hpp"
#include "console_output.hpp"
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main() {
    // Обработка SIGINT (Ctrl+C)
    signal(SIGINT, signal_handler);

    // Создаем компоненты
    auto aggregator = std::make_unique<DataAggregator>();
    auto output = std::make_unique<ConsoleOutput>();

    // Добавляем сборщик CPU
    aggregator->addCollector(std::make_unique<CpuCollector>());

    // Подписываем вывод на обновления
    aggregator->subscribe([&](const DataPoint& data) {
        output->display(data);
    });

    // Запускаем агрегатор
    aggregator->start();

    // Ждем завершения (Ctrl+C)
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Останавливаем агрегатор
    aggregator->stop();

    return 0;
}