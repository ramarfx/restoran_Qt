#include "algorithms.h"

HashMap::HashMap(int cap) {
  capacity = cap;
  numOfElements = 0;
  arr = new HashNode *[capacity]();
}

HashMap::~HashMap() {
  clear();
  delete[] arr;
}

void HashMap::clear() {
  for (int i = 0; i < capacity; ++i) {
    HashNode *curr = arr[i];
    while (curr != nullptr) {
      HashNode *prev = curr;
      curr = curr->next;
      delete prev;
    }
    arr[i] = nullptr;
  }
  numOfElements = 0;
}

int HashMap::hashFunction(const QString &key) const {
  int sum = 0;
  int factor = 31;
  for (int i = 0; i < key.length(); i++) {
    sum = (sum * factor + key[i].unicode()) % capacity;
  }
  return sum >= 0 ? sum : -sum;
}

// Menambahkan data baru ke dalam Hash Map
void HashMap::insert(const QString &key, const FoodItem &value) {
  int bucketIndex = hashFunction(key.toLower());

  HashNode *newNode = new HashNode;
  newNode->key = key.toLower();
  newNode->value = value;
  newNode->next = nullptr;

  if (arr[bucketIndex] == nullptr) {
    arr[bucketIndex] = newNode;
  } else {
    newNode->next = arr[bucketIndex];
    arr[bucketIndex] = newNode;
  }
  numOfElements++;
}

const FoodItem *HashMap::get(const QString &key) const {
  int bucketIndex = hashFunction(key.toLower());
  HashNode *curr = arr[bucketIndex];
  while (curr != nullptr) {
    if (curr->key == key.toLower()) {
      return &(curr->value);
    }
    curr = curr->next;
  }
  return nullptr;
}

QList<FoodItem> HashMap::searchPartial(const QString &query) const {
  QList<FoodItem> result;
  QString lowerQuery = query.trimmed().toLower();
  for (int i = 0; i < capacity; ++i) {
    HashNode *curr = arr[i];
    while (curr != nullptr) {
      // Jika query kosong atau nama makanan mengandung query
      if (lowerQuery.isEmpty() || curr->key.contains(lowerQuery)) {
        result.append(curr->value);
      }
      curr = curr->next;
    }
  }
  return result;
}

namespace MenuAlgorithms {
QList<FoodItem> search(const QList<FoodItem> &items, const QString &query) {
  HashMap menuMap(100);

  for (const auto &item : items) {
    menuMap.insert(item.name, item);
  }

  QString cleanQuery = query.trimmed();
  const FoodItem *exactItem = menuMap.get(cleanQuery);
  if (exactItem != nullptr) {
    return {*exactItem};
  }

  return menuMap.searchPartial(cleanQuery);
}

int partition(QList<FoodItem> &items, int low, int high, int sortOption) {
  FoodItem pivot = items[high];
  int i = low - 1;

  for (int j = low; j < high; j++) {
    bool shouldSwap = false;

    if (sortOption == 1) { // Harga Terendah -> Tertinggi
      shouldSwap = items[j].price < pivot.price;
    } else if (sortOption == 2) { // Harga Tertinggi -> Terendah
      shouldSwap = items[j].price > pivot.price;
    } else if (sortOption == 3) { //  A -> Z
      shouldSwap = items[j].name.localeAwareCompare(pivot.name) < 0;
    } else if (sortOption == 4) { //  Z -> A
      shouldSwap = items[j].name.localeAwareCompare(pivot.name) > 0;
    }

    if (shouldSwap) {
      i++;
      FoodItem temp = items[i];
      items[i] = items[j];
      items[j] = temp;
    }
  }
  FoodItem temp = items[i + 1];
  items[i + 1] = items[high];
  items[high] = temp;
  return i + 1;
}

void quickSort(QList<FoodItem> &items, int low, int high, int sortOption) {
  if (low < high) {
    int pi = partition(items, low, high, sortOption);
    quickSort(items, low, pi - 1, sortOption);
    quickSort(items, pi + 1, high, sortOption);
  }
}

void sort(QList<FoodItem> &items, int sortOption) {
  if (items.size() <= 1)
    return;
  quickSort(items, 0, items.size() - 1, sortOption);
}

} // namespace MenuAlgorithms