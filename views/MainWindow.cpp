/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow class
 */

#include "MainWindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Initialize utilities
    m_fileManager = new FileManager("data/");

    // Initialize controllers with dependency injection (SOLID: Dependency Inversion)
    m_productController = new ProductController(*m_fileManager);
    m_customerController = new CustomerController(*m_fileManager);
    m_transactionController = new TransactionController(
        *m_fileManager, *m_productController, *m_customerController);

    // Setup UI
    setupUI();
    createMenuBar();
    createStatusBar();
    connectSignals();

    // Load data
    initializeData();

    // Window settings
    setWindowTitle("Coffee Shop POS - Skena");
    setMinimumSize(1024, 768);
    resize(1280, 800);
}

MainWindow::~MainWindow() {
    // Save data before closing
    onSave();

    // Clean up controllers
    delete m_transactionController;
    delete m_customerController;
    delete m_productController;
    delete m_fileManager;
}

void MainWindow::setupUI() {
    // Create central tab widget
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    // Create views with controller injection (MVC pattern)
    m_transactionView = new TransactionView(
        m_transactionController, m_productController, m_customerController);
    m_productView = new ProductView(m_productController);
    m_customerView = new CustomerView(m_customerController);

    // Add tabs
    m_tabWidget->addTab(m_transactionView, "Point of Sale");
    m_tabWidget->addTab(m_productView, "Products");
    m_tabWidget->addTab(m_customerView, "Customers");
}

void MainWindow::createMenuBar() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    m_saveAction = new QAction("&Save", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    fileMenu->addAction(m_saveAction);

    m_loadAction = new QAction("&Reload", this);
    m_loadAction->setShortcut(QKeySequence::Refresh);
    fileMenu->addAction(m_loadAction);

    fileMenu->addSeparator();

    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(m_exitAction);

    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");

    m_aboutAction = new QAction("&About", this);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals() {
    // Menu actions
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::onLoad);
    connect(m_exitAction, &QAction::triggered, this, &QMainWindow::close);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    // Data change signals
    connect(m_productView, &ProductView::dataChanged, this, &MainWindow::onDataChanged);
    connect(m_customerView, &CustomerView::dataChanged, this, &MainWindow::onDataChanged);
    connect(m_transactionView, &TransactionView::transactionCompleted, this, &MainWindow::onDataChanged);

    // Refresh views when switching tabs
    connect(m_tabWidget, &QTabWidget::currentChanged, [this](int index) {
        if (index == 0) {
            m_transactionView->refreshAll();
        }
        updateStatusBar();
    });
}

void MainWindow::initializeData() {
    // Load all data from files
    m_productController->loadFromFile();
    m_customerController->loadFromFile();
    m_transactionController->loadFromFile();

    // Refresh all views
    m_productView->refreshTable();
    m_customerView->refreshTable();
    m_transactionView->refreshAll();

    updateStatusBar();
    statusBar()->showMessage("Data loaded successfully", 3000);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // Ask to save before closing
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Save Changes",
        "Do you want to save changes before closing?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Save) {
        onSave();
        event->accept();
    } else if (reply == QMessageBox::Discard) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::onSave() {
    bool success = true;

    if (!m_productController->saveToFile()) {
        success = false;
    }
    if (!m_customerController->saveToFile()) {
        success = false;
    }
    if (!m_transactionController->saveToFile()) {
        success = false;
    }

    if (success) {
        statusBar()->showMessage("All data saved successfully", 3000);
    } else {
        statusBar()->showMessage("Error saving some data", 3000);
        QMessageBox::warning(this, "Save Error",
            "Some data could not be saved. Please check file permissions.");
    }
}

void MainWindow::onLoad() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Reload Data",
        "This will reload all data from files. Any unsaved changes will be lost. Continue?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        initializeData();
    }
}

void MainWindow::onDataChanged() {
    updateStatusBar();
    // Auto-save on data change (optional - can be removed for manual save only)
    // onSave();
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About Coffee Shop POS",
        "<h2>Coffee Shop POS</h2>"
        "<p>Version 1.0</p>"
        "<p>A Point of Sales application for Coffee Shop management.</p>"
        "<h3>Features:</h3>"
        "<ul>"
        "<li>Product Management (Coffee & Snacks)</li>"
        "<li>Customer Loyalty Points</li>"
        "<li>Transaction Processing</li>"
        "<li>Transaction History</li>"
        "</ul>"
        "<h3>Technical Stack:</h3>"
        "<ul>"
        "<li>C++17</li>"
        "<li>Qt 6 Widgets</li>"
        "<li>MVC Architecture</li>"
        "<li>SOLID Principles</li>"
        "</ul>"
        "<p><b>OOP Principles Demonstrated:</b></p>"
        "<ul>"
        "<li>Abstraction (IEntity interface)</li>"
        "<li>Inheritance (Product hierarchy)</li>"
        "<li>Polymorphism (Virtual methods)</li>"
        "<li>Encapsulation (Private members)</li>"
        "</ul>"
    );
}

void MainWindow::updateStatusBar() {
    QString status = QString("Products: %1 | Customers: %2 | Transactions: %3")
        .arg(m_productController->getCount())
        .arg(m_customerController->getCount())
        .arg(m_transactionController->getCount());

    statusBar()->showMessage(status);
}
