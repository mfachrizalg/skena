/**
 * @file IdGenerator.cpp
 * @brief Implementation of IdGenerator class
 */

#include "IdGenerator.h"

IdGenerator& IdGenerator::getInstance() {
    static IdGenerator instance;
    return instance;
}

int IdGenerator::getNextId(const std::string& entityType) {
    // If counter doesn't exist, it will be initialized to 0
    return ++m_counters[entityType];
}

void IdGenerator::updateCounter(const std::string& entityType, int id) {
    // Only update if the provided ID is greater than current counter
    if (id > m_counters[entityType]) {
        m_counters[entityType] = id;
    }
}

int IdGenerator::getCurrentCounter(const std::string& entityType) const {
    auto it = m_counters.find(entityType);
    if (it != m_counters.end()) {
        return it->second;
    }
    return 0;
}

void IdGenerator::resetCounter(const std::string& entityType) {
    m_counters[entityType] = 0;
}

void IdGenerator::resetAll() {
    m_counters.clear();
}
