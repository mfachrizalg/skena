/**
 * @file CustomerController.cpp
 * @brief Implementation of CustomerController class
 */

#include "CustomerController.h"
#include "../utils/IdGenerator.h"
#include <algorithm>

CustomerController::CustomerController(FileManager& fileManager)
    : m_fileManager(fileManager)
{
}

std::vector<Customer> CustomerController::getAll() const {
    return m_customers;
}

Customer* CustomerController::getById(int id) {
    auto it = std::find_if(m_customers.begin(), m_customers.end(),
        [id](const Customer& c) {
            return c.getId() == id;
        });

    if (it != m_customers.end()) {
        return &(*it);
    }
    return nullptr;
}

bool CustomerController::add(const Customer& customer) {
    if (!customer.isValid()) {
        return false;
    }

    // Check for duplicate ID
    if (getById(customer.getId()) != nullptr) {
        return false;
    }

    // Update ID generator
    IdGenerator::getInstance().updateCounter("customer", customer.getId());

    m_customers.push_back(customer);
    return true;
}

bool CustomerController::update(const Customer& customer) {
    Customer* existing = getById(customer.getId());
    if (!existing) {
        return false;
    }

    existing->setName(customer.getName());
    existing->setPhone(customer.getPhone());
    existing->setLoyaltyPoints(customer.getLoyaltyPoints());

    return true;
}

bool CustomerController::remove(int id) {
    auto it = std::find_if(m_customers.begin(), m_customers.end(),
        [id](const Customer& c) {
            return c.getId() == id;
        });

    if (it != m_customers.end()) {
        m_customers.erase(it);
        return true;
    }
    return false;
}

bool CustomerController::saveToFile() {
    std::vector<std::string> lines;
    lines.reserve(m_customers.size());

    for (const auto& customer : m_customers) {
        lines.push_back(customer.serialize());
    }

    return m_fileManager.writeLines(FILENAME, lines);
}

bool CustomerController::loadFromFile() {
    m_customers.clear();

    std::vector<std::string> lines = m_fileManager.readLines(FILENAME);

    for (const auto& line : lines) {
        if (line.empty()) continue;

        Customer customer;
        customer.deserialize(line);

        if (customer.isValid()) {
            IdGenerator::getInstance().updateCounter("customer", customer.getId());
            m_customers.push_back(customer);
        }
    }

    return true;
}

int CustomerController::getCount() const {
    return static_cast<int>(m_customers.size());
}

Customer CustomerController::createCustomer(const std::string& name, const std::string& phone) {
    int id = IdGenerator::getInstance().getNextId("customer");
    return Customer(id, name, phone, 0);
}

std::vector<Customer*> CustomerController::searchByName(const std::string& searchTerm) {
    std::vector<Customer*> results;

    // Convert search term to lowercase for case-insensitive search
    std::string lowerSearch = searchTerm;
    std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

    for (auto& customer : m_customers) {
        std::string lowerName = customer.getName();
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        if (lowerName.find(lowerSearch) != std::string::npos) {
            results.push_back(&customer);
        }
    }

    return results;
}

Customer* CustomerController::getByPhone(const std::string& phone) {
    auto it = std::find_if(m_customers.begin(), m_customers.end(),
        [&phone](const Customer& c) {
            return c.getPhone() == phone;
        });

    if (it != m_customers.end()) {
        return &(*it);
    }
    return nullptr;
}

bool CustomerController::addLoyaltyPoints(int customerId, int points) {
    Customer* customer = getById(customerId);
    if (!customer || points <= 0) {
        return false;
    }

    customer->addPoints(points);
    return true;
}

bool CustomerController::redeemLoyaltyPoints(int customerId, int points) {
    Customer* customer = getById(customerId);
    if (!customer) {
        return false;
    }

    return customer->redeemPoints(points);
}

int CustomerController::getLoyaltyPoints(int customerId) const {
    auto it = std::find_if(m_customers.begin(), m_customers.end(),
        [customerId](const Customer& c) {
            return c.getId() == customerId;
        });

    if (it != m_customers.end()) {
        return it->getLoyaltyPoints();
    }
    return -1;
}
