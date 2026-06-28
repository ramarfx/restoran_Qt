#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>

// 3. CART PAGE
void RestaurantApp::createCartPage() {
  cartPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(cartPage);
  layout->setContentsMargins(30, 20, 30, 20);
  layout->setSpacing(15);

  // Header
  QHBoxLayout *headerRow = new QHBoxLayout;
  QLabel *title = new QLabel("🛒  Keranjang Pemesanan Anda");
  title->setObjectName("pageHeader");
  headerRow->addWidget(title);
  layout->addLayout(headerRow);

  // Form input pemesanan
  QFrame *inputFrame = new QFrame;
  inputFrame->setObjectName("infoCard");
  QVBoxLayout *inputLayout = new QVBoxLayout(inputFrame);
  inputLayout->setContentsMargins(16, 12, 16, 12);
  inputLayout->setSpacing(8);

  QLabel *lblInputTitle = new QLabel("📋  Data Pemesan");
  lblInputTitle->setObjectName("sectionHeader");

  QFormLayout *formLayout = new QFormLayout;
  formLayout->setSpacing(12);
  
  QLineEdit *txtName = new QLineEdit;
  txtName->setPlaceholderText("Masukkan Nama Anda");
  txtName->setObjectName("inputField");

  QComboBox *cmbOrderType = new QComboBox;
  cmbOrderType->addItems({"Dine In (Makan di Tempat)", "Takeaway (Bawa Pulang)"});
  cmbOrderType->setObjectName("inputField");

  formLayout->addRow("Nama Pelanggan :", txtName);
  formLayout->addRow("Tipe Pesanan   :", cmbOrderType);

  inputLayout->addWidget(lblInputTitle);
  inputLayout->addLayout(formLayout);
  layout->addWidget(inputFrame);

  // Daftar item
  cartListWidget = new QListWidget;
  layout->addWidget(cartListWidget, 1);

  // Label total
  cartTotalLabel = new QLabel("Total Pembayaran:  Rp 0");
  cartTotalLabel->setObjectName("totalLabel");
  layout->addWidget(cartTotalLabel);

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->setSpacing(15);
  QPushButton *btnDelete   = new QPushButton("🗑  Hapus Item Terpilih");
  QPushButton *btnCheckout = new QPushButton("✔  Checkout");
  QPushButton *btnBack     = new QPushButton("← Kembali Belanja");

  btnBack->setObjectName("secondaryBtn");
  btnDelete->setObjectName("dangerBtn");
  btnCheckout->setObjectName("primaryBtn");

  btnLayout->addWidget(btnBack);
  btnLayout->addStretch();
  btnLayout->addWidget(btnDelete);
  btnLayout->addWidget(btnCheckout);

  layout->addLayout(btnLayout);

  // Update info customer setiap kali halaman ditampilkan
  // Dilakukan via updateCartUI() yang dipanggil sebelum pindah ke index 2

  // Hapus Item Terpilih
  connect(btnDelete, &QPushButton::clicked, this, [=]() {
    int row = cartListWidget->currentRow();
    if (row >= 0 && row < cartItems.size()) {
      cartItems.removeAt(row);
      updateCartUI();
    } else {
      QMessageBox::warning(this, "Pilih",
                           "Silakan pilih item dari keranjang yang ingin dihapus.");
    }
  });

  // Checkout
  connect(btnCheckout, &QPushButton::clicked, this, [=]() {
    if (cartItems.isEmpty()) {
      QMessageBox::warning(this, "Kosong", "Keranjang Anda kosong!");
      return;
    }
    
    QString custName = txtName->text().trimmed();
    if (custName.isEmpty()) {
      QMessageBox::warning(this, "Wajib Diisi", "Nama pelanggan tidak boleh kosong!");
      return;
    }

    // Set info global
    currentCustomerName = custName;
    bool isTakeaway = (cmbOrderType->currentIndex() == 1);
    currentTableNumber = isTakeaway ? 0 : -1;
    currentQueueNumber = queueCounter++;

    // Hitung total
    int total = 0;
    for (const auto &item : cartItems) total += item.price;

    // Buat order baru dengan info customer
    Order newOrder;
    newOrder.orderId      = orderCounter++;
    
    // Assign table number if Dine In and not assigned yet
    if (currentTableNumber == -1) {
        currentTableNumber = (newOrder.orderId % 20) + 1; // Auto generate table 1-20
    }
    
    newOrder.status       = "Belum Dibayar";
    newOrder.items        = cartItems.toQList();
    newOrder.customerName = currentCustomerName;
    newOrder.tableNumber  = currentTableNumber;
    newOrder.queueNumber  = currentQueueNumber;
    cartItems.clear();

    cashierOrders.append(newOrder);

    // Info notifikasi untuk customer
    QString tableInfo = (newOrder.tableNumber == 0)
        ? "Takeaway"
        : QString("Meja %1").arg(newOrder.tableNumber);

    QString formattedTotal = QString::number(total);
    int len = formattedTotal.length();
    for (int i = len - 3; i > 0; i -= 3)
        formattedTotal.insert(i, '.');

    QMessageBox::information(
        this, "✅  Pesanan Diterima!",
        QString(
            "Halo, %1!\n\n"
            "Pesanan Anda telah berhasil dibuat.\n\n"
            "📌  Nomor Antrian : #%2\n"
            "🪑  Lokasi          : %3\n"
            "💰  Total           : Rp %4\n\n"
            "Silakan menuju kasir untuk pembayaran.\n"
            "Pesanan akan segera diproses setelah pembayaran.")
            .arg(newOrder.customerName)
            .arg(newOrder.queueNumber)
            .arg(tableInfo)
            .arg(formattedTotal));

    // Reset input form
    txtName->clear();
    cmbOrderType->setCurrentIndex(0);

    updateCartUI();
    stackedWidget->setCurrentIndex(0);
  });

  connect(btnBack, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(1); });
}

void RestaurantApp::updateCartUI() {
  cartListWidget->clear();
  int total = 0;

  for (const auto &item : cartItems) {
    QString formattedPrice = QString::number(item.price);
    int len = formattedPrice.length();
    for (int i = len - 3; i > 0; i -= 3)
        formattedPrice.insert(i, '.');
    cartListWidget->addItem(
        QString("• %1   →   Rp %2").arg(item.name).arg(formattedPrice));
    total += item.price;
  }

  // Format total
  QString formattedTotal = QString::number(total);
  int len = formattedTotal.length();
  for (int i = len - 3; i > 0; i -= 3)
      formattedTotal.insert(i, '.');
  cartTotalLabel->setText(
      QString("Total Pembayaran:   Rp %1").arg(formattedTotal));

  // (Info customer card label dinamis sudah dihapus, sekarang menggunakan input form)

  if (btnViewCart)
    btnViewCart->setText(
        QString("🛒  Keranjang (%1)").arg(cartItems.size()));
}
