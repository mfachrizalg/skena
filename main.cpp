/**
 * @file main.cpp
 * @brief Application entry point for Coffee Shop POS
 *
 * This file demonstrates the MVC pattern by showing how the
 * application components are wired together at startup.
 *
 * Architecture Overview:
 * - Model: Product, Coffee, Snack, Customer, Transaction classes
 * - View: MainWindow, ProductView, CustomerView, TransactionView
 * - Controller: ProductController, CustomerController, TransactionController
 *
 * OOP Principles Demonstrated:
 * - Abstraction: IEntity interface for all models
 * - Polymorphism: Coffee and Snack inherit from Product
 * - Inheritance: Product hierarchy
 * - Encapsulation: Private members with public accessors
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Each class has one job
 * - Open/Closed: Product hierarchy is extensible
 * - Liskov Substitution: Derived products can replace base
 * - Interface Segregation: Minimal interfaces
 * - Dependency Inversion: Controllers depend on abstractions
 */

#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "views/MainWindow.h"

int main(int argc, char *argv[]) {
    // Create Qt application
    QApplication app(argc, argv);

    // Set application info
    QApplication::setApplicationName("Coffee Shop POS");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Skena");

    // Set Fusion style for modern look
    app.setStyle(QStyleFactory::create("Fusion"));

    // Ensure data directory exists
    QDir dataDir("data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }

    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();

    // Run application event loop
    return app.exec();
}
