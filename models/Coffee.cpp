/**
 * @file Coffee.cpp
 * @brief Implementation of Coffee class
 */

#include "Coffee.h"
#include "../utils/FileManager.h"
#include <sstream>

Coffee::Coffee()
    : Product()
    , m_shotSize("single")
{
    m_type = "coffee";
}

Coffee::Coffee(int id, const std::string& name, double price, const std::string& shotSize)
    : Product(id, name, price, "coffee")
    , m_shotSize(shotSize)
{
}

std::string Coffee::serialize() const {
    std::ostringstream oss;
    oss << m_id << "|" << m_name << "|" << m_price << "|" << m_type << "|" << m_shotSize;
    return oss.str();
}

void Coffee::deserialize(const std::string& data) {
    std::vector<std::string> fields = FileManager::splitLine(data, '|');

    if (fields.size() >= 5) {
        m_id = std::stoi(fields[0]);
        m_name = fields[1];
        m_price = std::stod(fields[2]);
        m_type = fields[3];
        m_shotSize = fields[4];
    }
}

std::string Coffee::getDescription() const {
    return m_name + " (" + m_shotSize + " shot)";
}

std::string Coffee::getExtraField() const {
    return m_shotSize;
}

void Coffee::setExtraField(const std::string& value) {
    m_shotSize = value;
}

Product* Coffee::clone() const {
    return new Coffee(*this);
}

std::string Coffee::getShotSize() const {
    return m_shotSize;
}

void Coffee::setShotSize(const std::string& size) {
    m_shotSize = size;
}
