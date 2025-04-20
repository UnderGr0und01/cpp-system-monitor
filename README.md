# CPP System Monitor

A lightweight, terminal-based system monitoring tool written in C++ using `ncurses`. Monitor CPU usage, memory usage, and network activity (RX/TX) in real-time with a sleek, ASCII-based interface.

## Features
- **Real-time Monitoring**: Tracks CPU, memory, and network (RX/TX) with configurable update intervals.
- **Configurable**: Customize network interface and update interval via `config.ini`.
- **Lightweight**: Minimal resource usage, built with modern C++.

## Prerequisites
- **Linux** (Ubuntu 22.04 recommended)
- **Dependencies**:
  - `g++` (C++17 or later)
  - `cmake` (3.16 or later)
  - `libncursesw5-dev` (for terminal UI)

## Installation 
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/cpp-system-monitor.git
   cd cpp-system-monitor
   ```

2. **Install Dependencies** (Ubuntu):
   ```bash
   sudo apt update
   sudo apt install g++ cmake libncursesw5-dev
   ```

3. **Build the Project**:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. **Run the Monitor**:
   ```bash
   ./monitor
   `````

## Configuration
Edit `config.ini` in the project root to customize settings:
```ini
[monitor]
interface=eth0 # Network interface (e.g., eth0, wlan0)
update_interval_ms=1000  # Update interval in milliseconds
```

Find available interfaces with:
```bash
ip link
```

## Usage
- **Start**: Run `./monitor`.
- **Controls**:
  - `Ctrl+C`: Exit gracefully.
- **Output**:
  ```
  + System Monitor ----------------------+
  | CPU Usage:    [####      ]43.58%    |
  | Memory Usage: [########  ]85.32%    |
  | Network (wlp0s20f3): RX  0.00 KB/s ---- |
  |                     TX  0.00 KB/s ---- |
  | Network (Total): RX  0.00 KB/s ---- |
  |                  TX  0.00 KB/s ---- |
  +-------------------------------------+
  ```

## Troubleshooting 
- **Build errors**:
  - Verify `libncursesw5-dev` is installed.
  - Clear build directory: `rm -rf build/*`