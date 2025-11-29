/**
 * @file Snack.h
 * @brief Snack product class - inherits from Product
 *
 * OOP Principles Applied:
 * - Inheritance: Inherits from Product base class
 * - Polymorphism: Overrides virtual methods from Product
 * - Encapsulation: Private member m_category with accessors
 */

#ifndef SNACK_H
#define SNACK_H

#include "Product.h"

/**
 * @class Snack
 * @brief Represents a snack product in the coffee shop
 *
 * Snack products have an additional attribute for category
 * (pastry, sandwich, other) which helps classify the item.
 */
class Snack : public Product {
private:
    std::string m_category;  ///< Category: "pastry", "sandwich", "other"

public:
    /**
     * @brief Default constructor
     */
    Snack();

    /**
     * @brief Parameterized constructor
     * @param id Unique identifier
     * @param name Snack name
     * @param price Price in IDR
     * @param category Category ("pastry", "sandwich", "other")
     */
    Snack(int id, const std::string& name, double price, const std::string& category = "other");

    /**
     * @brief Virtual destructor
     */
    virtual ~Snack() = default;

    // ============ IEntity Interface Implementation ============

    /**
     * @brief Serializes snack data to CSV format
     * @return "id|name|price|snack|category"
     */
    std::string serialize() const override;

    /**
     * @brief Deserializes snack data from CSV format
     * @param data CSV string in format "id|name|price|snack|category"
     */
    void deserialize(const std::string& data) override;

    // ============ Product Virtual Methods Override ============

    /**
     * @brief Gets snack description with category
     * @return Description like "Croissant [pastry]"
     */
    std::string getDescription() const override;

    /**
     * @brief Gets the category
     * @return Category string
     */
    std::string getExtraField() const override;

    /**
     * @brief Sets the category
     * @param value New category
     */
    void setExtraField(const std::string& value) override;

    /**
     * @brief Creates a copy of this snack
     * @return Pointer to new Snack object
     */
    Product* clone() const override;

    // ============ Snack-Specific Methods ============

    /**
     * @brief Gets the snack category
     * @return "pastry", "sandwich", or "other"
     */
    std::string getCategory() const;

    /**
     * @brief Sets the snack category
     * @param category New category
     */
    void setCategory(const std::string& category);
};

#endif // SNACK_H
