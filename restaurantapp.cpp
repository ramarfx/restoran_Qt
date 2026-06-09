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
#include <QComboBox>
#include <algorithm>

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
  QVBoxLayout *mainLayout = new QVBoxLayout(homePage);
  mainLayout->setAlignment(Qt::AlignCenter);
  mainLayout->setContentsMargins(40, 40, 40, 40);

  QFrame *homeCard = new QFrame;
  homeCard->setObjectName("homeCard");
  QVBoxLayout *cardLayout = new QVBoxLayout(homeCard);
  cardLayout->setContentsMargins(40, 40, 40, 40);
  cardLayout->setSpacing(20);
  cardLayout->setAlignment(Qt::AlignCenter);

  QPixmap pixmaps(":/image/logo.png");
  QLabel *logoEmoji = new QLabel(this);
  logoEmoji->setObjectName("logoEmoji");
  logoEmoji->setAlignment(Qt::AlignCenter);
  logoEmoji->setPixmap(pixmaps);

  QLabel *subtitle = new QLabel("Sistem Manajemen & Pemesanan Kasir Kafe");
  subtitle->setObjectName("homeSubtitle");
  subtitle->setAlignment(Qt::AlignCenter);

  QPushButton *btnConsumer = new QPushButton("Pesan Makanan (Konsumen)");
  QPushButton *btnLogin = new QPushButton("Login Pegawai (Kasir / Dapur)");
  QPushButton *btnWaiter = new QPushButton("Area Pelayan (Output)");

  btnConsumer->setObjectName("primaryBtn");
  btnLogin->setObjectName("secondaryBtn");
  btnWaiter->setObjectName("secondaryBtn");

  cardLayout->addWidget(logoEmoji);
  cardLayout->addWidget(subtitle);
  cardLayout->addWidget(btnConsumer);
  cardLayout->addWidget(btnLogin);
  cardLayout->addWidget(btnWaiter);

  mainLayout->addWidget(homeCard);

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
  mainLayout->setContentsMargins(30, 30, 30, 30);
  mainLayout->setSpacing(20);

  QLabel *header = new QLabel("Katalog Pemesanan Konsumen");
  header->setObjectName("pageHeader");
  mainLayout->addWidget(header);

  QHBoxLayout *contentLayout = new QHBoxLayout;
  contentLayout->setSpacing(20);

  // Panel Kiri: Kategori
  QVBoxLayout *leftPanel = new QVBoxLayout;
  leftPanel->setSpacing(10);
  QLabel *lblCategory = new QLabel("Pilih Kategori");
  lblCategory->setObjectName("sectionHeader");
  QListWidget *categoryList = new QListWidget;
  categoryList->addItems({"Semua", "Makanan Utama", "Minuman", "Cemilan", "Dessert"});
  leftPanel->addWidget(lblCategory);
  leftPanel->addWidget(categoryList);

  // Panel Kanan: Katalog
  QVBoxLayout *rightPanel = new QVBoxLayout;
  rightPanel->setSpacing(10);
  QLabel *lblCatalog = new QLabel("Pilihan Menu Makanan & Minuman");
  lblCatalog->setObjectName("sectionHeader");
  QListWidget *catalogList = new QListWidget;

  // --- MODIFIKASI DATA & LOGIKA DI SINI ---

  // Data Master Menu berdasarkan Kategori
  QMap<QString, QStringList> menuData;
  menuData["Makanan Utama"] = {"Nasi Goreng Spesial - Rp 35.000",
                               "Steak Wagyu - Rp 150.000",
                               "Ayam Bakar Madu - Rp 40.000"};
  menuData["Minuman"] = {"Es Teh Manis - Rp 10.000",
                         "Kopi Susu Gula Aren - Rp 18.000",
                         "Jus Alpukat - Rp 22.000"};
  menuData["Cemilan"] = {"Kentang Goreng - Rp 20.000",
                         "Cireng Crispy - Rp 15.000"};
  menuData["Dessert"] = {"Pudding Coklat - Rp 25.000",
                         "Ice Cream Vanilla - Rp 18.000"};

  // Tambahkan Input Pencarian dan Dropdown Pengurutan
  QHBoxLayout *filterLayout = new QHBoxLayout;
  filterLayout->setSpacing(10);

  QLineEdit *searchBar = new QLineEdit;
  searchBar->setPlaceholderText("Cari menu...");
  searchBar->setObjectName("searchBar");

  QComboBox *sortCombo = new QComboBox;
  sortCombo->addItems({
      "Default (Tanpa Urutan)",
      "Harga: Terendah ke Tertinggi",
      "Harga: Tertinggi ke Terendah",
      "Nama: A-Z",
      "Nama: Z-A"
  });
  sortCombo->setObjectName("sortCombo");

  filterLayout->addWidget(searchBar, 2);
  filterLayout->addWidget(sortCombo, 1);

  // Lambda untuk mengupdate dan memfilter/mengurutkan katalog
  auto updateCatalog = [=]() {
    catalogList->clear();
    QListWidgetItem *catItem = categoryList->currentItem();
    if (!catItem) return;

    QString selectedCategory = catItem->text();
    QStringList rawItems;
    if (selectedCategory == "Semua") {
        for (auto it = menuData.begin(); it != menuData.end(); ++it) {
            rawItems.append(it.value());
        }
    } else {
        if (!menuData.contains(selectedCategory)) return;
        rawItems = menuData[selectedCategory];
    }

    QString searchText = searchBar->text().trimmed().toLower();
    int sortOption = sortCombo->currentIndex(); // 0: Default, 1: Harga Terendah, 2: Harga Tertinggi, 3: Nama A-Z, 4: Nama Z-A

    struct ParsedItem {
        QString originalText;
        QString name;
        int price;
    };

    QList<ParsedItem> items;
    for (const QString &itemText : rawItems) {
        QStringList parts = itemText.split(" - ");
        if (parts.size() < 2) continue;

        QString name = parts[0];
        QString priceStr = parts[1];
        int price = priceStr.remove("Rp ").replace(".", "").toInt();

        // Filter pencarian
        if (!searchText.isEmpty() && !name.toLower().contains(searchText)) {
            continue;
        }

        items.append({itemText, name, price});
    }

    // Pengurutan
    if (sortOption == 1) { // Harga Terendah ke Tertinggi
        std::sort(items.begin(), items.end(), [](const ParsedItem &a, const ParsedItem &b) {
            return a.price < b.price;
        });
    } else if (sortOption == 2) { // Harga Tertinggi ke Terendah
        std::sort(items.begin(), items.end(), [](const ParsedItem &a, const ParsedItem &b) {
            return a.price > b.price;
        });
    } else if (sortOption == 3) { // Nama A-Z
        std::sort(items.begin(), items.end(), [](const ParsedItem &a, const ParsedItem &b) {
            return a.name.localeAwareCompare(b.name) < 0;
        });
    } else if (sortOption == 4) { // Nama Z-A
        std::sort(items.begin(), items.end(), [](const ParsedItem &a, const ParsedItem &b) {
            return a.name.localeAwareCompare(b.name) > 0;
        });
    }

    // Tampilkan ke UI
    for (const auto &item : items) {
        catalogList->addItem(item.originalText);
    }
  };

  // Koneksi Sinyal & Slot
  connect(categoryList, &QListWidget::itemSelectionChanged, this, updateCatalog);
  connect(searchBar, &QLineEdit::textChanged, this, [=](const QString &) { updateCatalog(); });
  connect(sortCombo, &QComboBox::currentIndexChanged, this, [=](int) { updateCatalog(); });

  // Set default item pertama (Makanan Utama) aktif saat halaman dibuka
  categoryList->setCurrentRow(0);

  // --- AKHIR MODIFIKASI ---

  rightPanel->addWidget(lblCatalog);
  rightPanel->addLayout(filterLayout);
  rightPanel->addWidget(catalogList);

  contentLayout->addLayout(leftPanel, 1);
  contentLayout->addLayout(rightPanel, 2);

  // Tombol Bawah
  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->setSpacing(15);
  QPushButton *btnAddToCart = new QPushButton("Tambah ke Keranjang");
  QPushButton *btnViewCart = new QPushButton("Lihat Keranjang (0)");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  btnHome->setObjectName("secondaryBtn");
  btnAddToCart->setObjectName("primaryBtn");
  btnViewCart->setObjectName("accentBtn");

  bottomLayout->addWidget(btnHome);
  bottomLayout->addStretch();
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

