/**
 * @file ProductController.cpp
 * @brief Implementation of ProductController class
 */

#include "ProductController.h"
#include "../models/Coffee.h"
#include "../models/Snack.h"
#include "../utils/IdGenerator.h"
#include <algorithm>

ProductController::ProductController(FileManager& fileManager)
    : m_fileManager(fileManager)
{
}

std::vector<Product*> ProductController::getAll() const {
    std::vector<Product*> result;
    result.reserve(m_products.size());

    for (const auto& product : m_products) {
        result.push_back(product.get());
    }

    return result;
}

Product* ProductController::getById(int id) {
    auto it = std::find_if(m_products.begin(), m_products.end(),
        [id](const std::unique_ptr<Product>& p) {
            return p->getId() == id;
        });

    if (it != m_products.end()) {
        return it->get();
    }
    return nullptr;
}

std::vector<Product*> ProductController::getByType(const std::string& type) const {
    std::vector<Product*> result;

    for (const auto& product : m_products) {
        if (product->getType() == type) {
            result.push_back(product.get());
        }
    }

    return result;
}

bool ProductController::add(std::unique_ptr<Product> product) {
    if (!product || !product->isValid()) {
        return false;
    }

    // Update ID generator with this ID
    IdGenerator::getInstance().updateCounter("product", product->getId());

    m_products.push_back(std::move(product));
    return true;
}

bool ProductController::update(const Product& product) {
    Product* existing = getById(product.getId());
    if (!existing) {
        return false;
    }

    existing->setName(product.getName());
    existing->setPrice(product.getPrice());
    existing->setExtraField(product.getExtraField());

    return true;
}

bool ProductController::remove(int id) {
    auto it = std::find_if(m_products.begin(), m_products.end(),
        [id](const std::unique_ptr<Product>& p) {
            return p->getId() == id;
        });

    if (it != m_products.end()) {
        m_products.erase(it);
        return true;
    }
    return false;
}

std::unique_ptr<Product> ProductController::createCoffee(const std::string& name, double price,
                                                          const std::string& shotSize) {
    int id = IdGenerator::getInstance().getNextId("product");
    return std::make_unique<Coffee>(id, name, price, shotSize);
}

std::unique_ptr<Product> ProductController::createSnack(const std::string& name, double price,
                                                         const std::string& category) {
    int id = IdGenerator::getInstance().getNextId("product");
    return std::make_unique<Snack>(id, name, price, category);
}

bool ProductController::saveToFile() {
    std::vector<std::string> lines;
    lines.reserve(m_products.size());

    for (const auto& product : m_products) {
        lines.push_back(product->serialize());
    }

    return m_fileManager.writeLines(FILENAME, lines);
}

bool ProductController::loadFromFile() {
    m_products.clear();

    std::vector<std::string> lines = m_fileManager.readLines(FILENAME);

    for (const auto& line : lines) {
        if (line.empty()) continue;

        // Parse type from the line to determine product type
        std::vector<std::string> fields = FileManager::splitLine(line, '|');
        if (fields.size() < 4) continue;

        std::string type = fields[3];
        std::unique_ptr<Product> product;

        if (type == "coffee") {
            product = std::make_unique<Coffee>();
        } else if (type == "snack") {
            product = std::make_unique<Snack>();
        } else {
            continue;  // Unknown type
        }

        product->deserialize(line);

        if (product->isValid()) {
            // Update ID generator
            IdGenerator::getInstance().updateCounter("product", product->getId());
            m_products.push_back(std::move(product));
        }
    }

    return true;
}

int ProductController::getCount() const {
    return static_cast<int>(m_products.size());
}

int ProductController::getCountByType(const std::string& type) const {
    return static_cast<int>(std::count_if(m_products.begin(), m_products.end(),
        [&type](const std::unique_ptr<Product>& p) {
            return p->getType() == type;
        }));
}
