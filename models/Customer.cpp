/**
 * @file Customer.cpp
 * @brief Implementation of Customer class
 */

#include "Customer.h"
#include "../utils/FileManager.h"
#include <sstream>

Customer::Customer()
    : m_id(0)
    , m_name("")
    , m_phone("")
    , m_loyaltyPoints(0)
{
}

Customer::Customer(int id, const std::string& name, const std::string& phone, int loyaltyPoints)
    : m_id(id)
    , m_name(name)
    , m_phone(phone)
    , m_loyaltyPoints(loyaltyPoints)
{
}

int Customer::getId() const {
    return m_id;
}

std::string Customer::serialize() const {
    std::ostringstream oss;
    oss << m_id << "|" << m_name << "|" << m_phone << "|" << m_loyaltyPoints;
    return oss.str();
}

void Customer::deserialize(const std::string& data) {
    std::vector<std::string> fields = FileManager::splitLine(data, '|');

    if (fields.size() >= 4) {
        m_id = std::stoi(fields[0]);
        m_name = fields[1];
        m_phone = fields[2];
        m_loyaltyPoints = std::stoi(fields[3]);
    }
}

bool Customer::isValid() const {
    return m_id > 0 && !m_name.empty();
}

std::string Customer::getName() const {
    return m_name;
}

std::string Customer::getPhone() const {
    return m_phone;
}

int Customer::getLoyaltyPoints() const {
    return m_loyaltyPoints;
}

void Customer::setId(int id) {
    m_id = id;
}

void Customer::setName(const std::string& name) {
    m_name = name;
}

void Customer::setPhone(const std::string& phone) {
    m_phone = phone;
}

void Customer::setLoyaltyPoints(int points) {
    if (points >= 0) {
        m_loyaltyPoints = points;
    }
}

void Customer::addPoints(int points) {
    if (points > 0) {
        m_loyaltyPoints += points;
    }
}

bool Customer::redeemPoints(int points) {
    if (canRedeemPoints(points)) {
        m_loyaltyPoints -= points;
        return true;
    }
    return false;
}

int Customer::calculatePointsForAmount(double amount) {
    if (amount <= 0) {
        return 0;
    }
    // 1 point per 1,000 IDR (integer division)
    return static_cast<int>(amount / POINTS_PER_UNIT);
}

double Customer::calculatePointsValue(int points) {
    if (points <= 0) {
        return 0.0;
    }
    // Each point is worth 100 IDR
    return points * POINT_VALUE;
}

bool Customer::canRedeemPoints(int points) const {
    return points >= MIN_REDEEM_POINTS && points <= m_loyaltyPoints;
}
