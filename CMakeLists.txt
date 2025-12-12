cmake_minimum_required(VERSION 3.11)

# Proje Adı ve Versiyonu
project(DroneSwarmSim VERSION 1.0.0 LANGUAGES CXX)

# C++ Standartı (C++20 kullanalım, modern olsun)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib kütüphanesini otomatik indir ve projeye dahil et (FetchContent)
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/master.tar.gz
)
FetchContent_MakeAvailable(raylib)

# Kaynak dosyalarımız
add_executable(${PROJECT_NAME} src/main.cpp)

# Raylib'i projemize bağla (Link)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)