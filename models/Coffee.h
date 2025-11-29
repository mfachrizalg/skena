/**
 * @file Coffee.h
 * @brief Coffee product class - inherits from Product
 *
 * OOP Principles Applied:
 * - Inheritance: Inherits from Product base class
 * - Polymorphism: Overrides virtual methods from Product
 * - Encapsulation: Private member m_shotSize with accessors
 */

#ifndef COFFEE_H
#define COFFEE_H

#include "Product.h"

/**
 * @class Coffee
 * @brief Represents a coffee product in the coffee shop
 *
 * Coffee products have an additional attribute for shot size
 * (single, double) which affects the description.
 */
class Coffee : public Product {
private:
    std::string m_shotSize;  ///< Shot size: "single", "double"

public:
    /**
     * @brief Default constructor
     */
    Coffee();

    /**
     * @brief Parameterized constructor
     * @param id Unique identifier
     * @param name Coffee name
     * @param price Price in IDR
     * @param shotSize Shot size ("single" or "double")
     */
    Coffee(int id, const std::string& name, double price, const std::string& shotSize = "single");

    /**
     * @brief Virtual destructor
     */
    virtual ~Coffee() = default;

    // ============ IEntity Interface Implementation ============

    /**
     * @brief Serializes coffee data to CSV format
     * @return "id|name|price|coffee|shotSize"
     */
    std::string serialize() const override;

    /**
     * @brief Deserializes coffee data from CSV format
     * @param data CSV string in format "id|name|price|coffee|shotSize"
     */
    void deserialize(const std::string& data) override;

    // ============ Product Virtual Methods Override ============

    /**
     * @brief Gets coffee description with shot size
     * @return Description like "Espresso (double shot)"
     */
    std::string getDescription() const override;

    /**
     * @brief Gets the shot size
     * @return Shot size string
     */
    std::string getExtraField() const override;

    /**
     * @brief Sets the shot size
     * @param value New shot size
     */
    void setExtraField(const std::string& value) override;

    /**
     * @brief Creates a copy of this coffee
     * @return Pointer to new Coffee object
     */
    Product* clone() const override;

    // ============ Coffee-Specific Methods ============

    /**
     * @brief Gets the shot size
     * @return "single" or "double"
     */
    std::string getShotSize() const;

    /**
     * @brief Sets the shot size
     * @param size "single" or "double"
     */
    void setShotSize(const std::string& size);
};

#endif // COFFEE_H
