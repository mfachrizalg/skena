/**
 * @file TransactionItem.cpp
 * @brief Implementation of TransactionItem class
 */

#include "TransactionItem.h"
#include "../utils/FileManager.h"
#include <sstream>

TransactionItem::TransactionItem()
    : m_productId(0)
    , m_productName("")
    , m_unitPrice(0.0)
    , m_quantity(0)
{
}

TransactionItem::TransactionItem(int productId, const std::string& productName, double unitPrice, int quantity)
    : m_productId(productId)
    , m_productName(productName)
    , m_unitPrice(unitPrice)
    , m_quantity(quantity)
{
}

int TransactionItem::getProductId() const {
    return m_productId;
}

std::string TransactionItem::getProductName() const {
    return m_productName;
}

double TransactionItem::getUnitPrice() const {
    return m_unitPrice;
}

int TransactionItem::getQuantity() const {
    return m_quantity;
}

double TransactionItem::getSubtotal() const {
    return m_unitPrice * m_quantity;
}

void TransactionItem::setProductId(int id) {
    m_productId = id;
}

void TransactionItem::setProductName(const std::string& name) {
    m_productName = name;
}

void TransactionItem::setUnitPrice(double price) {
    if (price >= 0) {
        m_unitPrice = price;
    }
}

void TransactionItem::setQuantity(int qty) {
    if (qty >= 0) {
        m_quantity = qty;
    }
}

void TransactionItem::incrementQuantity(int amount) {
    if (amount > 0) {
        m_quantity += amount;
    }
}

bool TransactionItem::decrementQuantity(int amount) {
    if (amount > 0 && m_quantity >= amount) {
        m_quantity -= amount;
        return m_quantity > 0;
    }
    return false;
}

std::string TransactionItem::serializeCompact() const {
    std::ostringstream oss;
    oss << m_productId << ":" << m_quantity;
    return oss.str();
}

void TransactionItem::deserializeCompact(const std::string& data) {
    std::vector<std::string> parts = FileManager::splitLine(data, ':');
    if (parts.size() >= 2) {
        m_productId = std::stoi(parts[0]);
        m_quantity = std::stoi(parts[1]);
    }
}
