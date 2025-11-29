/**
 * @file ProductView.cpp
 * @brief Implementation of ProductView class
 */

#include "ProductView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>

ProductView::ProductView(ProductController* controller, QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
    , m_selectedProductId(-1)
{
    setupUI();
    connectSignals();
    refreshTable();
}

void ProductView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Filter section
    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Filter by Type:"));
    m_typeFilter = new QComboBox();
    m_typeFilter->addItem("All Products", "all");
    m_typeFilter->addItem("Coffee", "coffee");
    m_typeFilter->addItem("Snacks", "snack");
    filterLayout->addWidget(m_typeFilter);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    // Table
    m_productTable = new QTableWidget();
    m_productTable->setColumnCount(5);
    m_productTable->setHorizontalHeaderLabels({"ID", "Name", "Price (IDR)", "Type", "Details"});
    m_productTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_productTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_productTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_productTable->horizontalHeader()->setStretchLastSection(true);
    m_productTable->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_productTable);

    // Form section
    QGroupBox* formGroup = new QGroupBox("Product Details");
    QFormLayout* formLayout = new QFormLayout(formGroup);

    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("Enter product name");
    formLayout->addRow("Name:", m_nameEdit);

    m_priceSpinBox = new QDoubleSpinBox();
    m_priceSpinBox->setRange(0, 10000000);
    m_priceSpinBox->setSingleStep(1000);
    m_priceSpinBox->setPrefix("Rp ");
    formLayout->addRow("Price:", m_priceSpinBox);

    m_typeComboBox = new QComboBox();
    m_typeComboBox->addItem("Coffee", "coffee");
    m_typeComboBox->addItem("Snack", "snack");
    formLayout->addRow("Type:", m_typeComboBox);

    m_extraFieldLabel = new QLabel("Shot Size:");
    m_extraFieldEdit = new QLineEdit();
    m_extraFieldEdit->setPlaceholderText("single / double");
    formLayout->addRow(m_extraFieldLabel, m_extraFieldEdit);

    mainLayout->addWidget(formGroup);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_addButton = new QPushButton("Add Product");
    m_addButton->setStyleSheet("background-color: #4CAF50; color: white;");
    buttonLayout->addWidget(m_addButton);

    m_editButton = new QPushButton("Update Product");
    m_editButton->setStyleSheet("background-color: #2196F3; color: white;");
    m_editButton->setEnabled(false);
    buttonLayout->addWidget(m_editButton);

    m_deleteButton = new QPushButton("Delete Product");
    m_deleteButton->setStyleSheet("background-color: #f44336; color: white;");
    m_deleteButton->setEnabled(false);
    buttonLayout->addWidget(m_deleteButton);

    m_clearButton = new QPushButton("Clear Form");
    buttonLayout->addWidget(m_clearButton);

    mainLayout->addLayout(buttonLayout);
}

void ProductView::connectSignals() {
    connect(m_addButton, &QPushButton::clicked, this, &ProductView::onAddProduct);
    connect(m_editButton, &QPushButton::clicked, this, &ProductView::onEditProduct);
    connect(m_deleteButton, &QPushButton::clicked, this, &ProductView::onDeleteProduct);
    connect(m_clearButton, &QPushButton::clicked, this, &ProductView::onClearForm);
    connect(m_typeFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProductView::onFilterChanged);
    connect(m_typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProductView::onTypeChanged);
    connect(m_productTable, &QTableWidget::itemSelectionChanged,
            this, &ProductView::onTableRowSelected);
}

void ProductView::updateExtraFieldLabel() {
    QString type = m_typeComboBox->currentData().toString();
    if (type == "coffee") {
        m_extraFieldLabel->setText("Shot Size:");
        m_extraFieldEdit->setPlaceholderText("single / double");
    } else {
        m_extraFieldLabel->setText("Category:");
        m_extraFieldEdit->setPlaceholderText("pastry / sandwich / other");
    }
}

