/**
 * @file TransactionView.cpp
 * @brief Implementation of TransactionView class
 */

#include "TransactionView.h"
#include "../models/Customer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>

TransactionView::TransactionView(TransactionController* transactionController,
                                 ProductController* productController,
                                 CustomerController* customerController,
                                 QWidget* parent)
    : QWidget(parent)
    , m_transactionController(transactionController)
    , m_productController(productController)
    , m_customerController(customerController)
{
    setupUI();
    connectSignals();
    refreshAll();
    m_transactionController->startNewTransaction(0);
}

void TransactionView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Create tabbed interface for POS and History
    QTabWidget* tabWidget = new QTabWidget();

    // POS Tab
    QWidget* posTab = new QWidget();
    QHBoxLayout* posLayout = new QHBoxLayout(posTab);

    // Add panels
    posLayout->addWidget(createProductPanel(), 1);
    posLayout->addWidget(createCartPanel(), 2);
    posLayout->addWidget(createPaymentPanel(), 1);

    tabWidget->addTab(posTab, "Point of Sale");

    // History Tab
    tabWidget->addTab(createHistoryPanel(), "Transaction History");

    mainLayout->addWidget(tabWidget);
}

QWidget* TransactionView::createProductPanel() {
    QWidget* panel = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(panel);

    // Coffee section
    QGroupBox* coffeeGroup = new QGroupBox("Coffee");
    QVBoxLayout* coffeeLayout = new QVBoxLayout(coffeeGroup);
    m_coffeeList = new QListWidget();
    m_coffeeList->setToolTip("Double-click to add to cart");
    coffeeLayout->addWidget(m_coffeeList);
    layout->addWidget(coffeeGroup);

    // Snacks section
    QGroupBox* snackGroup = new QGroupBox("Snacks");
    QVBoxLayout* snackLayout = new QVBoxLayout(snackGroup);
    m_snackList = new QListWidget();
    m_snackList->setToolTip("Double-click to add to cart");
    snackLayout->addWidget(m_snackList);
    layout->addWidget(snackGroup);

    return panel;
}

QWidget* TransactionView::createCartPanel() {
    QWidget* panel = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(panel);

    QGroupBox* cartGroup = new QGroupBox("Shopping Cart");
    QVBoxLayout* cartLayout = new QVBoxLayout(cartGroup);

    // Cart table
    m_cartTable = new QTableWidget();
    m_cartTable->setColumnCount(5);
    m_cartTable->setHorizontalHeaderLabels({"ID", "Product", "Price", "Qty", "Subtotal"});
    m_cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_cartTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_cartTable->horizontalHeader()->setStretchLastSection(true);
    m_cartTable->verticalHeader()->setVisible(false);
    m_cartTable->setColumnHidden(0, true);  // Hide ID column
    cartLayout->addWidget(m_cartTable);

    // Remove button
    m_removeItemButton = new QPushButton("Remove Selected Item");
    m_removeItemButton->setStyleSheet("background-color: #f44336; color: white;");
    cartLayout->addWidget(m_removeItemButton);

    layout->addWidget(cartGroup);

    // Totals section
    QGroupBox* totalsGroup = new QGroupBox("Order Summary");
    QFormLayout* totalsLayout = new QFormLayout(totalsGroup);

    m_subtotalLabel = new QLabel("Rp 0");
    m_subtotalLabel->setStyleSheet("font-size: 14px;");
    totalsLayout->addRow("Subtotal:", m_subtotalLabel);

    m_discountLabel = new QLabel("Rp 0");
    m_discountLabel->setStyleSheet("font-size: 14px; color: green;");
    totalsLayout->addRow("Discount (Points):", m_discountLabel);

    m_totalLabel = new QLabel("Rp 0");
    m_totalLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    totalsLayout->addRow("TOTAL:", m_totalLabel);

    layout->addWidget(totalsGroup);

    return panel;
}

