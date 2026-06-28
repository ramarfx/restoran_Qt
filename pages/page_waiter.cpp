#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>

// 7. WAITER PAGE
void RestaurantApp::createWaiterPage() {
  waiterPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(waiterPage);
  layout->setContentsMargins(30, 20, 30, 20);
  layout->setSpacing(15);

  QLabel *title = new QLabel("🛎  Dashboard Pelayan — Antrean Pengantaran");
  title->setObjectName("pageHeader");

  QLabel *hint = new QLabel(
      "Daftar pesanan yang telah selesai dimasak dan siap diantarkan ke meja.");
  hint->setObjectName("hintText");
  hint->setWordWrap(true);

  waiterListWidget = new QListWidget;

  QPushButton *btnDeliver = new QPushButton("✔  Tandai Sudah Diantar");
  QPushButton *btnHome    = new QPushButton("← Beranda");

  btnDeliver->setObjectName("primaryBtn");
  btnHome->setObjectName("secondaryBtn");

  layout->addWidget(title);
  layout->addWidget(hint);
  layout->addWidget(waiterListWidget, 1);

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

      QString tableStr = (ord.tableNumber == 0)
          ? "kepada pelanggan (Takeaway)"
          : QString("ke Meja %1").arg(ord.tableNumber);

      QMessageBox::information(
          this, "✅  Selesai Diantar",
          QString(
              "Pesanan atas nama %1\n"
              "telah diantarkan %2.\n\n"
              "Order #%3 — Antrian #%4\n"
              "Terima kasih! 🙏")
              .arg(ord.customerName)
              .arg(tableStr)
              .arg(ord.orderId)
              .arg(ord.queueNumber));

      updateWaiterUI();
    } else {
      QMessageBox::warning(this, "Pilih",
                           "Pilih pesanan yang ingin ditandai sebagai sudah diantar.");
    }
  });

  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}

void RestaurantApp::updateWaiterUI() {
  waiterListWidget->clear();
  for (const auto &ord : waiterOrders) {
    QString tableStr = (ord.tableNumber == 0)
        ? "Takeaway"
        : QString("Meja %1").arg(ord.tableNumber);

    // Baris utama: nama & meja
    QString summary = QString(
        "🎫 Antrian #%1  |  👤 %2  |  🪑 %3  |  📦 %4 item  |  ✅ Siap Diantar")
        .arg(ord.queueNumber, 3, 10, QChar('0'))
        .arg(ord.customerName)
        .arg(tableStr)
        .arg(ord.items.size());

    QListWidgetItem *listItem = new QListWidgetItem(summary);
    listItem->setToolTip(
        QString("Order #%1\nPelanggan: %2\nLokasi: %3\nJumlah item: %4")
            .arg(ord.orderId)
            .arg(ord.customerName)
            .arg(tableStr)
            .arg(ord.items.size()));
    waiterListWidget->addItem(listItem);
  }
}
