# Coffee Shop POS (Point of Sale)

A GUI-based Point of Sale application for a Coffee Shop built with **C++17** and **Qt 6 Widgets**.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Qt6](https://img.shields.io/badge/Qt-6-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.30%2B-red.svg)

---

## Table of Contents

- [Features](#features)
- [Screenshots](#screenshots)
- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Linux](#linux)
  - [macOS](#macos)
  - [Windows](#windows)
- [Building the Project](#building-the-project)
  - [Using Clang (Default)](#using-clang-default)
  - [Using GCC](#using-gcc)
  - [Using an IDE](#using-an-ide)
- [Running the Application](#running-the-application)
- [Project Structure](#project-structure)
- [Data Files](#data-files)
- [OOP Principles](#oop-principles)
- [SOLID Principles](#solid-principles)
- [Contributing](#contributing)
- [License](#license)

---

## Features

- **Product Management**
  - Add, edit, delete Coffee and Snack products
  - Filter products by type
  - Store product details (name, price, type, extra attributes)

- **Customer Loyalty Program**
  - Register customers with contact information
  - Earn loyalty points: 1 point per Rp 1,000 spent
  - Redeem points: 1 point = Rp 100 discount
  - No limit on point redemption per transaction

- **Point of Sale (POS)**
  - Create orders with multiple products
  - Real-time cart management
  - Apply loyalty point discounts
  - Automatic points calculation

- **Transaction History**
  - View all completed transactions
  - Track customer purchase history
  - Monitor revenue

- **Data Persistence**
  - CSV-style text file storage (no database required)
  - Auto-load on startup
  - Save on exit with confirmation

---

## Screenshots

```
+------------------------------------------------------------------+
|  File   Help                                    Coffee Shop POS  |
+------------------------------------------------------------------+
| [Point of Sale] [Products] [Customers]                           |
+------------------------------------------------------------------+
|                                                                  |
|  +-----------------+  +-------------------+  +------------------+|
|  | PRODUCTS        |  | CART              |  | CUSTOMER         ||
|  |                 |  |                   |  |                  ||
|  | Coffee:         |  | Item   Qty  Price |  | [Customer v]     ||
|  | - Espresso      |  | Latte   2   64000 |  | Points: 150      ||
|  | - Latte         |  | Cookie  1   12000 |  |                  ||
|  | - Cappuccino    |  |                   |  | Use Points:      ||
|  |                 |  | Subtotal: 76000   |  | [  0  ] pts      ||
|  | Snacks:         |  | Discount: 0       |  | = Rp 0           ||
|  | - Croissant     |  | Total: 76000      |  |                  ||
|  | - Cookie        |  |                   |  | [Complete Order] ||
|  |                 |  | [Remove Item]     |  | [Cancel]         ||
|  +-----------------+  +-------------------+  +------------------+|
|                                                                  |
+------------------------------------------------------------------+
```

---

## Architecture

This project follows the **Model-View-Controller (MVC)** architectural pattern:

```
┌─────────────────────────────────────────────────────────────────┐
│                           VIEWS                                 │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐  │
│  │ ProductView │  │CustomerView │  │    TransactionView      │  │
│  └──────┬──────┘  └──────┬──────┘  └────────────┬────────────┘  │
│         │                │                      │               │
│         └────────────────┼──────────────────────┘               │
│                          │                                      │
│                   ┌──────┴──────┐                               │
│                   │  MainWindow │                               │
│                   └─────────────┘                               │
└─────────────────────────────────────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                       CONTROLLERS                               │
│  ┌─────────────────┐ ┌──────────────────┐ ┌──────────────────┐  │
│  │ProductController│ │CustomerController│ │TransactionController│
│  └────────┬────────┘ └────────┬─────────┘ └─────────┬────────┘  │
│           │                   │                     │           │
│           └───────────────────┼─────────────────────┘           │
│                               │                                 │
│                        ┌──────┴──────┐                          │
│                        │ FileManager │                          │
│                        └─────────────┘                          │
└─────────────────────────────────────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                         MODELS                                  │
│      ┌─────────┐                                                │
│      │ IEntity │ (Abstract Interface)                           │
│      └────┬────┘                                                │
│           │                                                     │
│   ┌───────┼───────────────────┐                                 │
│   │       │                   │                                 │
│   ▼       ▼                   ▼                                 │
│┌───────┐ ┌────────┐    ┌─────────────┐                          │
││Product│ │Customer│    │ Transaction │                          │
│└───┬───┘ └────────┘    └─────────────┘                          │
│    │                                                            │
│  ┌─┴──────────┐                                                 │
│  │            │                                                 │
│  ▼            ▼                                                 │
│┌──────┐  ┌───────┐                                              │
││Coffee│  │ Snack │                                              │
│└──────┘  └───────┘                                              │
└─────────────────────────────────────────────────────────────────┘
```

---

## Prerequisites

- **CMake** 3.30 or higher
- **C++17** compatible compiler (GCC 8+ or Clang 7+)
- **Qt 6** (Core, Gui, Widgets modules)

---

## Installation

### Linux

#### Fedora
```bash
sudo dnf install cmake qt6-qtbase-devel gcc-c++ clang
```

#### Ubuntu / Debian
```bash
sudo apt update
sudo apt install cmake qt6-base-dev g++ clang
```

#### Arch Linux
```bash
sudo pacman -S cmake qt6-base gcc clang
```

#### openSUSE
```bash
sudo zypper install cmake qt6-base-devel gcc-c++ clang
```

---

### macOS

#### Using Homebrew (Recommended)
```bash
# Install Homebrew if not installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake qt@6

# Add Qt to PATH (add to ~/.zshrc or ~/.bash_profile for persistence)
export CMAKE_PREFIX_PATH=$(brew --prefix qt@6)
```

#### Using Qt Online Installer
1. Download from https://www.qt.io/download-qt-installer
2. Run the installer and select Qt 6.x for macOS
3. Note the installation path (e.g., `~/Qt/6.x.x/macos`)

---

### Windows

#### Option 1: Qt Online Installer (Recommended)
1. Download the Qt Online Installer from https://www.qt.io/download-qt-installer
2. Run the installer
3. Select components:
   - Qt 6.x.x → MSVC 2019/2022 64-bit (for MSVC)
   - Qt 6.x.x → MinGW 64-bit (for GCC)
   - Developer and Designer Tools → CMake
4. Note the installation path (e.g., `C:\Qt\6.x.x\msvc2019_64`)

#### Option 2: vcpkg
```powershell
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install Qt6
.\vcpkg install qt6-base:x64-windows
```

#### Option 3: MSYS2 (for GCC/MinGW)
```bash
# In MSYS2 MINGW64 terminal
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-qt6-base mingw-w64-x86_64-gcc
```

---

## Building the Project

### Using Clang (Default)

The project is configured to use Clang by default.

#### Linux / macOS
```bash
# Clone or navigate to the project
cd skena

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build . -j$(nproc)
```

#### Windows (with LLVM/Clang)
```powershell
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2019_64"
cmake --build . --config Release
```

---

### Using GCC

To use GCC instead of Clang, modify `CMakeLists.txt`:

```cmake
# Comment out Clang
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

# Uncomment GCC
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
```

Or specify the compiler via command line:

#### Linux / macOS
```bash
mkdir build && cd build

# Using GCC
cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build . -j$(nproc)

# Using Clang
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build . -j$(nproc)
```

#### Windows (MinGW/GCC)
```powershell
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\mingw_64"
mingw32-make
```

---

### Using an IDE

#### CLion
1. Open the project folder in CLion
2. CLion will automatically detect `CMakeLists.txt`
3. If Qt6 is not found, go to:
   - **Settings → Build, Execution, Deployment → CMake**
   - Add to **CMake options**:
     ```
     -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
     ```
4. Click **Build** or press `Ctrl+F9`

#### Qt Creator
1. Open `CMakeLists.txt` with Qt Creator
2. Select your Qt 6 kit
3. Configure and build

#### Visual Studio (Windows)
1. Open the project folder
2. Visual Studio will detect CMake
3. Configure Qt path in `CMakeSettings.json`:
   ```json
   {
     "configurations": [
       {
         "name": "x64-Release",
         "generator": "Ninja",
         "configurationType": "Release",
         "buildRoot": "${projectDir}\\build",
         "cmakeCommandArgs": "-DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64"
       }
     ]
   }
   ```

#### VS Code
1. Install extensions: CMake, CMake Tools, C/C++
2. Open the project folder
3. Configure CMake kit (select GCC or Clang)
4. Add to `.vscode/settings.json`:
   ```json
   {
     "cmake.configureArgs": [
       "-DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64"
     ]
   }
   ```
5. Press `Ctrl+Shift+P` → "CMake: Build"

---

## Running the Application

### Linux
```bash
cd build
./skena
```

### macOS
```bash
cd build
./skena

# Or if built as app bundle
open skena.app
```

### Windows
```powershell
cd build
.\skena.exe

# Or double-click skena.exe in File Explorer
```

### Important Notes

1. **Data Directory**: The application expects a `data/` folder in the working directory. Run the application from the project root or build directory.

2. **First Run**: Sample data files are included:
   - `data/products.txt` - 16 sample products
   - `data/customers.txt` - 5 sample customers
   - `data/transactions.txt` - 3 sample transactions

3. **Qt DLLs (Windows)**: If you get missing DLL errors, either:
   - Run `windeployqt skena.exe` from Qt's bin directory
   - Add Qt's bin folder to your PATH

---

## Project Structure

```
skena/
├── CMakeLists.txt              # Build configuration
├── main.cpp                    # Application entry point
├── README.md                   # This file
│
├── data/                       # Data files (CSV format)
│   ├── products.txt            # Product catalog
│   ├── customers.txt           # Customer database
│   └── transactions.txt        # Transaction history
│
├── models/                     # Data models (M in MVC)
│   ├── IEntity.h               # Abstract base interface
│   ├── Product.h/.cpp          # Base product class
│   ├── Coffee.h/.cpp           # Coffee product (extends Product)
│   ├── Snack.h/.cpp            # Snack product (extends Product)
│   ├── Customer.h/.cpp         # Customer with loyalty points
│   ├── Transaction.h/.cpp      # Complete transaction
│   └── TransactionItem.h/.cpp  # Cart line item
│
├── views/                      # UI components (V in MVC)
│   ├── MainWindow.h/.cpp       # Main application window
│   ├── ProductView.h/.cpp      # Product management UI
│   ├── CustomerView.h/.cpp     # Customer management UI
│   └── TransactionView.h/.cpp  # POS interface
│
├── controllers/                # Business logic (C in MVC)
│   ├── IController.h           # Controller interface
│   ├── ProductController.h/.cpp
│   ├── CustomerController.h/.cpp
│   └── TransactionController.h/.cpp
│
└── utils/                      # Utility classes
    ├── FileManager.h/.cpp      # File I/O operations
    └── IdGenerator.h/.cpp      # Unique ID generation
```

---

## Data Files

Data is stored in CSV-style format using `|` as delimiter.

### products.txt
```
# Format: id|name|price|type|extra_field
1|Espresso|20000|coffee|single
9|Croissant|22000|snack|pastry
```

### customers.txt
```
# Format: id|name|phone|loyalty_points
1|Budi Santoso|081234567890|250
```

### transactions.txt
```
# Format: id|customer_id|date|total|points_earned|points_used|items
# Items: product_id:quantity,product_id:quantity
1|1|2025-01-15 10:30:00|64000|64|0|4:1,9:1,14:1
```

---

## OOP Principles

This project demonstrates all four pillars of Object-Oriented Programming:

### 1. Abstraction
```cpp
// IEntity.h - Abstract interface hiding implementation details
class IEntity {
public:
    virtual ~IEntity() = default;
    virtual int getId() const = 0;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};
```

### 2. Encapsulation
```cpp
// Product.h - Private data with public accessors
class Product : public IEntity {
protected:
    int m_id;           // Hidden from outside
    std::string m_name;
    double m_price;
public:
    std::string getName() const;  // Controlled access
    void setPrice(double price);  // Validation possible
};
```

### 3. Inheritance
```cpp
// Coffee inherits from Product
class Coffee : public Product {
private:
    std::string m_shotSize;  // Additional attribute
public:
    std::string getDescription() const override;
};
```

### 4. Polymorphism
```cpp
// Different behavior for same interface
std::vector<Product*> products;
products.push_back(new Coffee(...));
products.push_back(new Snack(...));

for (auto* p : products) {
    // Calls Coffee::getDescription() or Snack::getDescription()
    std::cout << p->getDescription() << std::endl;
}
```

---

## SOLID Principles

### Single Responsibility Principle (SRP)
Each class has one reason to change:
- `FileManager` - Only handles file I/O
- `ProductController` - Only manages products
- `ProductView` - Only displays product UI

### Open/Closed Principle (OCP)
```cpp
// Open for extension (new product types)
class Beverage : public Product { ... };  // Easy to add

// Closed for modification (existing code unchanged)
```

### Liskov Substitution Principle (LSP)
```cpp
// Any Product subclass can be used where Product is expected
void processProduct(Product* product) {
    std::cout << product->getDescription();  // Works for Coffee or Snack
}
```

### Interface Segregation Principle (ISP)
```cpp
// IEntity has minimal methods
class IEntity {
    virtual int getId() const = 0;
    virtual std::string serialize() const = 0;
    // No unnecessary methods
};
```

### Dependency Inversion Principle (DIP)
```cpp
// Controllers depend on abstractions (FileManager), not concrete files
class ProductController {
    FileManager& m_fileManager;  // Injected dependency
public:
    ProductController(FileManager& fm) : m_fileManager(fm) {}
};
```

---

## Troubleshooting

### Qt6 not found
```
CMake Error: Could not find a package configuration file provided by "Qt6"
```
**Solution**: Set `CMAKE_PREFIX_PATH`:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
```

### Missing Qt DLLs (Windows)
```
The application was unable to start correctly (0xc000007b)
```
**Solution**: Run Qt's deployment tool:
```powershell
C:\Qt\6.x.x\msvc2019_64\bin\windeployqt.exe skena.exe
```

### Compiler not found
```
CMake Error: CMAKE_CXX_COMPILER not set
```
**Solution**: Install compiler or specify path:
```bash
sudo apt install g++  # or clang
cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/g++
```

### Data files not found
```
Application runs but shows no products/customers
```
**Solution**: Ensure `data/` folder is in the working directory:
```bash
cd /path/to/skena
./build/skena  # Run from project root
```

---

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## Acknowledgments

- Built with [Qt Framework](https://www.qt.io/)
- Developed for Pemrograman Dasar (Basic Programming) course
- Demonstrates C++17 features and OOP best practices
