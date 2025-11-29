/**
 * @file IController.h
 * @brief Abstract controller interface defining common operations
 *
 * SOLID Principles Applied:
 * - Interface Segregation: Minimal interface with essential CRUD operations
 * - Dependency Inversion: High-level modules depend on this abstraction
 */

#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <vector>

/**
 * @class IController
 * @brief Template interface for all controllers
 *
 * This template class defines the contract for controller classes,
 * providing standard CRUD operations and file I/O methods.
 *
 * @tparam T The type of entity this controller manages
 */
template<typename T>
class IController {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~IController() = default;

    // ============ CRUD Operations ============

    /**
     * @brief Gets all entities
     * @return Vector of all entities
     */
    virtual std::vector<T> getAll() const = 0;

    /**
     * @brief Gets an entity by ID
     * @param id Entity ID to find
     * @return Pointer to entity, nullptr if not found
     */
    virtual T* getById(int id) = 0;

    /**
     * @brief Adds a new entity
     * @param item Entity to add
     * @return true if successful
     */
    virtual bool add(const T& item) = 0;

    /**
     * @brief Updates an existing entity
     * @param item Entity with updated data
     * @return true if found and updated
     */
    virtual bool update(const T& item) = 0;

    /**
     * @brief Removes an entity by ID
     * @param id Entity ID to remove
     * @return true if found and removed
     */
    virtual bool remove(int id) = 0;

    // ============ File I/O ============

    /**
     * @brief Saves all entities to file
     * @return true if successful
     */
    virtual bool saveToFile() = 0;

    /**
     * @brief Loads all entities from file
     * @return true if successful
     */
    virtual bool loadFromFile() = 0;

    // ============ Query ============

    /**
     * @brief Gets the total count of entities
     * @return Number of entities
     */
    virtual int getCount() const = 0;
};

#endif // ICONTROLLER_H
