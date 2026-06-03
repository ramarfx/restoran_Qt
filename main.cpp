#include <QApplication>
#include "restaurantapp.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  RestaurantApp window;
  window.show();

  return app.exec();
}