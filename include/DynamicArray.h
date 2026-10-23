#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "ICollection.h"

template <typename T>
class DynamicArray : public ICollection<T> {
    T* data;
    int size;     // сколько элементов реально есть
    int capacity; // под сколько элементов выделена память, всегда >= size
public:
    explicit DynamicArray(int size); //запрет неявного преобразования типов. чтоб выдавало ошибку при, (PrintArray(6))
    DynamicArray(const T* items, int count);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray();

    const T& Get(int index) const override; // чтоб нельзя было сделать arr.Get(5) = 8;
    int GetSize() const;
    int GetCapacity() const;
    void Set(int index, const T& value);

    // меняет размер до newSize; память выделяется с запасом (вдвое), поэтому Append в среднем O(1)
    void Resize(int newSize);

    // a = b = c
    DynamicArray<T>& operator=(const DynamicArray<T>& other);

    T& operator[](int index);
    // для константного массива: только чтение, иначе через [] можно было бы менять const объект
    const T& operator[](int index) const;

    int GetCount() const override;
};

// тела шаблона должны быть видны там, где подставляется T, поэтому не .cpp
#ifndef DYNAMICARRAY_TPP // если открыт сам .tpp, он уже подключает этот .h, по кругу не надо
#include "DynamicArray.tpp"
#endif

#endif
