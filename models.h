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

template<typename T>
struct Node {
    T data;
    Node* prev = nullptr;
    Node* next = nullptr;
};

template<typename T>
class DoublyLinkedList {
public:
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    int _size = 0;

    ~DoublyLinkedList() { clear(); }

    void append(const T& item) {
        Node<T>* newNode = new Node<T>{item, tail, nullptr};
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        _size++;
    }

    T takeAt(int index) {
        Node<T>* curr = head;
        for (int i = 0; i < index && curr != nullptr; ++i) {
            curr = curr->next;
        }
        T val = curr->data;

        if (curr->prev != nullptr) {
            curr->prev->next = curr->next;
        } else {
            head = curr->next;
        }

        if (curr->next != nullptr) {
            curr->next->prev = curr->prev;
        } else {
            tail = curr->prev;
        }

        delete curr;
        _size--;
        return val;
    }

    void removeAt(int index) {
        takeAt(index);
    }

    int size() const { return _size; }
    bool isEmpty() const { return _size == 0; }

    void clear() {
        Node<T>* curr = head;
        while (curr != nullptr) {
            Node<T>* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        head = tail = nullptr;
        _size = 0;
    }

    QList<T> toQList() const {
        QList<T> list;
        Node<T>* curr = head;
        while (curr != nullptr) {
            list.append(curr->data);
            curr = curr->next;
        }
        return list;
    }

    struct Iterator {
        Node<T>* node;
        T& operator*() { return node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    struct ConstIterator {
        Node<T>* node;
        const T& operator*() const { return node->data; }
        ConstIterator& operator++() { node = node->next; return *this; }
        bool operator!=(const ConstIterator& other) const { return node != other.node; }
    };

    Iterator begin() { return Iterator{head}; }
    Iterator end() { return Iterator{nullptr}; }
    ConstIterator begin() const { return ConstIterator{head}; }
    ConstIterator end() const { return ConstIterator{nullptr}; }
};

#endif