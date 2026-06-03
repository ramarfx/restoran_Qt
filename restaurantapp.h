#ifndef RESTAURANTAPP_H
#define RESTAURANTAPP_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>
#include <QStack>
#include <QQueue>
#include <QList>
#include <QScrollArea>
#include <QVBoxLayout>
#include "models.h"

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

  // Komponen Dinamis
  QListWidget *cartListWidget;
  QListWidget *cashierListWidget;
  QScrollArea *kitchenScrollArea;
  QWidget *kitchenQueueContainer;
  QVBoxLayout *kitchenQueueLayout;
  QListWidget *waiterListWidget;
  QLabel *currentCookingLabel;

  // Struktur Data Sesuai Flowchart
  QStack<FoodItem> cartStack; // STACK: Keranjang (LIFO untuk Undo)
  QQueue<Order> kitchenQueue; // QUEUE: Antrean Dapur (FIFO)
  QList<Order> cashierOrders; // List Kasir (Menunggu Pembayaran)
  QList<Order> waiterOrders;  // List Pelayan (Siap Antar)

  Order currentCookingOrder;
  bool isCooking = false;
  int orderCounter;

  // Helper functions for UI setup
  void setupUI();
  void createHomePage();
  void createConsumerPage();
  void createCartPage();
  void createLoginPage();
  void createCashierPage();
  void createKitchenPage();
  void createWaiterPage();

  // Helper functions for updating UI
  void updateCartUI();
  void updateCashierUI();
  void updateKitchenUI();
  void updateWaiterUI();

  // Apply QSS stylesheet
  void applyStyles();
};

#endif // RESTAURANTAPP_H
