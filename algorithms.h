#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "models.h"
#include <QList>
#include <QString>

struct HashNode {
    QString key;       
    FoodItem value;    
    HashNode* next;  
};

class HashMap {
public:
    int numOfElements;
    int capacity;
    HashNode** arr;  

    HashMap(int cap = 100);
    ~HashMap();

    void clear();
    int hashFunction(const QString& key) const;
    void insert(const QString& key, const FoodItem& value);
    const FoodItem* get(const QString& key) const;
    QList<FoodItem> searchPartial(const QString& query) const;
};

namespace MenuAlgorithms {
    QList<FoodItem> search(const QList<FoodItem>& items, const QString& query);
    void sort(QList<FoodItem>& items, int sortOption);
}

#endif 
