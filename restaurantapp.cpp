#include "restaurantapp.h"
#include <QApplication>
#include <QDateTime>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

RestaurantApp::RestaurantApp(QWidget *parent)
    : QMainWindow(parent), orderCounter(1) {
  setWindowTitle("Gourmet POS - Sistem Pemesanan Restoran");
  resize(800, 600);
  setupUI();
  applyStyles();
}

void RestaurantApp::setupUI() {
  stackedWidget = new QStackedWidget(this);
  setCentralWidget(stackedWidget);

  createHomePage();
  createConsumerPage();
  createCartPage();
  createLoginPage();
  createCashierPage();
  createKitchenPage();
  createWaiterPage();

  stackedWidget->addWidget(homePage);     // 0
  stackedWidget->addWidget(consumerPage); // 1
  stackedWidget->addWidget(cartPage);     // 2
  stackedWidget->addWidget(loginPage);    // 3
  stackedWidget->addWidget(cashierPage);  // 4
  stackedWidget->addWidget(kitchenPage);  // 5
  stackedWidget->addWidget(waiterPage);   // 6
}

// 1. HOME PAGE (Beranda Pemilihan Role)
void RestaurantApp::createHomePage() {
  homePage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(homePage);
  layout->setAlignment(Qt::AlignCenter);

  QLabel *title = new QLabel("Selamat Datang di Gourmet Resto");
  title->setObjectName("headerText");

  QPushButton *btnConsumer = new QPushButton("Pesan Makanan (Konsumen)");
  QPushButton *btnLogin = new QPushButton("Login Pegawai (Kasir / Dapur)");
  QPushButton *btnWaiter = new QPushButton("Area Pelayan (Output)");

  layout->addWidget(title, 0, Qt::AlignCenter);
  layout->addWidget(btnConsumer);
  layout->addWidget(btnLogin);
  layout->addWidget(btnWaiter);

  connect(btnConsumer, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(1); });
  connect(btnLogin, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(3); });
  connect(btnWaiter, &QPushButton::clicked, this, [=]() {
    updateWaiterUI();
    stackedWidget->setCurrentIndex(6);
  });
}

// 2. KONSUMEN PAGE (Menu & Katalog)
void RestaurantApp::createConsumerPage() {
  consumerPage = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(consumerPage);
  QHBoxLayout *contentLayout = new QHBoxLayout;

  // Panel Kiri: Kategori
  QVBoxLayout *leftPanel = new QVBoxLayout;
  QLabel *lblCategory = new QLabel("Kategori");
  QListWidget *categoryList = new QListWidget;
  categoryList->addItems({"Makanan Utama", "Minuman", "Cemilan", "Dessert"});
  leftPanel->addWidget(lblCategory);
  leftPanel->addWidget(categoryList);

  // Panel Kanan: Katalog
  QVBoxLayout *rightPanel = new QVBoxLayout;
  QLabel *lblCatalog = new QLabel("Katalog Makanan");
  QListWidget *catalogList = new QListWidget;
  catalogList->addItems(
      {"Nasi Goreng Spesial - Rp 35.000", "Steak Wagyu - Rp 150.000",
       "Es Teh Manis - Rp 10.000", "Pudding Coklat - Rp 25.000"});
  rightPanel->addWidget(lblCatalog);
  rightPanel->addWidget(catalogList);

  contentLayout->addLayout(leftPanel, 1);
  contentLayout->addLayout(rightPanel, 3);

  // Tombol Bawah
  QHBoxLayout *bottomLayout = new QHBoxLayout;
  QPushButton *btnAddToCart = new QPushButton("Tambah ke Keranjang");
  QPushButton *btnViewCart = new QPushButton("Lihat Keranjang (0)");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  bottomLayout->addWidget(btnHome);
  bottomLayout->addWidget(btnAddToCart);
  bottomLayout->addWidget(btnViewCart);

  mainLayout->addLayout(contentLayout);
  mainLayout->addLayout(bottomLayout);

  // Logika Tambah Keranjang
  connect(btnAddToCart, &QPushButton::clicked, this, [=]() {
    if (catalogList->currentItem()) {
      QString text = catalogList->currentItem()->text();
      QString name = text.split(" - ")[0];
      int price = text.split(" - ")[1].remove("Rp ").replace(".", "").toInt();

      cartStack.push({name, price}); // PUSH ke Stack
      btnViewCart->setText(
          QString("Lihat Keranjang (%1)").arg(cartStack.size()));
    } else {
      QMessageBox::warning(this, "Pilih",
                           "Silakan pilih makanan dari katalog dulu.");
    }
  });

  connect(btnViewCart, &QPushButton::clicked, this, [=]() {
    updateCartUI();
    stackedWidget->setCurrentIndex(2);
  });
  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}