QWidget* TransactionView::createPaymentPanel() {
    QWidget* panel = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(panel);

    // Customer selection
    QGroupBox* customerGroup = new QGroupBox("Customer");
    QVBoxLayout* customerLayout = new QVBoxLayout(customerGroup);

    m_customerComboBox = new QComboBox();
    customerLayout->addWidget(m_customerComboBox);

    m_customerPointsLabel = new QLabel("Available Points: 0");
    m_customerPointsLabel->setStyleSheet("font-size: 12px; color: #666;");
    customerLayout->addWidget(m_customerPointsLabel);

    layout->addWidget(customerGroup);

    // Points redemption
    QGroupBox* pointsGroup = new QGroupBox("Use Loyalty Points");
    QFormLayout* pointsLayout = new QFormLayout(pointsGroup);

    m_usePointsSpinBox = new QSpinBox();
    m_usePointsSpinBox->setRange(0, 0);
    m_usePointsSpinBox->setSingleStep(10);
    pointsLayout->addRow("Points to Use:", m_usePointsSpinBox);

    m_pointsValueLabel = new QLabel("= Rp 0 discount");
    m_pointsValueLabel->setStyleSheet("color: green;");
    pointsLayout->addRow("", m_pointsValueLabel);

    layout->addWidget(pointsGroup);

    // Action buttons
    layout->addStretch();

    m_completeButton = new QPushButton("Complete Order");
    m_completeButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 10px;");
    m_completeButton->setMinimumHeight(50);
    layout->addWidget(m_completeButton);

    m_cancelButton = new QPushButton("Cancel Order");
    m_cancelButton->setStyleSheet("background-color: #f44336; color: white;");
    layout->addWidget(m_cancelButton);

    return panel;
}

QWidget* TransactionView::createHistoryPanel() {
    QWidget* panel = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(panel);

    m_historyTable = new QTableWidget();
    m_historyTable->setColumnCount(6);
    m_historyTable->setHorizontalHeaderLabels({"ID", "Date/Time", "Customer", "Items", "Total", "Points Earned"});
    m_historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->horizontalHeader()->setStretchLastSection(true);
    m_historyTable->verticalHeader()->setVisible(false);
    layout->addWidget(m_historyTable);

    return panel;
}

void TransactionView::connectSignals() {
    connect(m_coffeeList, &QListWidget::itemDoubleClicked,
            this, &TransactionView::onCoffeeItemDoubleClicked);
    connect(m_snackList, &QListWidget::itemDoubleClicked,
            this, &TransactionView::onSnackItemDoubleClicked);
    connect(m_removeItemButton, &QPushButton::clicked,
            this, &TransactionView::onRemoveFromCart);
    connect(m_customerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionView::onCustomerChanged);
    connect(m_usePointsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TransactionView::onPointsUsageChanged);
    connect(m_completeButton, &QPushButton::clicked,
            this, &TransactionView::onCompleteTransaction);
    connect(m_cancelButton, &QPushButton::clicked,
            this, &TransactionView::onCancelTransaction);
}

void TransactionView::updateTotals() {
    const Transaction& trans = m_transactionController->getCurrentTransaction();

    m_subtotalLabel->setText(QString("Rp %1").arg(trans.getSubtotal(), 0, 'f', 0));
    m_discountLabel->setText(QString("- Rp %1").arg(trans.getDiscount(), 0, 'f', 0));
    m_totalLabel->setText(QString("Rp %1").arg(trans.getTotal(), 0, 'f', 0));
}

void TransactionView::updatePointsValue() {
    int points = m_usePointsSpinBox->value();
    double value = Customer::calculatePointsValue(points);
    m_pointsValueLabel->setText(QString("= Rp %1 discount").arg(value, 0, 'f', 0));
}

