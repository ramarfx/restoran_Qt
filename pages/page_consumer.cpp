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
#include <QFrame>
#include <algorithm>

// 2. KONSUMEN PAGE (Menu & Katalog)
void RestaurantApp::createConsumerPage() {
  consumerPage = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(consumerPage);
  mainLayout->setContentsMargins(30, 20, 30, 20);
  mainLayout->setSpacing(15);

  // --- Header ---
  QHBoxLayout *headerRow = new QHBoxLayout;
  QLabel *header = new QLabel("Katalog Pemesanan");
  header->setObjectName("pageHeader");

  // Info customer (akan diisi nanti di cart page)
  customerInfoLabel = new QLabel("");
  customerInfoLabel->hide();

  headerRow->addWidget(header);
  headerRow->addStretch();
  mainLayout->addLayout(headerRow);

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

  QVBoxLayout *rightPanel = new QVBoxLayout;
  rightPanel->setSpacing(10);
  QLabel *lblCatalog = new QLabel("Pilihan Menu Makanan & Minuman");
  lblCatalog->setObjectName("sectionHeader");
  QListWidget *catalogList = new QListWidget;

  QMap<QString, QList<FoodItem>> menuData;
  menuData["Makanan Utama"] = {
      {"Nasi Goreng Spesial", 35000},
      {"Steak Wagyu",         150000},
      {"Ayam Bakar Madu",     40000},
      {"Sop Buntut",          55000},
      {"Rendang Daging",      50000}
  };
  menuData["Minuman"] = {
      {"Es Teh Manis",         10000},
      {"Kopi Susu Gula Aren",  18000},
      {"Jus Alpukat",          22000},
      {"Lemon Tea",            15000},
      {"Air Mineral",           5000}
  };
  menuData["Cemilan"] = {
      {"Kentang Goreng",  20000},
      {"Cireng Crispy",   15000},
      {"Tahu Crispy",     12000},
      {"Kerupuk Udang",    8000}
  };
  menuData["Dessert"] = {
      {"Pudding Coklat",    25000},
      {"Ice Cream Vanilla", 18000},
      {"Es Cendol",         15000}
  };

  QHBoxLayout *filterLayout = new QHBoxLayout;
  filterLayout->setSpacing(10);

  QLineEdit *searchBar = new QLineEdit;
  searchBar->setPlaceholderText("🔍  Cari menu...");
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

  // Lambda update katalog
  auto updateCatalog = [=]() {
    catalogList->clear();
    QListWidgetItem *catItem = categoryList->currentItem();
    if (!catItem) return;

    QString selectedCategory = catItem->text();
    QList<FoodItem> rawItems;
    if (selectedCategory == "Semua") {
        for (auto it = menuData.begin(); it != menuData.end(); ++it)
            rawItems.append(it.value());
    } else {
        if (!menuData.contains(selectedCategory)) return;
        rawItems = menuData[selectedCategory];
    }

    QString searchText = searchBar->text();
    int sortOption     = sortCombo->currentIndex();

    HashMap menuMap(100);
    for (const auto &item : rawItems)
        menuMap.insert(item.name, item);

    QList<FoodItem> items;
    QString cleanQuery = searchText.trimmed();
    const FoodItem *exactItem = menuMap.get(cleanQuery);
    if (exactItem != nullptr) {
        items.append(*exactItem);
    } else {
        items = menuMap.searchPartial(cleanQuery);
    }
    MenuAlgorithms::sort(items, sortOption);

    for (const auto &item : items) {
        QString formattedPrice = QString::number(item.price);
        int len = formattedPrice.length();
        for (int i = len - 3; i > 0; i -= 3)
            formattedPrice.insert(i, '.');
        catalogList->addItem(
            QString("%1  Rp %2").arg(item.name.leftJustified(30, ' ')).arg(formattedPrice));
        // Gunakan item text yang simple & mudah di-parse
        catalogList->item(catalogList->count() - 1)
            ->setData(Qt::UserRole,      item.name);
        catalogList->item(catalogList->count() - 1)
            ->setData(Qt::UserRole + 1, item.price);
    }
  };

  connect(categoryList, &QListWidget::itemSelectionChanged, this, updateCatalog);
  connect(searchBar, &QLineEdit::textChanged, this, [=](const QString &) { updateCatalog(); });
  connect(sortCombo, &QComboBox::currentIndexChanged, this, [=](int) { updateCatalog(); });
  categoryList->setCurrentRow(0);

  rightPanel->addWidget(lblCatalog);
  rightPanel->addLayout(filterLayout);
  rightPanel->addWidget(catalogList);

  contentLayout->addLayout(leftPanel, 1);
  contentLayout->addLayout(rightPanel, 2);

  // --- Tombol Bawah ---
  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->setSpacing(15);
  QPushButton *btnAddToCart = new QPushButton("➕  Tambah ke Keranjang");
  btnViewCart = new QPushButton("🛒  Keranjang (0)");
  QPushButton *btnHome      = new QPushButton("← Beranda");

  btnHome->setObjectName("secondaryBtn");
  btnAddToCart->setObjectName("primaryBtn");
  btnViewCart->setObjectName("accentBtn");

  bottomLayout->addWidget(btnHome);
  bottomLayout->addStretch();
  bottomLayout->addWidget(btnAddToCart);
  bottomLayout->addWidget(btnViewCart);

  mainLayout->addLayout(contentLayout);
  mainLayout->addLayout(bottomLayout);

  // Tambah ke Keranjang (gunakan UserRole data)
  connect(btnAddToCart, &QPushButton::clicked, this, [=]() {
    QListWidgetItem *sel = catalogList->currentItem();
    if (sel) {
      QString name = sel->data(Qt::UserRole).toString();
      int     price = sel->data(Qt::UserRole + 1).toInt();
      if (name.isEmpty()) {
          // fallback: parse dari text lama
          QString text = sel->text();
          name  = text.split("  Rp ")[0].trimmed();
          price = text.split("  Rp ")[1].remove(".").toInt();
      }
      cartItems.append({name, price});
      btnViewCart->setText(
          QString("🛒  Keranjang (%1)").arg(cartItems.size()));
    } else {
      QMessageBox::warning(this, "Pilih",
                           "Silakan pilih menu dari katalog terlebih dahulu.");
    }
  });

  connect(btnViewCart, &QPushButton::clicked, this, [=]() {
    updateCartUI();
    stackedWidget->setCurrentIndex(2);
  });
  connect(btnHome, &QPushButton::clicked, this,
          [=]() { stackedWidget->setCurrentIndex(0); });
}


