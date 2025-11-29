/**
 * @file CustomerView.h
 * @brief View for customer management and loyalty points
 *
 * MVC Pattern: This is the View component for customers.
 */

#ifndef CUSTOMERVIEW_H
#define CUSTOMERVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include "../controllers/CustomerController.h"

/**
 * @class CustomerView
 * @brief Widget for managing customers and loyalty points
 */
class CustomerView : public QWidget {
    Q_OBJECT

private:
    CustomerController* m_controller;  ///< Customer controller reference

    // Table
    QTableWidget* m_customerTable;

    // Search
    QLineEdit* m_searchEdit;
    QPushButton* m_searchButton;

    // Form fields
    QLineEdit* m_nameEdit;
    QLineEdit* m_phoneEdit;
    QSpinBox* m_pointsSpinBox;

    // Buttons
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_clearButton;

    // Points management
    QSpinBox* m_addPointsSpinBox;
    QPushButton* m_addPointsButton;

    // Currently selected customer ID (-1 if none)
    int m_selectedCustomerId;

    /**
     * @brief Sets up the UI layout
     */
    void setupUI();

    /**
     * @brief Connects signals and slots
     */
    void connectSignals();

    /**
     * @brief Clears the form fields
     */
    void clearForm();

    /**
     * @brief Populates form with customer data
     * @param customer Customer to populate from
     */
    void populateForm(const Customer* customer);

public:
    /**
     * @brief Constructs the CustomerView
     * @param controller Pointer to CustomerController
     * @param parent Parent widget
     */
    explicit CustomerView(CustomerController* controller, QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~CustomerView() override = default;

public slots:
    /**
     * @brief Refreshes the customer table
     */
    void refreshTable();

private slots:
    /**
     * @brief Handles add button click
     */
    void onAddCustomer();

    /**
     * @brief Handles edit button click
     */
    void onEditCustomer();

    /**
     * @brief Handles delete button click
     */
    void onDeleteCustomer();

    /**
     * @brief Handles clear button click
     */
    void onClearForm();

    /**
     * @brief Handles table row selection
     */
    void onTableRowSelected();

    /**
     * @brief Handles search
     */
    void onSearch();

    /**
     * @brief Handles add points button click
     */
    void onAddPoints();

signals:
    /**
     * @brief Emitted when data changes
     */
    void dataChanged();
};

#endif // CUSTOMERVIEW_H