void TransactionView::refreshProductLists() {
    // Refresh coffee list
    m_coffeeList->clear();
    std::vector<Product*> coffees = m_productController->getByType("coffee");
    for (const auto* product : coffees) {
        QListWidgetItem* item = new QListWidgetItem(
            QString("%1 - Rp %2")
                .arg(QString::fromStdString(product->getName()))
                .arg(product->getPrice(), 0, 'f', 0)
        );
        item->setData(Qt::UserRole, product->getId());
        m_coffeeList->addItem(item);
    }

    // Refresh snack list
    m_snackList->clear();
    std::vector<Product*> snacks = m_productController->getByType("snack");
    for (const auto* product : snacks) {
        QListWidgetItem* item = new QListWidgetItem(
            QString("%1 - Rp %2")
                .arg(QString::fromStdString(product->getName()))
                .arg(product->getPrice(), 0, 'f', 0)
        );
        item->setData(Qt::UserRole, product->getId());
        m_snackList->addItem(item);
    }
}

void TransactionView::refreshCustomerList() {
    m_customerComboBox->clear();
    m_customerComboBox->addItem("Guest (No Loyalty)", 0);

    std::vector<Customer> customers = m_customerController->getAll();
    for (const auto& customer : customers) {
        m_customerComboBox->addItem(
            QString("%1 (%2 pts)")
                .arg(QString::fromStdString(customer.getName()))
                .arg(customer.getLoyaltyPoints()),
            customer.getId()
        );
    }
}

void TransactionView::refreshCart() {
    m_cartTable->setRowCount(0);

    const Transaction& trans = m_transactionController->getCurrentTransaction();
    const std::vector<TransactionItem>& items = trans.getItems();

    for (const auto& item : items) {
        int row = m_cartTable->rowCount();
        m_cartTable->insertRow(row);

        m_cartTable->setItem(row, 0, new QTableWidgetItem(QString::number(item.getProductId())));
        m_cartTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(item.getProductName())));
        m_cartTable->setItem(row, 2, new QTableWidgetItem(QString("Rp %1").arg(item.getUnitPrice(), 0, 'f', 0)));

        QSpinBox* qtySpinBox = new QSpinBox();
        qtySpinBox->setRange(1, 99);
        qtySpinBox->setValue(item.getQuantity());
        connect(qtySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this, row](int value) {
            onCartQuantityChanged(row, value);
        });
        m_cartTable->setCellWidget(row, 3, qtySpinBox);

        m_cartTable->setItem(row, 4, new QTableWidgetItem(QString("Rp %1").arg(item.getSubtotal(), 0, 'f', 0)));
    }

    m_cartTable->resizeColumnsToContents();
    updateTotals();
}

void TransactionView::refreshHistory() {
    m_historyTable->setRowCount(0);

    std::vector<Transaction*> transactions = m_transactionController->getRecent(50);

    for (const auto* trans : transactions) {
        int row = m_historyTable->rowCount();
        m_historyTable->insertRow(row);

        m_historyTable->setItem(row, 0, new QTableWidgetItem(QString::number(trans->getId())));
        m_historyTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(trans->getDateTime())));

        // Get customer name
        QString customerName = "Guest";
        if (trans->getCustomerId() > 0) {
            Customer* customer = m_customerController->getById(trans->getCustomerId());
            if (customer) {
                customerName = QString::fromStdString(customer->getName());
            }
        }
        m_historyTable->setItem(row, 2, new QTableWidgetItem(customerName));

        m_historyTable->setItem(row, 3, new QTableWidgetItem(QString::number(trans->getItemCount())));
        m_historyTable->setItem(row, 4, new QTableWidgetItem(QString("Rp %1").arg(trans->getTotal(), 0, 'f', 0)));
        m_historyTable->setItem(row, 5, new QTableWidgetItem(QString::number(trans->getPointsEarned())));
    }

    m_historyTable->resizeColumnsToContents();
}

void TransactionView::refreshAll() {
    refreshProductLists();
    refreshCustomerList();
    refreshCart();
    refreshHistory();
}

