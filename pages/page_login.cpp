#include "restaurantapp.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

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
