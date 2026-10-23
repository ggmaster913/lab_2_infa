#ifndef SEQUENCESTATS_H
#define SEQUENCESTATS_H

#include "Sequence.h"

template<typename T>
struct MinMaxAvg {
    T min;
    T max;
    double avg;
};

template<typename T>
struct MinMaxAvgAccumulator {
    T min;
    T max;
    T sum;
    double count; // double для того чтобы деление получалось дробным
};

template<typename T>
MinMaxAvgAccumulator<T> CombineMinMaxAvg(const MinMaxAvgAccumulator<T>& acc, const T& item);

template<typename T>
MinMaxAvg<T> ComputeMinMaxAvg(const Sequence<T>* seq);

// тела шаблона должны быть видны там, где подставляется T, поэтому не .cpp
#ifndef SEQUENCESTATS_TPP // если открыт сам .tpp, он уже подключает этот .h, по кругу не надо
#include "SequenceStats.tpp"
#endif

#endif
