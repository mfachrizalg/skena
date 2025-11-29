/**
 * @file Product.h
 * @brief Abstract base class for all product types
 *
 * OOP Principles Applied:
 * - Abstraction: Abstract class with pure virtual methods
 * - Encapsulation: Private/protected members with public accessors
 * - Inheritance: Base class for Coffee and Snack
 *
 * SOLID Principles Applied:
 * - Open/Closed: Open for extension (new product types), closed for modification
 * - Liskov Substitution: Derived classes can substitute for Product
 */

#ifndef PRODUCT_H
#define PRODUCT_H

#include "IEntity.h"
#include <string>

/**
 * @class Product
 * @brief Abstract base class for all products in the coffee shop
 *
 * This class defines the common interface and data for all products.
 * Coffee and Snack classes inherit from this class and provide
 * specific implementations.
 */
class Product : public IEntity {
protected:
    int m_id;              ///< Unique product identifier
    std::string m_name;    ///< Product name
    double m_price;        ///< Price in IDR
    std::string m_type;    ///< Product type ("coffee" or "snack")

public:
    /**
     * @brief Default constructor
     */
    Product();

    /**
     * @brief Parameterized constructor
     * @param id Unique identifier
     * @param name Product name
     * @param price Price in IDR
     * @param type Product type
     */
    Product(int id, const std::string& name, double price, const std::string& type);

    /**
     * @brief Virtual destructor
     */
    virtual ~Product() = default;

    // ============ IEntity Interface Implementation ============

    int getId() const override;
    bool isValid() const override;

    // serialize() and deserialize() are implemented by derived classes

    // ============ Getters ============

    /**
     * @brief Gets the product name
     * @return Product name
     */
    std::string getName() const;

    /**
     * @brief Gets the product price
     * @return Price in IDR
     */
    double getPrice() const;

    /**
     * @brief Gets the product type
     * @return "coffee" or "snack"
     */
    std::string getType() const;

    // ============ Setters ============

    /**
     * @brief Sets the product ID
     * @param id New ID value
     */
    void setId(int id);

    /**
     * @brief Sets the product name
     * @param name New name
     */
    void setName(const std::string& name);

    /**
     * @brief Sets the product price
     * @param price New price in IDR
     */
    void setPrice(double price);

    // ============ Virtual Methods (Polymorphism) ============

    /**
     * @brief Gets a detailed description of the product
     * @return Product description string
     *
     * Pure virtual - must be implemented by derived classes
     */
    virtual std::string getDescription() const = 0;

    /**
     * @brief Calculates total price for a quantity
     * @param quantity Number of items
     * @return Total price
     *
     * Virtual with default implementation - can be overridden
     */
    virtual double calculatePrice(int quantity) const;

    /**
     * @brief Gets the extra field value specific to product type
     * @return Extra field value (e.g., shot size for coffee, category for snack)
     */
    virtual std::string getExtraField() const = 0;

    /**
     * @brief Sets the extra field value specific to product type
     * @param value New extra field value
     */
    virtual void setExtraField(const std::string& value) = 0;

    /**
     * @brief Creates a clone of this product
     * @return Pointer to a new Product copy
     */
    virtual Product* clone() const = 0;
};

#endif // PRODUCT_H
