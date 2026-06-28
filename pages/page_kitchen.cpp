#include "restaurantapp.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

// 6. KITCHEN PAGE (Logika Queue)
void RestaurantApp::createKitchenPage() {
  kitchenPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(kitchenPage);
  layout->setContentsMargins(30, 20, 30, 20);
  layout->setSpacing(15);

  QLabel *title = new QLabel("👨‍🍳  Dashboard Dapur — Antrean Memasak (FIFO)");
  title->setObjectName("pageHeader");
  layout->addWidget(title);

  // Set up Scroll Area for Cards
  kitchenScrollArea = new QScrollArea;
  kitchenScrollArea->setWidgetResizable(true);
  kitchenQueueContainer = new QWidget;
  kitchenQueueContainer->setObjectName("kitchenQueueContainer");
  kitchenQueueLayout = new QVBoxLayout(kitchenQueueContainer);
  kitchenQueueLayout->setAlignment(Qt::AlignTop);
  kitchenQueueContainer->setLayout(kitchenQueueLayout);
  kitchenScrollArea->setWidget(kitchenQueueContainer);

  layout->addWidget(kitchenScrollArea, 1);

  // Label pesanan sedang dimasak
  currentCookingLabel = new QLabel("🔥  Sedang Dimasak: Belum ada");
  currentCookingLabel->setObjectName("highlightText");
  layout->addWidget(currentCookingLabel);

  QPushButton *btnDequeue = new QPushButton("▶  Masak Order Terdepan");
  QPushButton *btnReady   = new QPushButton("✅  Tandai Siap Saji → Kirim ke Pelayan");
  QPushButton *btnHome    = new QPushButton("← Beranda");

  btnDequeue->setObjectName("primaryBtn");
  btnReady->setObjectName("accentBtn");
  btnHome->setObjectName("secondaryBtn");

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->setSpacing(15);
  btnLayout->addWidget(btnHome);
  btnLayout->addStretch();
  btnLayout->addWidget(btnDequeue);
  btnLayout->addWidget(btnReady);
  layout->addLayout(btnLayout);

  connect(btnDequeue, &QPushButton::clicked, this, [=]() {
    if (isCooking) {
      QMessageBox::warning(
          this, "Sibuk",
          "Selesaikan order saat ini sebelum memasak yang baru!");
      return;
    }
    if (!kitchenQueue.isEmpty()) {
      currentCookingOrder = kitchenQueue.dequeue();
      isCooking = true;

      QString tableStr = (currentCookingOrder.tableNumber == 0)
          ? "Takeaway"
          : QString("Meja %1").arg(currentCookingOrder.tableNumber);

      currentCookingLabel->setText(
          QString("🔥  Sedang Dimasak: Order #%1  |  👤 %2  |  🪑 %3")
              .arg(currentCookingOrder.orderId)
              .arg(currentCookingOrder.customerName)
              .arg(tableStr));
      updateKitchenUI();
    } else {
      QMessageBox::information(this, "Kosong", "Tidak ada antrean pesanan.");
    }
  });

  connect(btnReady, &QPushButton::clicked, this, [=]() {
    if (isCooking) {
      currentCookingOrder.status = "Siap Diantar";
      waiterOrders.append(currentCookingOrder);
      isCooking = false;
      currentCookingLabel->setText("🔥  Sedang Dimasak: Belum ada");

      QString tableStr = (currentCookingOrder.tableNumber == 0)
          ? "Takeaway"
          : QString("Meja %1").arg(currentCookingOrder.tableNumber);

      QMessageBox::information(
          this, "✅  Siap Saji",
          QString(
              "Pesanan Order #%1 milik %2 (%3)\n"
              "telah siap dan dikirim ke pelayan!")
              .arg(currentCookingOrder.orderId)
              .arg(currentCookingOrder.customerName)
              .arg(tableStr));
    } else {
      QMessageBox::warning(this, "Kosong",
                           "Tidak ada order yang sedang dimasak saat ini.");
    }
  });

  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}

void RestaurantApp::updateKitchenUI() {
  // Bersihkan kartu lama
  QLayoutItem *item;
  while ((item = kitchenQueueLayout->takeAt(0)) != nullptr) {
    if (item->widget()) item->widget()->deleteLater();
    delete item;
  }

  if (kitchenQueue.isEmpty()) {
    QLabel *empty = new QLabel("Tidak ada pesanan dalam antrean.");
    empty->setObjectName("emptyText");
    empty->setAlignment(Qt::AlignCenter);
    kitchenQueueLayout->addWidget(empty);
    return;
  }

  int qPos = 1;
  for (const auto &ord : kitchenQueue) {
    QFrame *card = new QFrame;
    card->setObjectName("orderCard");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(16, 14, 16, 14);
    cardLayout->setSpacing(6);

    // Header kartu: no antrian + nama + meja
    QString tableStr = (ord.tableNumber == 0)
        ? "Takeaway"
        : QString("Meja %1").arg(ord.tableNumber);

    QHBoxLayout *cardHeader = new QHBoxLayout;
    QLabel *lblOrderId = new QLabel(
        QString("🎫 Antrian #%1  |  Order #%2")
            .arg(ord.queueNumber, 3, 10, QChar('0'))
            .arg(ord.orderId));
    lblOrderId->setObjectName("cardHeader");

    QLabel *lblPos = new QLabel(QString("Posisi: %1").arg(qPos++));
    lblPos->setObjectName("cardPos");
    lblPos->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    cardHeader->addWidget(lblOrderId, 1);
    cardHeader->addWidget(lblPos);
    cardLayout->addLayout(cardHeader);

    // Info customer
    QLabel *lblCust = new QLabel(
        QString("👤  %1   |   🪑  %2").arg(ord.customerName).arg(tableStr));
    lblCust->setObjectName("cardCustomer");
    cardLayout->addWidget(lblCust);

    // Garis pemisah kecil
    QFrame *sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setObjectName("cardDivider");
    cardLayout->addWidget(sep);

    // Item makanan
    for (const auto &food : ord.items) {
      QLabel *lblFood = new QLabel(QString("  •  %1").arg(food.name));
      lblFood->setObjectName("cardItemText");
      cardLayout->addWidget(lblFood);
    }

    QLabel *lblStatus = new QLabel(QString("Status: %1").arg(ord.status));
    lblStatus->setObjectName("cardStatus");
    cardLayout->addWidget(lblStatus);

    kitchenQueueLayout->addWidget(card);
  }
}
