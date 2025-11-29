/**
 * @file TransactionController.h
 * @brief Controller for managing transactions
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Only manages transaction operations
 * - Dependency Inversion: Depends on abstractions (FileManager, other controllers)
 */

#ifndef TRANSACTIONCONTROLLER_H
#define TRANSACTIONCONTROLLER_H

#include "IController.h"
#include "../models/Transaction.h"
#include "../utils/FileManager.h"
#include "ProductController.h"
#include "CustomerController.h"
#include <vector>

/**
 * @class TransactionController
 * @brief Manages transaction operations and coordinates with other controllers
 *
 * This controller handles the complete transaction flow, including
 * cart management, points calculation, and finalization.
 */
class TransactionController : public IController<Transaction> {
private:
    std::vector<Transaction> m_transactions;     ///< Completed transactions
    Transaction m_currentTransaction;            ///< Active cart/transaction
    FileManager& m_fileManager;                  ///< File I/O handler
    ProductController& m_productController;      ///< Product operations
    CustomerController& m_customerController;    ///< Customer operations
    static constexpr const char* FILENAME = "transactions.txt";

public:
    /**
     * @brief Constructs controller with dependencies
     * @param fileManager Reference to FileManager
     * @param productController Reference to ProductController
     * @param customerController Reference to CustomerController
     */
    TransactionController(FileManager& fileManager,
                          ProductController& productController,
                          CustomerController& customerController);

    /**
     * @brief Destructor
     */
    ~TransactionController() override = default;

    // ============ IController Interface Implementation ============

    std::vector<Transaction> getAll() const override;
    Transaction* getById(int id) override;
    bool add(const Transaction& transaction) override;
    bool update(const Transaction& transaction) override;
    bool remove(int id) override;
    bool saveToFile() override;
    bool loadFromFile() override;
    int getCount() const override;

    // ============ Current Transaction (Cart) Operations ============

    /**
     * @brief Starts a new transaction
     * @param customerId Customer ID (0 for guest)
     */
    void startNewTransaction(int customerId = 0);

    /**
     * @brief Gets the current active transaction
     * @return Reference to current transaction
     */
    Transaction& getCurrentTransaction();

    /**
     * @brief Gets the current active transaction (const)
     * @return Const reference to current transaction
     */
    const Transaction& getCurrentTransaction() const;

    /**
     * @brief Adds a product to the current transaction
     * @param productId Product ID to add
     * @param quantity Quantity (default: 1)
     * @return true if product found and added
     */
    bool addToCart(int productId, int quantity = 1);

    /**
     * @brief Removes a product from the current transaction
     * @param productId Product ID to remove
     * @return true if removed
     */
    bool removeFromCart(int productId);

    /**
     * @brief Updates quantity of a product in cart
     * @param productId Product ID
     * @param quantity New quantity
     * @return true if updated
     */
    bool updateCartQuantity(int productId, int quantity);

    /**
     * @brief Clears the current cart
     */
    void clearCart();

    /**
     * @brief Sets the customer for current transaction
     * @param customerId Customer ID
     */
    void setCurrentCustomer(int customerId);

    /**
     * @brief Sets points to use for current transaction
     * @param points Points to redeem
     * @return true if customer has enough points
     */
    bool setPointsToUse(int points);

    /**
     * @brief Completes the current transaction
     * @return true if transaction was completed successfully
     *
     * This will:
     * 1. Finalize the transaction
     * 2. Deduct loyalty points if used
     * 3. Add earned loyalty points to customer
     * 4. Add transaction to history
     * 5. Clear the cart
     */
    bool completeTransaction();

    /**
     * @brief Cancels the current transaction
     */
    void cancelTransaction();

    /**
     * @brief Checks if cart has items
     * @return true if cart is not empty
     */
    bool hasItemsInCart() const;

    // ============ Query Methods ============

    /**
     * @brief Gets transactions for a specific customer
     * @param customerId Customer ID
     * @return Vector of transactions for that customer
     */
    std::vector<Transaction*> getByCustomerId(int customerId);

    /**
     * @brief Gets transactions for a date range
     * @param startDate Start date (YYYY-MM-DD)
     * @param endDate End date (YYYY-MM-DD)
     * @return Vector of transactions in range
     */
    std::vector<Transaction*> getByDateRange(const std::string& startDate,
                                              const std::string& endDate);

    /**
     * @brief Calculates total revenue from all transactions
     * @return Total revenue in IDR
     */
    double getTotalRevenue() const;

    /**
     * @brief Gets recent transactions
     * @param count Number of recent transactions to get
     * @return Vector of recent transactions
     */
    std::vector<Transaction*> getRecent(int count);
};

#endif // TRANSACTIONCONTROLLER_H
