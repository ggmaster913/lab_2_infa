#ifndef MUTABLEARRAYSEQUENCE_H
#define MUTABLEARRAYSEQUENCE_H

#include "ArraySequence.h"

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }

    ArraySequence<T>* Instance() override {
        return this;
    }

    ArraySequence<T>* CreateInstance(const DynamicArray<T>& subArray) const override{
        return new MutableArraySequence<T>(subArray);
    }

public:
    // конструкторы не наследуются, поэтому передаём их в ArraySequence явно
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(const T* data, int count) : ArraySequence<T>(data, count) {}
    MutableArraySequence(const DynamicArray<T>& array) : ArraySequence<T>(array) {}
    MutableArraySequence(const LinkedList<T>& list) : ArraySequence<T>(list) {}

    // const версию пишем явно, иначе из за T& operator[] не перенеслось из Sequence тк есть уже оператор 
    const T& operator[](int index) const {
        return this->Get(index);
    }

    T& operator[](int index){
        return (*this->items)[index];
    }

    
};

// map всегда собирает MutableArraySequence: какой тип у исходной, шаблонный метод не знает
template<typename T>
template<typename T2>
Sequence<T2>* Sequence<T>::Map(T2 (*f)(const T&)) const {
    MutableArraySequence<T2>* result = new MutableArraySequence<T2>();
    int len = this->GetLength();
    for (int i = 0; i < len; i++){
        result->Append(f(this->Get(i)));
    }
    return result;
}

#endif