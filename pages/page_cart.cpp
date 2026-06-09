#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>

// 3. CART PAGE (Logika Stack)
void RestaurantApp::createCartPage() {
  cartPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(cartPage);
  layout->setContentsMargins(30, 30, 30, 30);
  layout->setSpacing(20);

  QLabel *title = new QLabel("Keranjang Pemesanan Anda (LIFO Stack)");
  title->setObjectName("pageHeader");
  cartListWidget = new QListWidget;

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->setSpacing(15);
  QPushButton *btnUndo = new QPushButton("Batal Terakhir (Pop / Undo)");
  QPushButton *btnCheckout = new QPushButton("Checkout Pesanan");
  QPushButton *btnBack = new QPushButton("Kembali Belanja");

  btnBack->setObjectName("secondaryBtn");
  btnUndo->setObjectName("dangerBtn");
  btnCheckout->setObjectName("primaryBtn");

  btnLayout->addWidget(btnBack);
  btnLayout->addStretch();
  btnLayout->addWidget(btnUndo);
  btnLayout->addWidget(btnCheckout);

  layout->addWidget(title);
  layout->addWidget(cartListWidget);
  layout->addLayout(btnLayout);

  // Logika POP
  connect(btnUndo, &QPushButton::clicked, this, [=]() {
    if (!cartStack.isEmpty()) {
      cartStack.pop(); // Menghapus elemen terakhir dimasukkan
      updateCartUI();
    } else {
      QMessageBox::information(this, "Info", "Keranjang kosong!");
    }
  });

  // Logika Checkout
  connect(btnCheckout, &QPushButton::clicked, this, [=]() {
    if (cartStack.isEmpty()) {
      QMessageBox::warning(this, "Kosong", "Keranjang Anda kosong!");
      return;
    }

    Order newOrder;
    newOrder.orderId = orderCounter++;
    newOrder.status = "Belum Dibayar";
    // Memindahkan isi stack ke order (dibalik agar urutannya logis)
    while (!cartStack.isEmpty()) {
      newOrder.items.prepend(cartStack.pop());
    }

    cashierOrders.append(newOrder);
    QMessageBox::information(
        this, "Sukses",
        "Pesanan berhasil di-checkout. Silakan bayar di Kasir.");
    stackedWidget->setCurrentIndex(0); // Balik ke Home
  });

  connect(btnBack, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(1); });
}
