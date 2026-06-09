#include "restaurantapp.h"
#include "algorithms.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QMap>
#include <algorithm>

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
  QMap<QString, QList<FoodItem>> menuData;
  menuData["Makanan Utama"] = {
      {"Nasi Goreng Spesial", 35000},
      {"Steak Wagyu", 150000},
      {"Ayam Bakar Madu", 40000}
  };
  menuData["Minuman"] = {
      {"Es Teh Manis", 10000},
      {"Kopi Susu Gula Aren", 18000},
      {"Jus Alpukat", 22000}
  };
  menuData["Cemilan"] = {
      {"Kentang Goreng", 20000},
      {"Cireng Crispy", 15000}
  };
  menuData["Dessert"] = {
      {"Pudding Coklat", 25000},
      {"Ice Cream Vanilla", 18000}
  };

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
    QList<FoodItem> rawItems;
    if (selectedCategory == "Semua") {
        for (auto it = menuData.begin(); it != menuData.end(); ++it) {
            rawItems.append(it.value());
        }
    } else {
        if (!menuData.contains(selectedCategory)) return;
        rawItems = menuData[selectedCategory];
    }

    QString searchText = searchBar->text();
    int sortOption = sortCombo->currentIndex(); // 0: Default, 1: Harga Terendah, 2: Harga Tertinggi, 3: Nama A-Z, 4: Nama Z-A

    QList<FoodItem> items = MenuAlgorithms::search(rawItems, searchText);
    MenuAlgorithms::sort(items, sortOption);

    // Tampilkan ke UI
    for (const auto &item : items) {
        QString formattedPrice = QString::number(item.price);
        int len = formattedPrice.length();
        for (int i = len - 3; i > 0; i -= 3) {
            formattedPrice.insert(i, '.');
        }
        catalogList->addItem(item.name + " - Rp " + formattedPrice);
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

void RestaurantApp::updateCartUI() {
  cartListWidget->clear();
  int total = 0;
  for (const auto &item : cartStack) {
    cartListWidget->addItem(item.name + " - Rp " + QString::number(item.price));
    total += item.price;
  }
  cartListWidget->addItem(QString("\nTOTAL: Rp %1").arg(total));
}
