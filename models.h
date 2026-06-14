#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QList>

struct FoodItem {
  QString name;
  int price;
};

struct Order {
  int orderId;
  QList<FoodItem> items;
  QString status;
};

#endif