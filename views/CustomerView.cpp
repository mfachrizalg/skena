/**
 * @file CustomerView.cpp
 * @brief Implementation of CustomerView class
 */

#include "CustomerView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>

CustomerView::CustomerView(CustomerController* controller, QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
    , m_selectedCustomerId(-1)
{
    setupUI();
    connectSignals();
    refreshTable();
}

void CustomerView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Search section
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:"));
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search by name...");
    searchLayout->addWidget(m_searchEdit);
    m_searchButton = new QPushButton("Search");
    searchLayout->addWidget(m_searchButton);
    QPushButton* showAllButton = new QPushButton("Show All");
    connect(showAllButton, &QPushButton::clicked, this, &CustomerView::refreshTable);
    searchLayout->addWidget(showAllButton);
    mainLayout->addLayout(searchLayout);

    // Table
    m_customerTable = new QTableWidget();
    m_customerTable->setColumnCount(4);
    m_customerTable->setHorizontalHeaderLabels({"ID", "Name", "Phone", "Loyalty Points"});
    m_customerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_customerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_customerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_customerTable->horizontalHeader()->setStretchLastSection(true);
    m_customerTable->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_customerTable);

    // Form section
    QGroupBox* formGroup = new QGroupBox("Customer Details");
    QFormLayout* formLayout = new QFormLayout(formGroup);

    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("Enter customer name");
    formLayout->addRow("Name:", m_nameEdit);

    m_phoneEdit = new QLineEdit();
    m_phoneEdit->setPlaceholderText("Enter phone number");
    formLayout->addRow("Phone:", m_phoneEdit);

    m_pointsSpinBox = new QSpinBox();
    m_pointsSpinBox->setRange(0, 999999);
    m_pointsSpinBox->setReadOnly(true);  // Points are managed through transactions
    formLayout->addRow("Loyalty Points:", m_pointsSpinBox);

    mainLayout->addWidget(formGroup);

    // Points management section
    QGroupBox* pointsGroup = new QGroupBox("Manual Points Management");
    QHBoxLayout* pointsLayout = new QHBoxLayout(pointsGroup);
    pointsLayout->addWidget(new QLabel("Add Points:"));
    m_addPointsSpinBox = new QSpinBox();
    m_addPointsSpinBox->setRange(1, 10000);
    m_addPointsSpinBox->setValue(100);
    pointsLayout->addWidget(m_addPointsSpinBox);
    m_addPointsButton = new QPushButton("Add Points");
    m_addPointsButton->setEnabled(false);
    pointsLayout->addWidget(m_addPointsButton);
    pointsLayout->addStretch();
    mainLayout->addWidget(pointsGroup);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_addButton = new QPushButton("Add Customer");
    m_addButton->setStyleSheet("background-color: #4CAF50; color: white;");
    buttonLayout->addWidget(m_addButton);

    m_editButton = new QPushButton("Update Customer");
    m_editButton->setStyleSheet("background-color: #2196F3; color: white;");
    m_editButton->setEnabled(false);
    buttonLayout->addWidget(m_editButton);

    m_deleteButton = new QPushButton("Delete Customer");
    m_deleteButton->setStyleSheet("background-color: #f44336; color: white;");
    m_deleteButton->setEnabled(false);
    buttonLayout->addWidget(m_deleteButton);

    m_clearButton = new QPushButton("Clear Form");
    buttonLayout->addWidget(m_clearButton);

    mainLayout->addLayout(buttonLayout);
}

void CustomerView::connectSignals() {
    connect(m_addButton, &QPushButton::clicked, this, &CustomerView::onAddCustomer);
    connect(m_editButton, &QPushButton::clicked, this, &CustomerView::onEditCustomer);
    connect(m_deleteButton, &QPushButton::clicked, this, &CustomerView::onDeleteCustomer);
    connect(m_clearButton, &QPushButton::clicked, this, &CustomerView::onClearForm);
    connect(m_searchButton, &QPushButton::clicked, this, &CustomerView::onSearch);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &CustomerView::onSearch);
    connect(m_addPointsButton, &QPushButton::clicked, this, &CustomerView::onAddPoints);
    connect(m_customerTable, &QTableWidget::itemSelectionChanged,
            this, &CustomerView::onTableRowSelected);
}

void CustomerView::clearForm() {
    m_nameEdit->clear();
    m_phoneEdit->clear();
    m_pointsSpinBox->setValue(0);
    m_selectedCustomerId = -1;
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_addPointsButton->setEnabled(false);
    m_customerTable->clearSelection();
}

