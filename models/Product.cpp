/**
 * @file Product.cpp
 * @brief Implementation of Product base class
 */

#include "Product.h"

Product::Product()
    : m_id(0)
    , m_name("")
    , m_price(0.0)
    , m_type("")
{
}

Product::Product(int id, const std::string& name, double price, const std::string& type)
    : m_id(id)
    , m_name(name)
    , m_price(price)
    , m_type(type)
{
}

int Product::getId() const {
    return m_id;
}

bool Product::isValid() const {
    return m_id > 0 && !m_name.empty() && m_price >= 0;
}

std::string Product::getName() const {
    return m_name;
}

double Product::getPrice() const {
    return m_price;
}

std::string Product::getType() const {
    return m_type;
}

void Product::setId(int id) {
    m_id = id;
}

void Product::setName(const std::string& name) {
    m_name = name;
}

void Product::setPrice(double price) {
    if (price >= 0) {
        m_price = price;
    }
}

double Product::calculatePrice(int quantity) const {
    if (quantity <= 0) {
        return 0.0;
    }
    return m_price * quantity;
}
