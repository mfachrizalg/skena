/**
 * @file MainWindow.h
 * @brief Main application window containing all views
 *
 * MVC Pattern: This is the main View container that coordinates
 * all sub-views and handles application-level operations.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QAction>
#include <QMenu>
#include "ProductView.h"
#include "CustomerView.h"
#include "TransactionView.h"
#include "../controllers/ProductController.h"
#include "../controllers/CustomerController.h"
#include "../controllers/TransactionController.h"
#include "../utils/FileManager.h"

/**
 * @class MainWindow
 * @brief Main application window
 *
 * This class serves as the main container for the POS application,
 * managing all controllers, views, and file operations.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    // Utilities (owned)
    FileManager* m_fileManager;

    // Controllers (owned)
    ProductController* m_productController;
    CustomerController* m_customerController;
    TransactionController* m_transactionController;

    // Views (owned by Qt parent system)
    QTabWidget* m_tabWidget;
    ProductView* m_productView;
    CustomerView* m_customerView;
    TransactionView* m_transactionView;

    // Menu actions
    QAction* m_saveAction;
    QAction* m_loadAction;
    QAction* m_exitAction;
    QAction* m_aboutAction;

    /**
     * @brief Sets up the UI layout
     */
    void setupUI();

    /**
     * @brief Creates menu bar
     */
    void createMenuBar();

    /**
     * @brief Creates status bar
     */
    void createStatusBar();

    /**
     * @brief Connects signals and slots
     */
    void connectSignals();

    /**
     * @brief Initializes data (load from files)
     */
    void initializeData();

public:
    /**
     * @brief Constructs the MainWindow
     * @param parent Parent widget
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor - saves data before closing
     */
    ~MainWindow() override;

protected:
    /**
     * @brief Handles close event - prompts to save
     * @param event Close event
     */
    void closeEvent(QCloseEvent* event) override;

private slots:
    /**
     * @brief Saves all data to files
     */
    void onSave();

    /**
     * @brief Loads all data from files
     */
    void onLoad();

    /**
     * @brief Handles data changes
     */
    void onDataChanged();

    /**
     * @brief Shows about dialog
     */
    void onAbout();

    /**
     * @brief Updates status bar
     */
    void updateStatusBar();
};

#endif // MAINWINDOW_H
