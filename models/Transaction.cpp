/**
 * @file Transaction.cpp
 * @brief Implementation of Transaction class
 */

#include "Transaction.h"
#include "Customer.h"
#include "../utils/FileManager.h"
#include <sstream>
#include <chrono>
#include <iomanip>
#include <algorithm>

Transaction::Transaction()
    : m_id(0)
    , m_customerId(0)
    , m_dateTime("")
    , m_subtotal(0.0)
    , m_discount(0.0)
    , m_total(0.0)
    , m_pointsEarned(0)
    , m_pointsUsed(0)
{
}

Transaction::Transaction(int id, int customerId)
    : m_id(id)
    , m_customerId(customerId)
    , m_dateTime("")
    , m_subtotal(0.0)
    , m_discount(0.0)
    , m_total(0.0)
    , m_pointsEarned(0)
    , m_pointsUsed(0)
{
    setCurrentDateTime();
}

int Transaction::getId() const {
    return m_id;
}

std::string Transaction::serialize() const {
    std::ostringstream oss;
    oss << m_id << "|"
        << m_customerId << "|"
        << m_dateTime << "|"
        << std::fixed << std::setprecision(0) << m_total << "|"
        << m_pointsEarned << "|"
        << m_pointsUsed << "|"
        << serializeItems();
    return oss.str();
}

void Transaction::deserialize(const std::string& data) {
    std::vector<std::string> fields = FileManager::splitLine(data, '|');

    if (fields.size() >= 7) {
        m_id = std::stoi(fields[0]);
        m_customerId = std::stoi(fields[1]);
        m_dateTime = fields[2];
        m_total = std::stod(fields[3]);
        m_pointsEarned = std::stoi(fields[4]);
        m_pointsUsed = std::stoi(fields[5]);
        deserializeItems(fields[6]);

        // Recalculate discount from points used
        m_discount = Customer::calculatePointsValue(m_pointsUsed);
        m_subtotal = m_total + m_discount;
    }
}

bool Transaction::isValid() const {
    return m_id > 0 && !m_items.empty();
}

int Transaction::getCustomerId() const {
    return m_customerId;
}

std::string Transaction::getDateTime() const {
    return m_dateTime;
}

const std::vector<TransactionItem>& Transaction::getItems() const {
    return m_items;
}

double Transaction::getSubtotal() const {
    return m_subtotal;
}

double Transaction::getDiscount() const {
    return m_discount;
}

double Transaction::getTotal() const {
    return m_total;
}

int Transaction::getPointsEarned() const {
    return m_pointsEarned;
}

int Transaction::getPointsUsed() const {
    return m_pointsUsed;
}

int Transaction::getItemCount() const {
    int count = 0;
    for (const auto& item : m_items) {
        count += item.getQuantity();
    }
    return count;
}

void Transaction::setId(int id) {
    m_id = id;
}

void Transaction::setCustomerId(int customerId) {
    m_customerId = customerId;
}

void Transaction::setDateTime(const std::string& dateTime) {
    m_dateTime = dateTime;
}

void Transaction::setPointsUsed(int points) {
    if (points >= 0) {
        m_pointsUsed = points;
        recalculate();
    }
}

void Transaction::addItem(const TransactionItem& item) {
    // Check if product already exists
    for (auto& existingItem : m_items) {
        if (existingItem.getProductId() == item.getProductId()) {
            existingItem.incrementQuantity(item.getQuantity());
            recalculate();
            return;
        }
    }

    // Add new item
    m_items.push_back(item);
    recalculate();
}

bool Transaction::removeItem(int productId) {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [productId](const TransactionItem& item) {
            return item.getProductId() == productId;
        });

    if (it != m_items.end()) {
        m_items.erase(it);
        recalculate();
        return true;
    }
    return false;
}

bool Transaction::updateItemQuantity(int productId, int quantity) {
    for (auto& item : m_items) {
        if (item.getProductId() == productId) {
            if (quantity <= 0) {
                return removeItem(productId);
            }
            item.setQuantity(quantity);
            recalculate();
            return true;
        }
    }
    return false;
}

TransactionItem* Transaction::getItem(int productId) {
    for (auto& item : m_items) {
        if (item.getProductId() == productId) {
            return &item;
        }
    }
    return nullptr;
}

void Transaction::clearItems() {
    m_items.clear();
    recalculate();
}

bool Transaction::hasItems() const {
    return !m_items.empty();
}

void Transaction::recalculate() {
    // Calculate subtotal
    m_subtotal = 0.0;
    for (const auto& item : m_items) {
        m_subtotal += item.getSubtotal();
    }

    // Calculate discount from points
    m_discount = Customer::calculatePointsValue(m_pointsUsed);

    // Ensure discount doesn't exceed subtotal (no negative totals)
    if (m_discount > m_subtotal) {
        m_discount = m_subtotal;
    }

    // Calculate final total
    m_total = m_subtotal - m_discount;

    // Calculate points earned (based on final total)
    m_pointsEarned = calculatePointsEarned();
}

int Transaction::calculatePointsEarned() const {
    return Customer::calculatePointsForAmount(m_total);
}

void Transaction::setCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    m_dateTime = oss.str();
}

std::string Transaction::serializeItems() const {
    if (m_items.empty()) {
        return "";
    }

    std::ostringstream oss;
    for (size_t i = 0; i < m_items.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }
        oss << m_items[i].serializeCompact();
    }
    return oss.str();
}

void Transaction::deserializeItems(const std::string& data) {
    m_items.clear();

    if (data.empty()) {
        return;
    }

    // Split by comma
    std::vector<std::string> itemStrings = FileManager::splitLine(data, ',');

    for (const auto& itemStr : itemStrings) {
        TransactionItem item;
        item.deserializeCompact(itemStr);
        if (item.getProductId() > 0) {
            m_items.push_back(item);
        }
    }
}
