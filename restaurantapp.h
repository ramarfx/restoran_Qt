#ifndef RESTAURANTAPP_H
#define RESTAURANTAPP_H

#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

#include "models.h"
#include <QDir>
#include <QList>
#include <QPixmap>
#include <QQueue>
#include <QScrollArea>
#include <QVBoxLayout>


class RestaurantApp : public QMainWindow {
  Q_OBJECT

public:
  explicit RestaurantApp(QWidget *parent = nullptr);

private:
  // UI Elements
  QStackedWidget *stackedWidget;

  // Halaman
  QWidget *homePage;
  QWidget *consumerPage;
  QWidget *cartPage;
  QWidget *loginPage;
  QWidget *cashierPage;
  QWidget *kitchenPage;
  QWidget *waiterPage;

  // Komponen
  QListWidget *cartListWidget;
  QPushButton *btnViewCart;
  QLabel      *cartTotalLabel;
  QListWidget *cashierListWidget;
  QLabel      *cashierDetailLabel;
  QScrollArea *kitchenScrollArea;
  QWidget     *kitchenQueueContainer;
  QVBoxLayout *kitchenQueueLayout;
  QListWidget *waiterListWidget;
  QLabel      *currentCookingLabel;

  // Info Customer saat ini (sesi pemesanan)
  QString currentCustomerName;
  int     currentTableNumber;   // 0 = takeaway
  int     currentQueueNumber;

  // Label info customer di consumer page
  QLabel *customerInfoLabel;

  // Struktur Data
  // DLL: Keranjang Belanja
  DoublyLinkedList<FoodItem> cartItems;
  // QUEUE: Antrean Dapur
  QQueue<Order> kitchenQueue;
  // QList: Antrean Kasir
  QList<Order> cashierOrders;
  // DLL: Riwayat Transaksi Pelayan
  DoublyLinkedList<Order> waiterOrders;

  Order currentCookingOrder;
  bool  isCooking = false;
  int   orderCounter;
  int   queueCounter;

  void setupUI();
  void createHomePage();
  void createConsumerPage();
  void createCartPage();
  void createLoginPage();
  void createCashierPage();
  void createKitchenPage();
  void createWaiterPage();

  void updateCartUI();
  void updateCashierUI();
  void updateKitchenUI();
  void updateWaiterUI();

  // Apply QSS stylesheet
  void applyStyles();
};

#endif
