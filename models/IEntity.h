/**
 * @file IEntity.h
 * @brief Abstract base interface for all model entities
 *
 * OOP Principle: Abstraction
 * - Defines a common contract for all model classes
 * - Pure virtual functions enforce implementation in derived classes
 *
 * SOLID Principles Applied:
 * - Interface Segregation: Minimal interface with only essential methods
 * - Liskov Substitution: All entities can be treated as IEntity
 */

#ifndef IENTITY_H
#define IENTITY_H

#include <string>

/**
 * @class IEntity
 * @brief Abstract base class (interface) for all model entities
 *
 * This interface defines the contract that all model classes must follow.
 * It provides methods for identification and serialization/deserialization
 * for file persistence.
 */
class IEntity {
public:
    /**
     * @brief Virtual destructor for proper cleanup in derived classes
     */
    virtual ~IEntity() = default;

    /**
     * @brief Gets the unique identifier of the entity
     * @return The entity's ID
     */
    virtual int getId() const = 0;

    /**
     * @brief Serializes the entity to a CSV-format string
     * @return String representation suitable for file storage
     *
     * The format uses '|' as delimiter:
     * id|field1|field2|...
     */
    virtual std::string serialize() const = 0;

    /**
     * @brief Deserializes data from a CSV-format string
     * @param data String representation from file
     *
     * Parses the '|' delimited string and populates entity fields.
     */
    virtual void deserialize(const std::string& data) = 0;

    /**
     * @brief Checks if the entity is valid
     * @return true if the entity has valid data, false otherwise
     */
    virtual bool isValid() const = 0;
};

#endif // IENTITY_H
