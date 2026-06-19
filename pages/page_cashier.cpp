#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "restaurantapp.h"

// 5. KASIR PAGE
void RestaurantApp::createCashierPage() {
    cashierPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(cashierPage);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel* title = new QLabel("Dashboard Kasir - Antrean Pembayaran");
    title->setObjectName("pageHeader");
    cashierListWidget = new QListWidget;

    QPushButton* btnPay = new QPushButton("Proses Pembayaran (Kirim ke Dapur)");
    QPushButton* btnHome = new QPushButton("Kembali ke Beranda");

    btnPay->setObjectName("primaryBtn");
    btnHome->setObjectName("secondaryBtn");

    layout->addWidget(title);
    layout->addWidget(cashierListWidget);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(15);
    btnLayout->addWidget(btnHome);
    btnLayout->addStretch();
    btnLayout->addWidget(btnPay);
    layout->addLayout(btnLayout);

    connect(btnPay, &QPushButton::clicked, this, [=]() {
        int row = cashierListWidget->currentRow();
        if (row >= 0 && row < cashierOrders.size()) {
            Order ord = cashierOrders.takeAt(row);
            ord.status = "Dibayar & Masak";
            kitchenQueue.enqueue(ord);  // Masukkan ke QUEUE Dapur

            QMessageBox::information(
                this, "Lunas",
                QString("Order #%1 telah dibayar dan dikirim ke dapur.")
                    .arg(ord.orderId));
            updateCashierUI();
        } else {
            QMessageBox::warning(this, "Pilih",
                                 "Pilih pesanan yang ingin dibayar.");
        }
    });
    connect(btnHome, &QPushButton::clicked, this,
            [=]() { stackedWidget->setCurrentIndex(0); });
}

void RestaurantApp::updateCashierUI() {
    cashierListWidget->clear();
    for (const auto& ord : cashierOrders) {
        cashierListWidget->addItem(QString("Order #%1 - %2 items - %3")
                                       .arg(ord.orderId)
                                       .arg(ord.items.size())
                                       .arg(ord.status));
    }
}
