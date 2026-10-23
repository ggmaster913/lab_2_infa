#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"
#include "DynamicArray.h"

template<typename T>
class ListSequence : public Sequence<T>{
protected:
    LinkedList<T>* items;

    virtual ListSequence<T>* Clone() const = 0;
    virtual ListSequence<T>* Instance() = 0;
    virtual ListSequence<T>* CreateInstance(const LinkedList<T>& subList) const = 0;

    ListSequence<T>* AppendInternal(const T& item);
    ListSequence<T>* PrependInternal(const T& item);
    ListSequence<T>* InsertAtInternal(const T& item, int index);
    ListSequence<T>* ConcatInternal(const Sequence<T>* list);
public:
    ListSequence();
    ListSequence(const T* data, int count);
    ListSequence(const ListSequence<T>& other);
    ListSequence(const LinkedList<T>& other);
    ListSequence(const DynamicArray<T>& other);

    // a = b = c
    ListSequence<T>& operator=(const ListSequence<T>& other);

    ~ListSequence() override;

    const T& GetFirst() const override;
    const T& GetLast() const override;
    int GetLength() const override;
    const T& Get(int index) const override;

    ListSequence<T>* Append(const T& item) override;
    ListSequence<T>* Prepend(const T& item) override;
    ListSequence<T>* InsertAt(const T& item, int index) override;
    ListSequence<T>* Concat(const Sequence<T>* other) override;

    ListSequence<T>* GetSubSequence(int startIndex, int endIndex) const override;
    Sequence<T>* Where(bool (*predicate)(const T&)) const override;
};

// тела шаблона должны быть видны там, где подставляется T, поэтому не .cpp
#ifndef LISTSEQUENCE_TPP // если открыт сам .tpp, он уже подключает этот .h, по кругу не надо
#include "ListSequence.tpp"
#endif

#endif
