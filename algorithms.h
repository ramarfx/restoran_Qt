#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "models.h"
#include <QList>

namespace MenuAlgorithms {
    QList<FoodItem> search(const QList<FoodItem>& items, const QString& query);
    void sort(QList<FoodItem>& items, int sortOption);
}

#endif