// 3. CART PAGE (Logika Stack)
void RestaurantApp::createCartPage() {
  cartPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(cartPage);

  QLabel *title = new QLabel("Keranjang Anda (LIFO Stack)");
  title->setObjectName("headerText");
  cartListWidget = new QListWidget;

  QHBoxLayout *btnLayout = new QHBoxLayout;
  QPushButton *btnUndo = new QPushButton("Batal (Pop / Undo)");
  QPushButton *btnCheckout = new QPushButton("Checkout");
  QPushButton *btnBack = new QPushButton("Kembali");

  btnLayout->addWidget(btnBack);
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

void RestaurantApp::updateCartUI() {
  cartListWidget->clear();
  int total = 0;
  for (const auto &item : cartStack) {
    cartListWidget->addItem(item.name + " - Rp " +
                             QString::number(item.price));
    total += item.price;
  }
  cartListWidget->addItem(QString("\nTOTAL: Rp %1").arg(total));
}

// 4. LOGIN PAGE (Gerbang Pegawai)
void RestaurantApp::createLoginPage() {
  loginPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(loginPage);
  layout->setAlignment(Qt::AlignCenter);

  QFormLayout *formLayout = new QFormLayout;
  QLineEdit *txtUser = new QLineEdit;
  QLineEdit *txtPass = new QLineEdit;
  txtPass->setEchoMode(QLineEdit::Password);

  formLayout->addRow("Username:", txtUser);
  formLayout->addRow("Password:", txtPass);

  QPushButton *btnLogin = new QPushButton("Login");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  layout->addWidget(new QLabel("Login Pegawai"));
  layout->addLayout(formLayout);
  layout->addWidget(btnLogin);
  layout->addWidget(btnHome);

  connect(btnLogin, &QPushButton::clicked, this, [=]() {
    QString user = txtUser->text();
    if (user == "kasir") {
      updateCashierUI();
      stackedWidget->setCurrentIndex(4);
    } else if (user == "koki") {
      updateKitchenUI();
      stackedWidget->setCurrentIndex(5);
    } else {
      QMessageBox::warning(this, "Error",
                           "Username salah! Coba 'kasir' atau 'koki'.");
    }
    txtUser->clear();
    txtPass->clear();
  });
  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}

// 5. KASIR PAGE
void RestaurantApp::createCashierPage() {
  cashierPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(cashierPage);

  layout->addWidget(new QLabel("Dashboard Kasir - Antrean Pembayaran"));
  cashierListWidget = new QListWidget;

  QPushButton *btnPay = new QPushButton("Proses Pembayaran (Kirim ke Dapur)");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  layout->addWidget(cashierListWidget);
  layout->addWidget(btnPay);
  layout->addWidget(btnHome);

  connect(btnPay, &QPushButton::clicked, this, [=]() {
    int row = cashierListWidget->currentRow();
    if (row >= 0 && row < cashierOrders.size()) {
      Order ord = cashierOrders.takeAt(row);
      ord.status = "Dibayar & Masak";
      kitchenQueue.enqueue(ord); // Masukkan ke QUEUE Dapur

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
  for (const auto &ord : cashierOrders) {
    cashierListWidget->addItem(QString("Order #%1 - %2 items - %3")
                                   .arg(ord.orderId)
                                   .arg(ord.items.size())
                                   .arg(ord.status));
  }
}

// 6. KITCHEN PAGE (Logika Queue)
void RestaurantApp::createKitchenPage() {
  kitchenPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(kitchenPage);

  layout->addWidget(new QLabel("Dashboard Dapur (FIFO Queue)"));
  
  // Set up Scroll Area for Cards
  kitchenScrollArea = new QScrollArea;
  kitchenScrollArea->setWidgetResizable(true);
  kitchenQueueContainer = new QWidget;
  kitchenQueueContainer->setObjectName("kitchenQueueContainer");
  kitchenQueueLayout = new QVBoxLayout(kitchenQueueContainer);
  kitchenQueueLayout->setAlignment(Qt::AlignTop);
  kitchenQueueContainer->setLayout(kitchenQueueLayout);
  kitchenScrollArea->setWidget(kitchenQueueContainer);

  currentCookingLabel = new QLabel("Sedang Dimasak: Belum ada");
  currentCookingLabel->setObjectName("highlightText");

  QPushButton *btnDequeue = new QPushButton("Masak Order Terdepan (Dequeue)");
  QPushButton *btnReady = new QPushButton("Tandai Siap Saji");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  layout->addWidget(kitchenScrollArea);
  layout->addWidget(currentCookingLabel);
  layout->addWidget(btnDequeue);
  layout->addWidget(btnReady);
  layout->addWidget(btnHome);

  connect(btnDequeue, &QPushButton::clicked, this, [=]() {
    if (isCooking) {
      QMessageBox::warning(
          this, "Sibuk",
          "Selesaikan order saat ini sebelum memasak yang baru!");
      return;
    }
    if (!kitchenQueue.isEmpty()) {
      currentCookingOrder =
          kitchenQueue.dequeue(); // Ambil dari antrean depan
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

// 7. WAITER PAGE (Output Akhir)
void RestaurantApp::createWaiterPage() {
  waiterPage = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(waiterPage);

  layout->addWidget(new QLabel("Dashboard Pelayan - Siap Antar"));
  waiterListWidget = new QListWidget;

  QPushButton *btnDeliver = new QPushButton("Antar ke Meja / Selesai");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  layout->addWidget(waiterListWidget);
  layout->addWidget(btnDeliver);
  layout->addWidget(btnHome);

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

// UI QSS STYLING (Biar Cakep Sesuai Request)
void RestaurantApp::applyStyles() {
  QString qss = R"(
          QMainWindow {
              background-color: #f7f9fa;
          }
          QLabel {
              font-size: 14px;
              color: #2c3e50;
              font-weight: bold;
              background-color: transparent;
          }
          QLabel#headerText {
              font-size: 24px;
              color: #e74c3c;
              margin-bottom: 20px;
          }
          QLabel#highlightText {
              font-size: 16px;
              color: #d35400;
              padding: 10px;
              background-color: #fdebd0;
              border-radius: 5px;
          }
          QPushButton {
              background-color: #e74c3c;
              color: white;
              border-radius: 8px;
              padding: 12px;
              font-size: 14px;
              font-weight: bold;
              border: none;
              min-width: 150px;
          }
          QPushButton:hover {
              background-color: #c0392b;
          }
          QPushButton:pressed {
              background-color: #a93226;
          }
          QListWidget {
              background-color: white;
              color: #2c3e50;
              border: 1px solid #bdc3c7;
              border-radius: 8px;
              padding: 5px;
              font-size: 14px;
          }
          QListWidget::item {
              color: #2c3e50;
              padding: 10px;
              border-bottom: 1px solid #ecf0f1;
          }
          QListWidget::item:selected {
              background-color: #fadbd8;
              color: #c0392b;
          }
          QLineEdit {
              background-color: white;
              color: #2c3e50;
              padding: 10px;
              border: 1px solid #bdc3c7;
              border-radius: 5px;
              font-size: 14px;
          }
          QScrollArea {
              border: none;
              background-color: transparent;
          }
          QWidget#kitchenQueueContainer {
              background-color: transparent;
          }
          QFrame#orderCard {
              background-color: white;
              border: 1px solid #dcdde1;
              border-radius: 12px;
              padding: 15px;
              margin-bottom: 10px;
          }
          QLabel#cardHeader {
              font-size: 16px;
              color: #2c3e50;
              font-weight: bold;
              border-bottom: 1px solid #f1f2f6;
              padding-bottom: 5px;
              margin-bottom: 5px;
          }
          QLabel#cardItemText {
              font-size: 13px;
              color: #57606f;
              font-weight: normal;
          }
          QLabel#cardStatus {
              font-size: 12px;
              color: #e67e22;
              font-weight: bold;
              margin-top: 5px;
          }
      )";
  qApp->setStyleSheet(qss);
}
