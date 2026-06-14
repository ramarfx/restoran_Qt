#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>

// 7. WAITER PAGE
void RestaurantApp::createWaiterPage() {
  waiterPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(waiterPage);
  layout->setContentsMargins(30, 30, 30, 30);
  layout->setSpacing(20);

  QLabel *title = new QLabel("Dashboard Pelayan - Antrean Pengantaran");
  title->setObjectName("pageHeader");
  waiterListWidget = new QListWidget;

  QPushButton *btnDeliver = new QPushButton("Antar ke Meja / Selesai");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  btnDeliver->setObjectName("primaryBtn");
  btnHome->setObjectName("secondaryBtn");

  layout->addWidget(title);
  layout->addWidget(waiterListWidget);

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->setSpacing(15);
  btnLayout->addWidget(btnHome);
  btnLayout->addStretch();
  btnLayout->addWidget(btnDeliver);
  layout->addLayout(btnLayout);

  connect(btnDeliver, &QPushButton::clicked, this, [=]() {
    int row = waiterListWidget->currentRow();
    if (row >= 0 && row < waiterOrders.size()) {
      Order ord = waiterOrders.takeAt(row);
      QMessageBox::information(
          this, "Selesai",
          QString("Order #%1 telah diantar ke meja konsumen. Cycle selesai!")
              .arg(ord.orderId));
      updateWaiterUI();
    }
  });
  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}

void RestaurantApp::updateWaiterUI() {
  waiterListWidget->clear();
  for (const auto &ord : waiterOrders) {
    waiterListWidget->addItem(
        QString("Order #%1 - SIAP DIANTAR").arg(ord.orderId));
  }
}
