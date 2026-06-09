#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QPushButton>
#include <QMessageBox>

// 6. KITCHEN PAGE (Logika Queue)
void RestaurantApp::createKitchenPage() {
  kitchenPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(kitchenPage);
  layout->setContentsMargins(30, 30, 30, 30);
  layout->setSpacing(20);

  QLabel *title = new QLabel("Dashboard Dapur - Antrean Memasak (FIFO)");
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

  layout->addWidget(kitchenScrollArea);

  currentCookingLabel = new QLabel("Sedang Dimasak: Belum ada");
  currentCookingLabel->setObjectName("highlightText");
  layout->addWidget(currentCookingLabel);

  QPushButton *btnDequeue = new QPushButton("Masak Order Terdepan (Dequeue)");
  QPushButton *btnReady = new QPushButton("Tandai Siap Saji");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

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
      currentCookingOrder = kitchenQueue.dequeue(); // Ambil dari antrean depan
      isCooking = true;
      currentCookingLabel->setText(QString("Sedang Dimasak: Order #%1")
                                       .arg(currentCookingOrder.orderId));
      updateKitchenUI();
    } else {
      QMessageBox::information(this, "Kosong", "Tidak ada antrean pesanan.");
    }
  });

  connect(btnReady, &QPushButton::clicked, this, [=]() {
    if (isCooking) {
      currentCookingOrder.status = "Siap Diantar";
      waiterOrders.append(currentCookingOrder); // Kirim ke pelayan
      isCooking = false;
      currentCookingLabel->setText("Sedang Dimasak: Belum ada");
      QMessageBox::information(this, "Selesai", "Makanan siap disajikan!");
    }
  });

  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}

void RestaurantApp::updateKitchenUI() {
  // Clear previous cards
  QLayoutItem *item;
  while ((item = kitchenQueueLayout->takeAt(0)) != nullptr) {
    if (item->widget()) {
      item->widget()->deleteLater();
    }
    delete item;
  }

  // Add cards for each order in queue
  for (const auto &ord : kitchenQueue) {
    QFrame *card = new QFrame;
    card->setObjectName("orderCard");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    QLabel *lblOrderId = new QLabel(QString("Order #%1").arg(ord.orderId));
    lblOrderId->setObjectName("cardHeader");
    cardLayout->addWidget(lblOrderId);

    // Add each food item in the order
    for (const auto &food : ord.items) {
      QLabel *lblFood = new QLabel(QString("• %1").arg(food.name));
      lblFood->setObjectName("cardItemText");
      cardLayout->addWidget(lblFood);
    }

    QLabel *lblStatus = new QLabel(QString("Status: %1").arg(ord.status));
    lblStatus->setObjectName("cardStatus");
    cardLayout->addWidget(lblStatus);

    kitchenQueueLayout->addWidget(card);
  }
}
