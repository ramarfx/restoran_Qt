#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>

// 3. CART PAGE
void RestaurantApp::createCartPage() {
  cartPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(cartPage);
  layout->setContentsMargins(30, 30, 30, 30);
  layout->setSpacing(20);

  QLabel *title = new QLabel("Keranjang Pemesanan Anda");
  title->setObjectName("pageHeader");
  cartListWidget = new QListWidget;

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->setSpacing(15);
  QPushButton *btnDelete = new QPushButton("Hapus Item Terpilih");
  QPushButton *btnCheckout = new QPushButton("Checkout Pesanan");
  QPushButton *btnBack = new QPushButton("Kembali Belanja");

  btnBack->setObjectName("secondaryBtn");
  btnDelete->setObjectName("dangerBtn");
  btnCheckout->setObjectName("primaryBtn");

  btnLayout->addWidget(btnBack);
  btnLayout->addStretch();
  btnLayout->addWidget(btnDelete);
  btnLayout->addWidget(btnCheckout);

  layout->addWidget(title);
  layout->addWidget(cartListWidget);
  layout->addLayout(btnLayout);

  // Logika Hapus Item Terpilih
  connect(btnDelete, &QPushButton::clicked, this, [=]() {
    int currentRow = cartListWidget->currentRow();
    if (currentRow >= 0 && currentRow < cartItems.size()) {
      cartItems.removeAt(currentRow);
      updateCartUI();
    } else {
      QMessageBox::warning(this, "Pilih", "Silakan pilih item dari keranjang yang ingin dihapus.");
    }
  });

  // Logika Checkout
  connect(btnCheckout, &QPushButton::clicked, this, [=]() {
    if (cartItems.isEmpty()) {
      QMessageBox::warning(this, "Kosong", "Keranjang Anda kosong!");
      return;
    }

    Order newOrder;
    newOrder.orderId = orderCounter++;
    newOrder.status = "Belum Dibayar";
    newOrder.items = cartItems;
    cartItems.clear();

    cashierOrders.append(newOrder);
    QMessageBox::information(
        this, "Sukses",
        "Pesanan berhasil di-checkout. Silakan bayar di Kasir.");
    updateCartUI();
    stackedWidget->setCurrentIndex(0); 
  });

  connect(btnBack, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(1); });
}
