# 🚁 Drone Swarm Simulator

> A high-performance, interactive 2D drone swarm simulation built with **C++** and **Raylib**.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-20-blue.svg)
![Raylib](https://img.shields.io/badge/Raylib-5.0-orange.svg)

## 📌 Overview

**Drone Swarm Simulator** is a real-time agent-based simulation that demonstrates complex emergent behaviors from simple rules. It utilizes **Flow Fields** and **Path Integration** for efficient pathfinding of thousands of agents simultaneously.

Whether you're interested in swarm intelligence, game development, or just want to see thousands of drones navigate a maze, this project serves as a clean, performant example.

## ✨ Features

-   **High Performance Rendering**: Efficiently renders thousands of agents using Raylib.
-   **Flow Field Pathfinding**: Agents navigate towards a target using a calculated potential field, allowing for organic movement around obstacles.
-   **Interactive World**:
    -   **Add/Remove Walls**: Dynamically modify the environment with a smart brush system.
    -   **Heatmap Visualization**: See the underlying cost/integration field in real-time.
    -   **Grid Visualization**: Toggle grid lines for precise editing.
-   **Customizable Parameters**: Adjust max speed, steering force, and friction on the fly via the built-in UI.
-   **Smart Brush**: Intelligent wall editing that remembers your initial click action (draw vs. erase).

## 🚀 Getting Started

### Prerequisites

-   **CMake** (Version 3.11 or higher)
-   **C++ Compiler** (supporting C++20)
-   **Git**

### Installation

1.  **Clone the repository**
    ```bash
    git clone https://github.com/yourusername/DroneSwarmSim.git
    cd DroneSwarmSim
    ```

2.  **Build the project**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

3.  **Run the simulation**
    -   **Windows**: `.\Debug\DroneSwarmSim.exe`
    -   **Linux/Mac**: `./DroneSwarmSim`

## 🎮 Controls

| Input | Action |
| :--- | :--- |
| **Left Click** | Set Target Position |
| **Right Click** | Toggle Wall (Click) / Paint Walls (Hold) |
| **R Key** | Reset Simulation (Randomize Agents) |
| **ESC** | Exit Simulation |

## 🛠️ Built With

*   [Raylib](https://www.raylib.com/) - A simple and easy-to-use library to enjoy videogames programming.
*   [CMake](https://cmake.org/) - Cross-platform family of tools designed to build, test and package software.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request


