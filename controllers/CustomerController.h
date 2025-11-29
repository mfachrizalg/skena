/**
 * @file CustomerController.h
 * @brief Controller for managing customers and loyalty points
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Only manages customer operations
 * - Dependency Inversion: Depends on FileManager abstraction
 */

#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include "IController.h"
#include "../models/Customer.h"
#include "../utils/FileManager.h"
#include <vector>

/**
 * @class CustomerController
 * @brief Manages customer CRUD operations and loyalty points
 */
class CustomerController : public IController<Customer> {
private:
    std::vector<Customer> m_customers;  ///< Customer collection
    FileManager& m_fileManager;          ///< File I/O handler
    static constexpr const char* FILENAME = "customers.txt";

public:
    /**
     * @brief Constructs controller with FileManager dependency
     * @param fileManager Reference to FileManager for I/O
     */
    explicit CustomerController(FileManager& fileManager);

    /**
     * @brief Destructor
     */
    ~CustomerController() override = default;

    // ============ IController Interface Implementation ============

    std::vector<Customer> getAll() const override;
    Customer* getById(int id) override;
    bool add(const Customer& customer) override;
    bool update(const Customer& customer) override;
    bool remove(int id) override;
    bool saveToFile() override;
    bool loadFromFile() override;
    int getCount() const override;

    // ============ Customer-Specific Methods ============

    /**
     * @brief Creates a new customer with generated ID
     * @param name Customer name
     * @param phone Phone number
     * @return Newly created Customer
     */
    Customer createCustomer(const std::string& name, const std::string& phone);

    /**
     * @brief Searches customers by name (partial match)
     * @param searchTerm Search term
     * @return Vector of matching customers
     */
    std::vector<Customer*> searchByName(const std::string& searchTerm);

    /**
     * @brief Gets customer by phone number
     * @param phone Phone number to search
     * @return Pointer to customer, nullptr if not found
     */
    Customer* getByPhone(const std::string& phone);

    // ============ Loyalty Points Operations ============

    /**
     * @brief Adds loyalty points to a customer
     * @param customerId Customer ID
     * @param points Points to add
     * @return true if successful
     */
    bool addLoyaltyPoints(int customerId, int points);

    /**
     * @brief Redeems loyalty points from a customer
     * @param customerId Customer ID
     * @param points Points to redeem
     * @return true if successful (sufficient points)
     */
    bool redeemLoyaltyPoints(int customerId, int points);

    /**
     * @brief Gets current loyalty points for a customer
     * @param customerId Customer ID
     * @return Points balance, -1 if customer not found
     */
    int getLoyaltyPoints(int customerId) const;
};

#endif // CUSTOMERCONTROLLER_H
