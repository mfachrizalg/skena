/**
 * @file Transaction.h
 * @brief Transaction model class representing a complete order
 *
 * OOP Principles Applied:
 * - Encapsulation: Private members with controlled access
 * - Abstraction: Implements IEntity interface
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "IEntity.h"
#include "TransactionItem.h"
#include <vector>
#include <string>

/**
 * @class Transaction
 * @brief Represents a complete sales transaction
 *
 * A transaction contains multiple items, customer information,
 * and loyalty points data. It calculates totals and manages
 * the points earned/used.
 */
class Transaction : public IEntity {
private:
    int m_id;                              ///< Unique transaction identifier
    int m_customerId;                      ///< Customer ID (0 for guest)
    std::string m_dateTime;                ///< Transaction date/time
    std::vector<TransactionItem> m_items;  ///< Items in the transaction
    double m_subtotal;                     ///< Total before discount
    double m_discount;                     ///< Discount from points
    double m_total;                        ///< Final total after discount
    int m_pointsEarned;                    ///< Points earned from this transaction
    int m_pointsUsed;                      ///< Points redeemed in this transaction

public:
    /**
     * @brief Default constructor
     */
    Transaction();

    /**
     * @brief Parameterized constructor
     * @param id Transaction ID
     * @param customerId Customer ID (0 for guest)
     */
    Transaction(int id, int customerId = 0);

    /**
     * @brief Virtual destructor
     */
    virtual ~Transaction() = default;

    // ============ IEntity Interface Implementation ============

    int getId() const override;
    std::string serialize() const override;
    void deserialize(const std::string& data) override;
    bool isValid() const override;

    // ============ Getters ============

    int getCustomerId() const;
    std::string getDateTime() const;
    const std::vector<TransactionItem>& getItems() const;
    double getSubtotal() const;
    double getDiscount() const;
    double getTotal() const;
    int getPointsEarned() const;
    int getPointsUsed() const;
    int getItemCount() const;

    // ============ Setters ============

    void setId(int id);
    void setCustomerId(int customerId);
    void setDateTime(const std::string& dateTime);
    void setPointsUsed(int points);

    // ============ Item Management ============

    /**
     * @brief Adds an item to the transaction
     * @param item TransactionItem to add
     *
     * If product already exists, quantity is incremented.
     */
    void addItem(const TransactionItem& item);

    /**
     * @brief Removes an item by product ID
     * @param productId Product ID to remove
     * @return true if removed, false if not found
     */
    bool removeItem(int productId);

    /**
     * @brief Updates quantity of an item
     * @param productId Product ID
     * @param quantity New quantity
     * @return true if updated, false if not found
     */
    bool updateItemQuantity(int productId, int quantity);

    /**
     * @brief Gets an item by product ID
     * @param productId Product ID to find
     * @return Pointer to item, nullptr if not found
     */
    TransactionItem* getItem(int productId);

    /**
     * @brief Clears all items from the transaction
     */
    void clearItems();

    /**
     * @brief Checks if transaction has any items
     * @return true if items exist
     */
    bool hasItems() const;

    // ============ Calculations ============

    /**
     * @brief Recalculates all totals
     *
     * Updates subtotal, discount, total, and points earned
     * based on current items and points used.
     */
    void recalculate();

    /**
     * @brief Calculates points that can be earned
     * @return Points earned based on final total
     */
    int calculatePointsEarned() const;

    /**
     * @brief Sets the current date/time
     */
    void setCurrentDateTime();

    // ============ Serialization Helpers ============

    /**
     * @brief Serializes items to compact format
     * @return "productId:qty,productId:qty,..." format
     */
    std::string serializeItems() const;

    /**
     * @brief Deserializes items from compact format
     * @param data Items string in compact format
     */
    void deserializeItems(const std::string& data);
};

#endif // TRANSACTION_H
