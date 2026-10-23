#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template <typename T>
class LinkedList{
    struct Node{
        T data;
        Node* next;
    };
    Node* head;
    Node* tail;
    int length;
public:
    LinkedList(const T* items, int count);
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();

    // a = b = c
    LinkedList<T>& operator=(const LinkedList<T>& other);

    T& operator[](int index);
    // для константного списка: только чтение, как и в DynamicArray
    const T& operator[](int index) const;

    const T& GetFirst() const;
    const T& GetLast() const;
    const T& Get(int index) const;
    int GetLength() const;
    LinkedList<T>* GetSubList(int stIndex, int endIndex) const;

    void Append(const T& item);
    void Prepend(const T& item);
    void InsertAt(const T& item, int index);
    LinkedList<T>* Concat(LinkedList<T>* list) const;
};

// тела шаблона должны быть видны там, где подставляется T, поэтому не .cpp
#ifndef LINKEDLIST_TPP // если открыт сам .tpp, он уже подключает этот .h, по кругу не надо
#include "LinkedList.tpp"
#endif

#endif