void ProductView::clearForm() {
    m_nameEdit->clear();
    m_priceSpinBox->setValue(0);
    m_typeComboBox->setCurrentIndex(0);
    m_extraFieldEdit->clear();
    m_selectedProductId = -1;
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_productTable->clearSelection();
}

void ProductView::populateForm(const Product* product) {
    if (!product) return;

    m_nameEdit->setText(QString::fromStdString(product->getName()));
    m_priceSpinBox->setValue(product->getPrice());

    int typeIndex = m_typeComboBox->findData(QString::fromStdString(product->getType()));
    if (typeIndex >= 0) {
        m_typeComboBox->setCurrentIndex(typeIndex);
    }

    m_extraFieldEdit->setText(QString::fromStdString(product->getExtraField()));
    m_selectedProductId = product->getId();
    m_editButton->setEnabled(true);
    m_deleteButton->setEnabled(true);
}

void ProductView::refreshTable() {
    m_productTable->setRowCount(0);

    QString filterType = m_typeFilter->currentData().toString();
    std::vector<Product*> products;

    if (filterType == "all") {
        products = m_controller->getAll();
    } else {
        products = m_controller->getByType(filterType.toStdString());
    }

    for (const auto* product : products) {
        int row = m_productTable->rowCount();
        m_productTable->insertRow(row);

        m_productTable->setItem(row, 0, new QTableWidgetItem(QString::number(product->getId())));
        m_productTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(product->getName())));
        m_productTable->setItem(row, 2, new QTableWidgetItem(QString::number(product->getPrice(), 'f', 0)));
        m_productTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(product->getType())));
        m_productTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(product->getExtraField())));
    }

    m_productTable->resizeColumnsToContents();
}

void ProductView::onAddProduct() {
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a product name.");
        return;
    }

    double price = m_priceSpinBox->value();
    QString type = m_typeComboBox->currentData().toString();
    QString extraField = m_extraFieldEdit->text().trimmed();

    if (extraField.isEmpty()) {
        extraField = (type == "coffee") ? "single" : "other";
    }

    std::unique_ptr<Product> product;
    if (type == "coffee") {
        product = m_controller->createCoffee(name.toStdString(), price, extraField.toStdString());
    } else {
        product = m_controller->createSnack(name.toStdString(), price, extraField.toStdString());
    }

    if (m_controller->add(std::move(product))) {
        refreshTable();
        clearForm();
        emit dataChanged();
        QMessageBox::information(this, "Success", "Product added successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to add product.");
    }
}

void ProductView::onEditProduct() {
    if (m_selectedProductId < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a product to edit.");
        return;
    }

    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a product name.");
        return;
    }

    Product* product = m_controller->getById(m_selectedProductId);
    if (!product) {
        QMessageBox::warning(this, "Error", "Product not found.");
        return;
    }

    product->setName(name.toStdString());
    product->setPrice(m_priceSpinBox->value());
    product->setExtraField(m_extraFieldEdit->text().toStdString());

    refreshTable();
    clearForm();
    emit dataChanged();
    QMessageBox::information(this, "Success", "Product updated successfully!");
}

void ProductView::onDeleteProduct() {
    if (m_selectedProductId < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a product to delete.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete",
        "Are you sure you want to delete this product?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (m_controller->remove(m_selectedProductId)) {
            refreshTable();
            clearForm();
            emit dataChanged();
            QMessageBox::information(this, "Success", "Product deleted successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete product.");
        }
    }
}

void ProductView::onClearForm() {
    clearForm();
}

void ProductView::onTableRowSelected() {
    QList<QTableWidgetItem*> selectedItems = m_productTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    int row = selectedItems.first()->row();
    int productId = m_productTable->item(row, 0)->text().toInt();

    Product* product = m_controller->getById(productId);
    if (product) {
        populateForm(product);
    }
}

void ProductView::onFilterChanged() {
    refreshTable();
}

void ProductView::onTypeChanged() {
    updateExtraFieldLabel();
}
