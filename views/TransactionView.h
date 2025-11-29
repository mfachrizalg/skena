/**
 * @file TransactionView.h
 * @brief Main POS view for handling transactions
 *
 * MVC Pattern: This is the View component for transactions.
 * It provides the primary Point of Sales interface.
 */

#ifndef TRANSACTIONVIEW_H
#define TRANSACTIONVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include "../controllers/TransactionController.h"
#include "../controllers/ProductController.h"
#include "../controllers/CustomerController.h"

/**
 * @class TransactionView
 * @brief Main Point of Sales interface widget
 */
class TransactionView : public QWidget {
    Q_OBJECT

private:
    TransactionController* m_transactionController;
    ProductController* m_productController;
    CustomerController* m_customerController;

    // Product Selection (Left Panel)
    QListWidget* m_coffeeList;
    QListWidget* m_snackList;

    // Cart (Center Panel)
    QTableWidget* m_cartTable;
    QLabel* m_subtotalLabel;
    QLabel* m_discountLabel;
    QLabel* m_totalLabel;
    QPushButton* m_removeItemButton;

    // Customer & Payment (Right Panel)
    QComboBox* m_customerComboBox;
    QLabel* m_customerPointsLabel;
    QSpinBox* m_usePointsSpinBox;
    QLabel* m_pointsValueLabel;
    QPushButton* m_completeButton;
    QPushButton* m_cancelButton;

    // Transaction History
    QTableWidget* m_historyTable;

    /**
     * @brief Sets up the UI layout
     */
    void setupUI();

    /**
     * @brief Creates the left panel (product selection)
     */
    QWidget* createProductPanel();

    /**
     * @brief Creates the center panel (cart)
     */
    QWidget* createCartPanel();

    /**
     * @brief Creates the right panel (customer & payment)
     */
    QWidget* createPaymentPanel();

    /**
     * @brief Creates the bottom panel (transaction history)
     */
    QWidget* createHistoryPanel();

    /**
     * @brief Connects signals and slots
     */
    void connectSignals();

    /**
     * @brief Updates the totals display
     */
    void updateTotals();

    /**
     * @brief Updates points value label
     */
    void updatePointsValue();

public:
    /**
     * @brief Constructs the TransactionView
     * @param transactionController Pointer to TransactionController
     * @param productController Pointer to ProductController
     * @param customerController Pointer to CustomerController
     * @param parent Parent widget
     */
    TransactionView(TransactionController* transactionController,
                    ProductController* productController,
                    CustomerController* customerController,
                    QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~TransactionView() override = default;

public slots:
    /**
     * @brief Refreshes the product lists
     */
    void refreshProductLists();

    /**
     * @brief Refreshes the customer dropdown
     */
    void refreshCustomerList();

    /**
     * @brief Refreshes the cart table
     */
    void refreshCart();

    /**
     * @brief Refreshes the transaction history
     */
    void refreshHistory();

    /**
     * @brief Refreshes all views
     */
    void refreshAll();

private slots:
    /**
     * @brief Handles coffee item double-click
     */
    void onCoffeeItemDoubleClicked(QListWidgetItem* item);

    /**
     * @brief Handles snack item double-click
     */
    void onSnackItemDoubleClicked(QListWidgetItem* item);

    /**
     * @brief Handles remove item from cart
     */
    void onRemoveFromCart();

    /**
     * @brief Handles cart quantity change
     */
    void onCartQuantityChanged(int row, int column);

    /**
     * @brief Handles customer selection change
     */
    void onCustomerChanged();

    /**
     * @brief Handles points usage change
     */
    void onPointsUsageChanged();

    /**
     * @brief Handles complete transaction
     */
    void onCompleteTransaction();

    /**
     * @brief Handles cancel transaction
     */
    void onCancelTransaction();

signals:
    /**
     * @brief Emitted when a transaction is completed
     */
    void transactionCompleted();
};

#endif // TRANSACTIONVIEW_H