void TransactionView::onCoffeeItemDoubleClicked(QListWidgetItem* item) {
    int productId = item->data(Qt::UserRole).toInt();
    m_transactionController->addToCart(productId, 1);
    refreshCart();
}

void TransactionView::onSnackItemDoubleClicked(QListWidgetItem* item) {
    int productId = item->data(Qt::UserRole).toInt();
    m_transactionController->addToCart(productId, 1);
    refreshCart();
}

void TransactionView::onRemoveFromCart() {
    QList<QTableWidgetItem*> selectedItems = m_cartTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to remove.");
        return;
    }

    int row = selectedItems.first()->row();
    int productId = m_cartTable->item(row, 0)->text().toInt();

    m_transactionController->removeFromCart(productId);
    refreshCart();
}

void TransactionView::onCartQuantityChanged(int row, int quantity) {
    int productId = m_cartTable->item(row, 0)->text().toInt();
    m_transactionController->updateCartQuantity(productId, quantity);

    // Update subtotal for this row
    TransactionItem* item = m_transactionController->getCurrentTransaction().getItem(productId);
    if (item) {
        m_cartTable->item(row, 4)->setText(QString("Rp %1").arg(item->getSubtotal(), 0, 'f', 0));
    }

    updateTotals();
}

void TransactionView::onCustomerChanged() {
    int customerId = m_customerComboBox->currentData().toInt();
    m_transactionController->setCurrentCustomer(customerId);

    // Update available points
    if (customerId > 0) {
        Customer* customer = m_customerController->getById(customerId);
        if (customer) {
            int points = customer->getLoyaltyPoints();
            m_customerPointsLabel->setText(QString("Available Points: %1").arg(points));
            m_usePointsSpinBox->setMaximum(points);
            m_usePointsSpinBox->setEnabled(points >= Customer::MIN_REDEEM_POINTS);
        }
    } else {
        m_customerPointsLabel->setText("Available Points: 0");
        m_usePointsSpinBox->setMaximum(0);
        m_usePointsSpinBox->setValue(0);
        m_usePointsSpinBox->setEnabled(false);
    }

    updateTotals();
}

void TransactionView::onPointsUsageChanged() {
    int points = m_usePointsSpinBox->value();

    // Validate minimum points
    if (points > 0 && points < Customer::MIN_REDEEM_POINTS) {
        m_usePointsSpinBox->setValue(0);
        return;
    }

    m_transactionController->setPointsToUse(points);
    updatePointsValue();
    updateTotals();
}

void TransactionView::onCompleteTransaction() {
    if (!m_transactionController->hasItemsInCart()) {
        QMessageBox::warning(this, "Empty Cart", "Please add items to the cart before completing.");
        return;
    }

    // Confirm transaction
    const Transaction& trans = m_transactionController->getCurrentTransaction();
    QString message = QString("Complete this order?\n\n"
                              "Total: Rp %1\n"
                              "Points to Use: %2\n"
                              "Points to Earn: %3")
                          .arg(trans.getTotal(), 0, 'f', 0)
                          .arg(trans.getPointsUsed())
                          .arg(trans.getPointsEarned());

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Order", message,
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (m_transactionController->completeTransaction()) {
            refreshCart();
            refreshHistory();
            refreshCustomerList();  // Update points display
            m_customerComboBox->setCurrentIndex(0);  // Reset to Guest
            m_usePointsSpinBox->setValue(0);

            emit transactionCompleted();

            QMessageBox::information(this, "Success", "Transaction completed successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to complete transaction.");
        }
    }
}

void TransactionView::onCancelTransaction() {
    if (!m_transactionController->hasItemsInCart()) {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Cancel Order",
        "Are you sure you want to cancel this order?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_transactionController->cancelTransaction();
        m_transactionController->startNewTransaction(0);
        refreshCart();
        m_customerComboBox->setCurrentIndex(0);
        m_usePointsSpinBox->setValue(0);
    }
}