void CustomerView::populateForm(const Customer* customer) {
    if (!customer) return;

    m_nameEdit->setText(QString::fromStdString(customer->getName()));
    m_phoneEdit->setText(QString::fromStdString(customer->getPhone()));
    m_pointsSpinBox->setValue(customer->getLoyaltyPoints());
    m_selectedCustomerId = customer->getId();
    m_editButton->setEnabled(true);
    m_deleteButton->setEnabled(true);
    m_addPointsButton->setEnabled(true);
}

void CustomerView::refreshTable() {
    m_customerTable->setRowCount(0);

    std::vector<Customer> customers = m_controller->getAll();

    for (const auto& customer : customers) {
        int row = m_customerTable->rowCount();
        m_customerTable->insertRow(row);

        m_customerTable->setItem(row, 0, new QTableWidgetItem(QString::number(customer.getId())));
        m_customerTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(customer.getName())));
        m_customerTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(customer.getPhone())));
        m_customerTable->setItem(row, 3, new QTableWidgetItem(QString::number(customer.getLoyaltyPoints())));
    }

    m_customerTable->resizeColumnsToContents();
    m_searchEdit->clear();
}

void CustomerView::onAddCustomer() {
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a customer name.");
        return;
    }

    QString phone = m_phoneEdit->text().trimmed();

    Customer customer = m_controller->createCustomer(name.toStdString(), phone.toStdString());

    if (m_controller->add(customer)) {
        refreshTable();
        clearForm();
        emit dataChanged();
        QMessageBox::information(this, "Success", "Customer added successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to add customer.");
    }
}

void CustomerView::onEditCustomer() {
    if (m_selectedCustomerId < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a customer to edit.");
        return;
    }

    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a customer name.");
        return;
    }

    Customer* customer = m_controller->getById(m_selectedCustomerId);
    if (!customer) {
        QMessageBox::warning(this, "Error", "Customer not found.");
        return;
    }

    customer->setName(name.toStdString());
    customer->setPhone(m_phoneEdit->text().toStdString());

    refreshTable();
    clearForm();
    emit dataChanged();
    QMessageBox::information(this, "Success", "Customer updated successfully!");
}

void CustomerView::onDeleteCustomer() {
    if (m_selectedCustomerId < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a customer to delete.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete",
        "Are you sure you want to delete this customer?\nThis will also delete their loyalty points.",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (m_controller->remove(m_selectedCustomerId)) {
            refreshTable();
            clearForm();
            emit dataChanged();
            QMessageBox::information(this, "Success", "Customer deleted successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete customer.");
        }
    }
}

void CustomerView::onClearForm() {
    clearForm();
}

void CustomerView::onTableRowSelected() {
    QList<QTableWidgetItem*> selectedItems = m_customerTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    int row = selectedItems.first()->row();
    int customerId = m_customerTable->item(row, 0)->text().toInt();

    Customer* customer = m_controller->getById(customerId);
    if (customer) {
        populateForm(customer);
    }
}

void CustomerView::onSearch() {
    QString searchTerm = m_searchEdit->text().trimmed();
    if (searchTerm.isEmpty()) {
        refreshTable();
        return;
    }

    m_customerTable->setRowCount(0);

    std::vector<Customer*> results = m_controller->searchByName(searchTerm.toStdString());

    for (const auto* customer : results) {
        int row = m_customerTable->rowCount();
        m_customerTable->insertRow(row);

        m_customerTable->setItem(row, 0, new QTableWidgetItem(QString::number(customer->getId())));
        m_customerTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(customer->getName())));
        m_customerTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(customer->getPhone())));
        m_customerTable->setItem(row, 3, new QTableWidgetItem(QString::number(customer->getLoyaltyPoints())));
    }

    m_customerTable->resizeColumnsToContents();
}

void CustomerView::onAddPoints() {
    if (m_selectedCustomerId < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a customer first.");
        return;
    }

    int points = m_addPointsSpinBox->value();
    if (m_controller->addLoyaltyPoints(m_selectedCustomerId, points)) {
        // Refresh the form to show updated points
        Customer* customer = m_controller->getById(m_selectedCustomerId);
        if (customer) {
            m_pointsSpinBox->setValue(customer->getLoyaltyPoints());
        }
        refreshTable();
        emit dataChanged();
        QMessageBox::information(this, "Success",
            QString("Added %1 points successfully!").arg(points));
    } else {
        QMessageBox::warning(this, "Error", "Failed to add points.");
    }
}
