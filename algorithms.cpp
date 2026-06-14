#include "algorithms.h"

namespace MenuAlgorithms {

QList<FoodItem> search(const QList<FoodItem>& items, const QString& query) {
    QList<FoodItem> result;
    QString lowerQuery = query.trimmed().toLower();
    
    // Algoritma Linear Search
    for (int i = 0; i < items.size(); ++i) {
        if (lowerQuery.isEmpty() || items[i].name.toLower().contains(lowerQuery)) {
            result.append(items[i]);
        }
    }
    return result;
}

void sort(QList<FoodItem>& items, int sortOption) {
    if (items.size() <= 1) return;

    // Algoritma Bubble Sort
    int n = items.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            bool shouldSwap = false;
            
            if (sortOption == 1) { // Harga Terendah ke Tertinggi (Ascending)
                if (items[j].price > items[j + 1].price) {
                    shouldSwap = true;
                }
            } else if (sortOption == 2) { // Harga Tertinggi ke Terendah (Descending)
                if (items[j].price < items[j + 1].price) {
                    shouldSwap = true;
                }
            } else if (sortOption == 3) { // Nama A-Z (Ascending)
                if (items[j].name.localeAwareCompare(items[j + 1].name) > 0) {
                    shouldSwap = true;
                }
            } else if (sortOption == 4) { // Nama Z-A (Descending)
                if (items[j].name.localeAwareCompare(items[j + 1].name) < 0) {
                    shouldSwap = true;
                }
            }
            
            if (shouldSwap) {
                // Proses swap (pertukaran elemen)
                FoodItem temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
}

}
