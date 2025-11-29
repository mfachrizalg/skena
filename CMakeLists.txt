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
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

# Source files organized by layer (MVC Architecture)
set(UTIL_SOURCES
    utils/FileManager.cpp
    utils/IdGenerator.cpp
)

set(MODEL_SOURCES
    models/Product.cpp
    models/Coffee.cpp
    models/Snack.cpp
    models/Customer.cpp
    models/TransactionItem.cpp
    models/Transaction.cpp
)

set(CONTROLLER_SOURCES
    controllers/ProductController.cpp
    controllers/CustomerController.cpp
    controllers/TransactionController.cpp
)

set(VIEW_SOURCES
    views/ProductView.cpp
    views/CustomerView.cpp
    views/TransactionView.cpp
    views/MainWindow.cpp
)

# Header files for Qt MOC
set(HEADERS
    utils/FileManager.h
    utils/IdGenerator.h
    models/IEntity.h
    models/Product.h
    models/Coffee.h
    models/Snack.h
    models/Customer.h
    models/TransactionItem.h
    models/Transaction.h
    controllers/IController.h
    controllers/ProductController.h
    controllers/CustomerController.h
    controllers/TransactionController.h
    views/ProductView.h
    views/CustomerView.h
    views/TransactionView.h
    views/MainWindow.h
)

# Create executable with all sources
add_executable(skena
    main.cpp
    ${UTIL_SOURCES}
    ${MODEL_SOURCES}
    ${CONTROLLER_SOURCES}
    ${VIEW_SOURCES}
    ${HEADERS}
)

# Include directories
target_include_directories(skena PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/views
)

# Link Qt libraries
target_link_libraries(skena
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

# Copy data files to build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)

# Install data files
install(DIRECTORY data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
