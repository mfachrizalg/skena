/**
 * @file TransactionItem.h
 * @brief Represents a single item in a transaction (line item)
 *
 * OOP Principles Applied:
 * - Encapsulation: Private members with public accessors
 */

#ifndef TRANSACTIONITEM_H
#define TRANSACTIONITEM_H

#include <string>

/**
 * @class TransactionItem
 * @brief Represents a single product line item in a transaction
 *
 * Each transaction can contain multiple items, where each item
 * represents a product with its quantity and price at time of purchase.
 */
class TransactionItem {
private:
    int m_productId;          ///< Product ID reference
    std::string m_productName; ///< Product name (stored for history)
    double m_unitPrice;        ///< Price per unit at time of purchase
    int m_quantity;            ///< Number of units

public:
    /**
     * @brief Default constructor
     */
    TransactionItem();

    /**
     * @brief Parameterized constructor
     * @param productId Product ID
     * @param productName Product name
     * @param unitPrice Price per unit
     * @param quantity Number of units
     */
    TransactionItem(int productId, const std::string& productName, double unitPrice, int quantity);

    // ============ Getters ============

    int getProductId() const;
    std::string getProductName() const;
    double getUnitPrice() const;
    int getQuantity() const;

    /**
     * @brief Calculates subtotal for this item
     * @return unitPrice * quantity
     */
    double getSubtotal() const;

    // ============ Setters ============

    void setProductId(int id);
    void setProductName(const std::string& name);
    void setUnitPrice(double price);
    void setQuantity(int qty);

    /**
     * @brief Increases quantity by specified amount
     * @param amount Amount to add (default: 1)
     */
    void incrementQuantity(int amount = 1);

    /**
     * @brief Decreases quantity by specified amount
     * @param amount Amount to subtract (default: 1)
     * @return true if quantity is still positive after decrement
     */
    bool decrementQuantity(int amount = 1);

    // ============ Serialization ============

    /**
     * @brief Serializes to compact format for transaction storage
     * @return "productId:quantity"
     */
    std::string serializeCompact() const;

    /**
     * @brief Deserializes from compact format
     * @param data "productId:quantity" format
     */
    void deserializeCompact(const std::string& data);
};

#endif // TRANSACTIONITEM_H
