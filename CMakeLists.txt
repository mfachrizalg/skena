cmake_minimum_required(VERSION 3.30)

# --- COMPILER SELECTION (Must be before project()!) ---

# OPTION 1: Use GCC (and G++)
#set(CMAKE_C_COMPILER "gcc")
#set(CMAKE_CXX_COMPILER "g++")

# OPTION 2: Use Clang (Uncomment these and comment out GCC above to use Clang)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# ------------------------------------------------------

project(skena)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

add_executable(skena main.cpp)
target_link_libraries(skena
  Qt::Core
  Qt::Gui
  Qt::Widgets
)