void RestaurantApp::updateCartUI() {
  cartListWidget->clear();
  int total = 0;
  for (const auto &item : cartStack) {
    cartListWidget->addItem(item.name + " - Rp " + QString::number(item.price));
    total += item.price;
  }
  cartListWidget->addItem(QString("\nTOTAL: Rp %1").arg(total));
}

// 4. LOGIN PAGE (Gerbang Pegawai)
void RestaurantApp::createLoginPage() {
  loginPage = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(loginPage);
  mainLayout->setAlignment(Qt::AlignCenter);
  mainLayout->setContentsMargins(40, 40, 40, 40);

  QFrame *loginCard = new QFrame;
  loginCard->setObjectName("loginCard");
  QVBoxLayout *cardLayout = new QVBoxLayout(loginCard);
  cardLayout->setContentsMargins(40, 40, 40, 40);
  cardLayout->setSpacing(20);
  cardLayout->setAlignment(Qt::AlignCenter);

  QLabel *lockEmoji = new QLabel("🔐");
  lockEmoji->setObjectName("logoEmoji");
  lockEmoji->setAlignment(Qt::AlignCenter);

  QLabel *title = new QLabel("Akses Pegawai");
  title->setObjectName("homeTitle");
  title->setAlignment(Qt::AlignCenter);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->setSpacing(15);
  QLineEdit *txtUser = new QLineEdit;
  QLineEdit *txtPass = new QLineEdit;
  txtPass->setEchoMode(QLineEdit::Password);
  txtUser->setPlaceholderText("Username (kasir / koki)");
  txtPass->setPlaceholderText("Password");

  QLabel *lblUser = new QLabel("Username:");
  lblUser->setObjectName("formLabel");
  QLabel *lblPass = new QLabel("Password:");
  lblPass->setObjectName("formLabel");

  formLayout->addRow(lblUser, txtUser);
  formLayout->addRow(lblPass, txtPass);

  QPushButton *btnLogin = new QPushButton("Login Pegawai");
  QPushButton *btnHome = new QPushButton("Kembali");

  btnLogin->setObjectName("primaryBtn");
  btnHome->setObjectName("secondaryBtn");

  cardLayout->addWidget(lockEmoji);
  cardLayout->addWidget(title);
  cardLayout->addLayout(formLayout);
  cardLayout->addSpacing(15);
  cardLayout->addWidget(btnLogin);
  cardLayout->addWidget(btnHome);

  mainLayout->addWidget(loginCard);

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
  layout->setContentsMargins(30, 30, 30, 30);
  layout->setSpacing(20);

  QLabel *title = new QLabel("Dashboard Kasir - Antrean Pembayaran");
  title->setObjectName("pageHeader");
  cashierListWidget = new QListWidget;

  QPushButton *btnPay = new QPushButton("Proses Pembayaran (Kirim ke Dapur)");
  QPushButton *btnHome = new QPushButton("Kembali ke Beranda");

  btnPay->setObjectName("primaryBtn");
  btnHome->setObjectName("secondaryBtn");

  layout->addWidget(title);
  layout->addWidget(cashierListWidget);

  QHBoxLayout *btnLayout = new QHBoxLayout;
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
      kitchenQueue.enqueue(ord); // Masukkan ke QUEUE Dapur

      QMessageBox::information(
          this, "Lunas",
          QString("Order #%1 telah dibayar dan dikirim ke dapur.")
              .arg(ord.orderId));
      updateCashierUI();
    } else {
      QMessageBox::warning(this, "Pilih", "Pilih pesanan yang ingin dibayar.");
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

// 7. WAITER PAGE (Output Akhir)
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
