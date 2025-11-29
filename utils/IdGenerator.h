/**
 * @file IdGenerator.h
 * @brief Utility class for generating unique IDs
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Only generates unique IDs
 */

#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <atomic>
#include <unordered_map>
#include <string>

/**
 * @class IdGenerator
 * @brief Generates unique sequential IDs for different entity types
 *
 * This class maintains separate ID counters for each entity type
 * (products, customers, transactions) to ensure unique IDs within
 * each category.
 */
class IdGenerator {
private:
    std::unordered_map<std::string, int> m_counters;  ///< Counters per entity type

    // Private constructor for singleton pattern
    IdGenerator() = default;

public:
    // Delete copy constructor and assignment operator
    IdGenerator(const IdGenerator&) = delete;
    IdGenerator& operator=(const IdGenerator&) = delete;

    /**
     * @brief Gets the singleton instance
     * @return Reference to the IdGenerator instance
     */
    static IdGenerator& getInstance();

    /**
     * @brief Generates the next unique ID for an entity type
     * @param entityType Type of entity (e.g., "product", "customer", "transaction")
     * @return New unique ID
     */
    int getNextId(const std::string& entityType);

    /**
     * @brief Updates the counter if a higher ID is found
     * @param entityType Type of entity
     * @param id ID value to consider
     *
     * This is useful when loading existing data to ensure
     * new IDs don't conflict with existing ones.
     */
    void updateCounter(const std::string& entityType, int id);

    /**
     * @brief Gets the current counter value for an entity type
     * @param entityType Type of entity
     * @return Current counter value (0 if not initialized)
     */
    int getCurrentCounter(const std::string& entityType) const;

    /**
     * @brief Resets the counter for an entity type
     * @param entityType Type of entity
     */
    void resetCounter(const std::string& entityType);

    /**
     * @brief Resets all counters
     */
    void resetAll();
};

#endif // IDGENERATOR_H
