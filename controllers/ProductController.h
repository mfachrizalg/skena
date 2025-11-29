/**
 * @file ProductController.h
 * @brief Controller for managing products (Coffee and Snacks)
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Only manages product operations
 * - Dependency Inversion: Depends on FileManager abstraction
 * - Open/Closed: Can work with any Product subclass
 */

#ifndef PRODUCTCONTROLLER_H
#define PRODUCTCONTROLLER_H

#include "IController.h"
#include "../models/Product.h"
#include "../utils/FileManager.h"
#include <memory>
#include <vector>

/**
 * @class ProductController
 * @brief Manages product CRUD operations and persistence
 *
 * This controller handles both Coffee and Snack products using
 * polymorphism. Products are stored as unique_ptr for memory safety.
 */
class ProductController {
private:
    std::vector<std::unique_ptr<Product>> m_products;  ///< Product collection
    FileManager& m_fileManager;                         ///< File I/O handler
    static constexpr const char* FILENAME = "products.txt";

public:
    /**
     * @brief Constructs controller with FileManager dependency
     * @param fileManager Reference to FileManager for I/O
     */
    explicit ProductController(FileManager& fileManager);

    /**
     * @brief Destructor
     */
    ~ProductController() = default;

    // ============ CRUD Operations ============

    /**
     * @brief Gets all products as raw pointers
     * @return Vector of product pointers
     */
    std::vector<Product*> getAll() const;

    /**
     * @brief Gets a product by ID
     * @param id Product ID
     * @return Pointer to product, nullptr if not found
     */
    Product* getById(int id);

    /**
     * @brief Gets products by type
     * @param type "coffee" or "snack"
     * @return Vector of matching products
     */
    std::vector<Product*> getByType(const std::string& type) const;

    /**
     * @brief Adds a new product (takes ownership)
     * @param product Unique pointer to product
     * @return true if successful
     */
    bool add(std::unique_ptr<Product> product);

    /**
     * @brief Updates an existing product
     * @param product Product with updated data
     * @return true if found and updated
     */
    bool update(const Product& product);

    /**
     * @brief Removes a product by ID
     * @param id Product ID to remove
     * @return true if found and removed
     */
    bool remove(int id);

    // ============ Factory Methods ============

    /**
     * @brief Creates a new Coffee product
     * @param name Product name
     * @param price Price in IDR
     * @param shotSize Shot size (single/double)
     * @return Unique pointer to new Coffee
     */
    std::unique_ptr<Product> createCoffee(const std::string& name, double price,
                                           const std::string& shotSize = "single");

    /**
     * @brief Creates a new Snack product
     * @param name Product name
     * @param price Price in IDR
     * @param category Category (pastry/sandwich/other)
     * @return Unique pointer to new Snack
     */
    std::unique_ptr<Product> createSnack(const std::string& name, double price,
                                          const std::string& category = "other");

    // ============ File I/O ============

    /**
     * @brief Saves all products to file
     * @return true if successful
     */
    bool saveToFile();

    /**
     * @brief Loads all products from file
     * @return true if successful
     */
    bool loadFromFile();

    // ============ Query ============

    /**
     * @brief Gets total product count
     * @return Number of products
     */
    int getCount() const;

    /**
     * @brief Gets count by product type
     * @param type "coffee" or "snack"
     * @return Number of products of that type
     */
    int getCountByType(const std::string& type) const;
};

#endif // PRODUCTCONTROLLER_H
