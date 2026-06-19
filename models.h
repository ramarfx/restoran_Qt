#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QList>

// ─── Domain Models ───────────────────────────────────────────────────────────

struct FoodItem {
    QString name;
    int price;
};

struct Order {
    int orderId;
    QList<FoodItem> items;  // Items inside an order remain a plain QList
    QString status;
};

// ─── Custom Doubly Linked List ────────────────────────────────────────────────
// Menggantikan QList<T> untuk Keranjang Belanja dan Riwayat Transaksi Pelayan.
// Setiap node menyimpan pointer ke node sebelumnya (prev) dan sesudahnya (next),
// sehingga operasi insert/delete di ujung manapun berjalan dalam O(1).

template<typename T>
struct DLLNode {
    T data;
    DLLNode<T>* prev;
    DLLNode<T>* next;
    explicit DLLNode(const T& d) : data(d), prev(nullptr), next(nullptr) {}
};

template<typename T>
class DoublyLinkedList {
public:
    DLLNode<T>* head;
    DLLNode<T>* tail;

    DoublyLinkedList() : head(nullptr), tail(nullptr), _size(0) {}

    ~DoublyLinkedList() { clear(); }

    // Tambah elemen di ekor — O(1)
    void append(const T& item) {
        DLLNode<T>* node = new DLLNode<T>(item);
        if (!tail) {
            head = tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        ++_size;
    }

    // Akses elemen berdasarkan indeks — O(n)
    T& operator[](int index) { return nodeAt(index)->data; }
    const T& operator[](int index) const { return nodeAt(index)->data; }

    // Ambil dan hapus elemen pada indeks — O(n)
    T takeAt(int index) {
        DLLNode<T>* node = nodeAt(index);
        T val = node->data;
        unlink(node);
        return val;
    }

    // Hapus elemen pada indeks — O(n)
    void removeAt(int index) { takeAt(index); }

    int  size()    const { return _size; }
    bool isEmpty() const { return _size == 0; }

    // Hapus semua elemen dan bebaskan memori — O(n)
    void clear() {
        DLLNode<T>* cur = head;
        while (cur) {
            DLLNode<T>* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        _size = 0;
    }

    // Konversi ke QList (untuk kompatibilitas, misal mengisi Order.items)
    QList<T> toQList() const {
        QList<T> list;
        DLLNode<T>* cur = head;
        while (cur) { list.append(cur->data); cur = cur->next; }
        return list;
    }

    // ── Range-based for loop support ─────────────────────────────────────────
    struct Iterator {
        DLLNode<T>* cur;
        explicit Iterator(DLLNode<T>* n) : cur(n) {}
        T& operator*() { return cur->data; }
        Iterator& operator++() { cur = cur->next; return *this; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };
    struct ConstIterator {
        DLLNode<T>* cur;
        explicit ConstIterator(DLLNode<T>* n) : cur(n) {}
        const T& operator*() const { return cur->data; }
        ConstIterator& operator++() { cur = cur->next; return *this; }
        bool operator!=(const ConstIterator& o) const { return cur != o.cur; }
    };
    Iterator      begin()        { return Iterator(head); }
    Iterator      end()          { return Iterator(nullptr); }
    ConstIterator begin()  const { return ConstIterator(head); }
    ConstIterator end()    const { return ConstIterator(nullptr); }

private:
    int _size;

    // Navigasi ke node pada indeks — O(n)
    DLLNode<T>* nodeAt(int index) const {
        DLLNode<T>* cur = head;
        for (int i = 0; i < index && cur; ++i) cur = cur->next;
        return cur;
    }

    // Putuskan node dari rantai dan hapus — O(1)
    void unlink(DLLNode<T>* node) {
        if (node->prev) node->prev->next = node->next;
        else            head = node->next;
        if (node->next) node->next->prev = node->prev;
        else            tail = node->prev;
        delete node;
        --_size;
    }
};

#endif // MODELS_H