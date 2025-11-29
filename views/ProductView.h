/**
 * @file ProductView.h
 * @brief View for product management (CRUD operations)
 *
 * MVC Pattern: This is the View component for products.
 * It displays product data and captures user input, delegating
 * all business logic to the ProductController.
 */

#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include "../controllers/ProductController.h"

/**
 * @class ProductView
 * @brief Widget for managing products (add, edit, delete, view)
 */
class ProductView : public QWidget {
    Q_OBJECT

private:
    ProductController* m_controller;  ///< Product controller reference

    // Table
    QTableWidget* m_productTable;

    // Filter
    QComboBox* m_typeFilter;

    // Form fields
    QLineEdit* m_nameEdit;
    QDoubleSpinBox* m_priceSpinBox;
    QComboBox* m_typeComboBox;
    QLineEdit* m_extraFieldEdit;
    QLabel* m_extraFieldLabel;

    // Buttons
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_clearButton;

    // Currently selected product ID (-1 if none)
    int m_selectedProductId;

    /**
     * @brief Sets up the UI layout
     */
    void setupUI();

    /**
     * @brief Connects signals and slots
     */
    void connectSignals();

    /**
     * @brief Updates the extra field label based on product type
     */
    void updateExtraFieldLabel();

    /**
     * @brief Clears the form fields
     */
    void clearForm();

    /**
     * @brief Populates form with product data
     * @param product Product to populate from
     */
    void populateForm(const Product* product);

public:
    /**
     * @brief Constructs the ProductView
     * @param controller Pointer to ProductController
     * @param parent Parent widget
     */
    explicit ProductView(ProductController* controller, QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ProductView() override = default;

public slots:
    /**
     * @brief Refreshes the product table
     */
    void refreshTable();

private slots:
    /**
     * @brief Handles add button click
     */
    void onAddProduct();

    /**
     * @brief Handles edit button click
     */
    void onEditProduct();

    /**
     * @brief Handles delete button click
     */
    void onDeleteProduct();

    /**
     * @brief Handles clear button click
     */
    void onClearForm();

    /**
     * @brief Handles table row selection
     */
    void onTableRowSelected();

    /**
     * @brief Handles filter change
     */
    void onFilterChanged();

    /**
     * @brief Handles product type change in form
     */
    void onTypeChanged();

signals:
    /**
     * @brief Emitted when data changes
     */
    void dataChanged();
};

#endif // PRODUCTVIEW_H
