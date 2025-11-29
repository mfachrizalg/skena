/**
 * @file Customer.h
 * @brief Customer model class with loyalty points functionality
 *
 * OOP Principles Applied:
 * - Encapsulation: Private members with controlled access via methods
 * - Abstraction: Implements IEntity interface
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Only manages customer data and loyalty points
 */

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "IEntity.h"
#include <string>

/**
 * @class Customer
 * @brief Represents a customer in the loyalty program
 *
 * Customers can earn and redeem loyalty points based on their purchases.
 * Points are earned at 1 point per 1,000 IDR and redeemed at 100 IDR per point.
 */
class Customer : public IEntity {
private:
    int m_id;                 ///< Unique customer identifier
    std::string m_name;       ///< Customer name
    std::string m_phone;      ///< Phone number
    int m_loyaltyPoints;      ///< Current loyalty points balance

public:
    // ============ Constants ============

    static constexpr int POINTS_PER_UNIT = 1000;      ///< IDR per point earned
    static constexpr int POINT_VALUE = 100;           ///< IDR value per point
    static constexpr int MIN_REDEEM_POINTS = 10;      ///< Minimum points to redeem

    // ============ Constructors ============

    /**
     * @brief Default constructor
     */
    Customer();

    /**
     * @brief Parameterized constructor
     * @param id Unique identifier
     * @param name Customer name
     * @param phone Phone number
     * @param loyaltyPoints Initial loyalty points (default: 0)
     */
    Customer(int id, const std::string& name, const std::string& phone, int loyaltyPoints = 0);

    /**
     * @brief Virtual destructor
     */
    virtual ~Customer() = default;

    // ============ IEntity Interface Implementation ============

    int getId() const override;
    std::string serialize() const override;
    void deserialize(const std::string& data) override;
    bool isValid() const override;

    // ============ Getters ============

    /**
     * @brief Gets the customer name
     * @return Customer name
     */
    std::string getName() const;

    /**
     * @brief Gets the phone number
     * @return Phone number string
     */
    std::string getPhone() const;

    /**
     * @brief Gets current loyalty points balance
     * @return Number of points
     */
    int getLoyaltyPoints() const;

    // ============ Setters ============

    /**
     * @brief Sets the customer ID
     * @param id New ID
     */
    void setId(int id);

    /**
     * @brief Sets the customer name
     * @param name New name
     */
    void setName(const std::string& name);

    /**
     * @brief Sets the phone number
     * @param phone New phone number
     */
    void setPhone(const std::string& phone);

    /**
     * @brief Sets the loyalty points balance
     * @param points New points balance
     */
    void setLoyaltyPoints(int points);

    // ============ Loyalty Points Operations ============

    /**
     * @brief Adds points to the customer's balance
     * @param points Number of points to add
     */
    void addPoints(int points);

    /**
     * @brief Attempts to redeem points
     * @param points Number of points to redeem
     * @return true if successful, false if insufficient points
     */
    bool redeemPoints(int points);

    /**
     * @brief Calculates points earned for a transaction amount
     * @param amount Transaction amount in IDR
     * @return Number of points earned
     */
    static int calculatePointsForAmount(double amount);

    /**
     * @brief Calculates the IDR value of points
     * @param points Number of points
     * @return Value in IDR
     */
    static double calculatePointsValue(int points);

    /**
     * @brief Checks if points can be redeemed
     * @param points Points to check
     * @return true if enough points available
     */
    bool canRedeemPoints(int points) const;
};

#endif // CUSTOMER_H
