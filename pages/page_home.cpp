#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

// 1. HOME PAGE
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
