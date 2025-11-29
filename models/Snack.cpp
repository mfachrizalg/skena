/**
 * @file Snack.cpp
 * @brief Implementation of Snack class
 */

#include "Snack.h"
#include "../utils/FileManager.h"
#include <sstream>

Snack::Snack()
    : Product()
    , m_category("other")
{
    m_type = "snack";
}

Snack::Snack(int id, const std::string& name, double price, const std::string& category)
    : Product(id, name, price, "snack")
    , m_category(category)
{
}

std::string Snack::serialize() const {
    std::ostringstream oss;
    oss << m_id << "|" << m_name << "|" << m_price << "|" << m_type << "|" << m_category;
    return oss.str();
}

void Snack::deserialize(const std::string& data) {
    std::vector<std::string> fields = FileManager::splitLine(data, '|');

    if (fields.size() >= 5) {
        m_id = std::stoi(fields[0]);
        m_name = fields[1];
        m_price = std::stod(fields[2]);
        m_type = fields[3];
        m_category = fields[4];
    }
}

std::string Snack::getDescription() const {
    return m_name + " [" + m_category + "]";
}

std::string Snack::getExtraField() const {
    return m_category;
}

void Snack::setExtraField(const std::string& value) {
    m_category = value;
}

Product* Snack::clone() const {
    return new Snack(*this);
}

std::string Snack::getCategory() const {
    return m_category;
}

void Snack::setCategory(const std::string& category) {
    m_category = category;
}
