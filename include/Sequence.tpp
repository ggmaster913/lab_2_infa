#ifndef SEQUENCE_TPP
#define SEQUENCE_TPP

#include "Sequence.h" // для редактора: он открывает .tpp отдельно и без этого не знает класс

// два template: первый от класса Sequence<T>, второй от самого метода (T2)
template<typename T>
template<typename T2>
T2 Sequence<T>::Reduce(T2 (*f)(const T2&, const T&), const T2& initial) const {
    T2 accum = initial;
    int len = this->GetLength();
    for (int i = 0; i < len; i++){
        accum = f(accum, this->Get(i)); // тут и вызывается переданная функция
    }
    return accum;
}

template<typename T>
const T& Sequence<T>::operator[](int index) const {
    return this->Get(index);
}

#endif
