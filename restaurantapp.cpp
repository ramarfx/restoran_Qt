#include "restaurantapp.h"

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
