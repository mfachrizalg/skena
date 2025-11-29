/**
 * @file TransactionController.cpp
 * @brief Implementation of TransactionController class
 */

#include "TransactionController.h"
#include "../utils/IdGenerator.h"
#include <algorithm>

TransactionController::TransactionController(FileManager& fileManager,
                                             ProductController& productController,
                                             CustomerController& customerController)
    : m_fileManager(fileManager)
    , m_productController(productController)
    , m_customerController(customerController)
{
}

std::vector<Transaction> TransactionController::getAll() const {
    return m_transactions;
}

Transaction* TransactionController::getById(int id) {
    auto it = std::find_if(m_transactions.begin(), m_transactions.end(),
        [id](const Transaction& t) {
            return t.getId() == id;
        });

    if (it != m_transactions.end()) {
        return &(*it);
    }
    return nullptr;
}

bool TransactionController::add(const Transaction& transaction) {
    if (!transaction.isValid()) {
        return false;
    }

    IdGenerator::getInstance().updateCounter("transaction", transaction.getId());
    m_transactions.push_back(transaction);
    return true;
}

bool TransactionController::update(const Transaction& transaction) {
    Transaction* existing = getById(transaction.getId());
    if (!existing) {
        return false;
    }

    *existing = transaction;
    return true;
}

bool TransactionController::remove(int id) {
    auto it = std::find_if(m_transactions.begin(), m_transactions.end(),
        [id](const Transaction& t) {
            return t.getId() == id;
        });

    if (it != m_transactions.end()) {
        m_transactions.erase(it);
        return true;
    }
    return false;
}

bool TransactionController::saveToFile() {
    std::vector<std::string> lines;
    lines.reserve(m_transactions.size());

    for (const auto& transaction : m_transactions) {
        lines.push_back(transaction.serialize());
    }

    return m_fileManager.writeLines(FILENAME, lines);
}

bool TransactionController::loadFromFile() {
    m_transactions.clear();

    std::vector<std::string> lines = m_fileManager.readLines(FILENAME);

    for (const auto& line : lines) {
        if (line.empty()) continue;

        Transaction transaction;
        transaction.deserialize(line);

        if (transaction.isValid()) {
            // Populate product names and prices for items
            for (auto& item : const_cast<std::vector<TransactionItem>&>(transaction.getItems())) {
                Product* product = m_productController.getById(item.getProductId());
                if (product) {
                    item.setProductName(product->getName());
                    item.setUnitPrice(product->getPrice());
                }
            }

            IdGenerator::getInstance().updateCounter("transaction", transaction.getId());
            m_transactions.push_back(transaction);
        }
    }

    return true;
}

int TransactionController::getCount() const {
    return static_cast<int>(m_transactions.size());
}

void TransactionController::startNewTransaction(int customerId) {
    m_currentTransaction = Transaction();
    m_currentTransaction.setCustomerId(customerId);
    m_currentTransaction.setCurrentDateTime();
}

Transaction& TransactionController::getCurrentTransaction() {
    return m_currentTransaction;
}

const Transaction& TransactionController::getCurrentTransaction() const {
    return m_currentTransaction;
}

bool TransactionController::addToCart(int productId, int quantity) {
    Product* product = m_productController.getById(productId);
    if (!product || quantity <= 0) {
        return false;
    }

    TransactionItem item(productId, product->getName(), product->getPrice(), quantity);
    m_currentTransaction.addItem(item);
    return true;
}

bool TransactionController::removeFromCart(int productId) {
    return m_currentTransaction.removeItem(productId);
}

bool TransactionController::updateCartQuantity(int productId, int quantity) {
    return m_currentTransaction.updateItemQuantity(productId, quantity);
}

void TransactionController::clearCart() {
    m_currentTransaction.clearItems();
}

void TransactionController::setCurrentCustomer(int customerId) {
    m_currentTransaction.setCustomerId(customerId);
    // Reset points usage when customer changes
    m_currentTransaction.setPointsUsed(0);
}

bool TransactionController::setPointsToUse(int points) {
    int customerId = m_currentTransaction.getCustomerId();
    if (customerId <= 0) {
        return false;
    }

    Customer* customer = m_customerController.getById(customerId);
    if (!customer) {
        return false;
    }

    // Check if customer has enough points
    if (points > 0 && !customer->canRedeemPoints(points)) {
        return false;
    }

    m_currentTransaction.setPointsUsed(points);
    return true;
}

bool TransactionController::completeTransaction() {
    if (!m_currentTransaction.hasItems()) {
        return false;
    }

    // Generate ID
    int id = IdGenerator::getInstance().getNextId("transaction");
    m_currentTransaction.setId(id);
    m_currentTransaction.setCurrentDateTime();
    m_currentTransaction.recalculate();

    int customerId = m_currentTransaction.getCustomerId();
    int pointsUsed = m_currentTransaction.getPointsUsed();
    int pointsEarned = m_currentTransaction.getPointsEarned();

    // Handle loyalty points
    if (customerId > 0) {
        // Deduct used points
        if (pointsUsed > 0) {
            if (!m_customerController.redeemLoyaltyPoints(customerId, pointsUsed)) {
                return false;  // Failed to redeem points
            }
        }

        // Add earned points
        if (pointsEarned > 0) {
            m_customerController.addLoyaltyPoints(customerId, pointsEarned);
        }
    }

    // Add to history
    m_transactions.push_back(m_currentTransaction);

    // Clear cart for next transaction
    startNewTransaction(0);

    return true;
}

void TransactionController::cancelTransaction() {
    m_currentTransaction = Transaction();
}

bool TransactionController::hasItemsInCart() const {
    return m_currentTransaction.hasItems();
}

std::vector<Transaction*> TransactionController::getByCustomerId(int customerId) {
    std::vector<Transaction*> result;

    for (auto& transaction : m_transactions) {
        if (transaction.getCustomerId() == customerId) {
            result.push_back(&transaction);
        }
    }

    return result;
}

std::vector<Transaction*> TransactionController::getByDateRange(const std::string& startDate,
                                                                 const std::string& endDate) {
    std::vector<Transaction*> result;

    for (auto& transaction : m_transactions) {
        std::string date = transaction.getDateTime().substr(0, 10);  // YYYY-MM-DD
        if (date >= startDate && date <= endDate) {
            result.push_back(&transaction);
        }
    }

    return result;
}

double TransactionController::getTotalRevenue() const {
    double total = 0.0;
    for (const auto& transaction : m_transactions) {
        total += transaction.getTotal();
    }
    return total;
}

std::vector<Transaction*> TransactionController::getRecent(int count) {
    std::vector<Transaction*> result;

    int start = std::max(0, static_cast<int>(m_transactions.size()) - count);

    for (int i = static_cast<int>(m_transactions.size()) - 1; i >= start; --i) {
        result.push_back(&m_transactions[i]);
    }

    return result;
}
