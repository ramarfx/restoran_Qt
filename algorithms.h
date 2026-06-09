#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "models.h"
#include <QList>

namespace MenuAlgorithms {
    // Linear Search: Mencari item berdasarkan kecocokan nama (case-insensitive substring)
    QList<FoodItem> search(const QList<FoodItem>& items, const QString& query);

    // Bubble Sort: Mengurutkan list secara in-place
    // sortOption: 1 (Harga Naik), 2 (Harga Turun), 3 (Nama A-Z), 4 (Nama Z-A)
    void sort(QList<FoodItem>& items, int sortOption);
}

#endif // ALGORITHMS_H
