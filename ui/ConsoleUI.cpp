#include <iostream>
#include <chrono>

#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"
#include "BitSequence.h"
#include "SequenceStats.h"

// Функции для функциональных операций
bool PredicateEven(const int& x) { return x % 2 == 0; }
int TransformSquare(const int& x) { return x * x; }
int AccumulateSum(const int& acc, const int& x) { return acc + x; }

void OutputSequence(const Sequence<int>* seq) {
    if (seq == nullptr || seq->GetLength() == 0) {
        std::cout << "Последовательность пуста.\n";
        return;
    }
    std::cout << "Элементы: [ ";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i) << " ";
    }
    std::cout << "] (всего: " << seq->GetLength() << ")\n";
}

void OutputBitMask(const BitSequence* bits) {
    if (bits == nullptr || bits->GetLength() == 0) {
        std::cout << "Маска пуста.\n";
        return;
    }
    std::cout << "Биты: ";
    for (int i = 0; i < bits->GetLength(); ++i) {
        std::cout << (bits->GetBit(i) ? '1' : '0');
    }
    std::cout << "\n";
}

Sequence<int>* BuildSequenceInteractive(bool& isMutable) {
    std::cout << "\n--- Создание последовательности ---\n";
    std::cout << "1. Массив (DynamicArray)\n2. Связный список (LinkedList)\nВыбор: ";
    int containerChoice;
    std::cin >> containerChoice;

    std::cout << "1. Изменяемая (Mutable)\n2. Неизменяемая (Immutable)\nВыбор: ";
    int mutChoice;
    std::cin >> mutChoice;
    isMutable = (mutChoice == 1);

    std::cout << "Количество начальных элементов: ";
    int count;
    std::cin >> count;

    DynamicArray<int> buffer(count > 0 ? count : 0);
    for (int i = 0; i < count; ++i) {
        std::cout << "Элемент " << i << ": ";
        int val;
        std::cin >> val;
        buffer.Set(i, val);
    }

    if (containerChoice == 2) {
        LinkedList<int> listBuf;
        for (int i = 0; i < count; ++i) {
            listBuf.Append(buffer.Get(i));
        }
        if (isMutable) return new MutableListSequence<int>(listBuf);
        return new ImmutableListSequence<int>(listBuf);
    }

    if (isMutable) return new MutableArraySequence<int>(buffer);
    return new ImmutableArraySequence<int>(buffer);
}

void ExecuteSequenceMenu() {
    bool isMutable = true;
    Sequence<int>* currentSeq = BuildSequenceInteractive(isMutable);

    while (true) {
        std::cout << "\n[Меню Sequence: " << (isMutable ? "Mutable" : "Immutable") << "]\n"
                  << "1. Добавить в конец (Append)\n"
                  << "2. Добавить в начало (Prepend)\n"
                  << "3. Вставить по индексу (InsertAt)\n"
                  << "4. Прочитать элемент по индексу\n"
                  << "5. Получить первый и последний элементы\n"
                  << "6. Вырезать подпоследовательность\n"
                  << "7. Сцепить (Concat) с новой последовательностью\n"
                  << "8. Фильтрация Where (только четные)\n"
                  << "9. Преобразование Map (возведение в квадрат)\n"
                  << "10. Свертка Reduce (сумма элементов)\n"
                  << "11. Статистика Min/Max/Avg (за 1 проход)\n"
                  << "12. Вывести текущие данные\n"
                  << "0. Вернуться в главное меню\n"
                  << "Пункт > ";

        int option;
        std::cin >> option;
        if (option == 0) break;

        try {
            if (option == 1) {
                std::cout << "Значение: ";
                int v; std::cin >> v;
                Sequence<int>* next = currentSeq->Append(v);
                if (!isMutable) delete currentSeq;
                currentSeq = next;
                OutputSequence(currentSeq);
            } else if (option == 2) {
                std::cout << "Значение: ";
                int v; std::cin >> v;
                Sequence<int>* next = currentSeq->Prepend(v);
                if (!isMutable) delete currentSeq;
                currentSeq = next;
                OutputSequence(currentSeq);
            } else if (option == 3) {
                std::cout << "Значение: ";
                int v; std::cin >> v;
                std::cout << "Индекс: ";
                int idx; std::cin >> idx;
                Sequence<int>* next = currentSeq->InsertAt(v, idx);
                if (!isMutable) delete currentSeq;
                currentSeq = next;
                OutputSequence(currentSeq);
            } else if (option == 4) {
                std::cout << "Индекс: ";
                int idx; std::cin >> idx;
                std::cout << "Элемент [" << idx << "] = " << currentSeq->Get(idx) << "\n";
            } else if (option == 5) {
                std::cout << "Первый: " << currentSeq->GetFirst() 
                          << ", Последний: " << currentSeq->GetLast() << "\n";
            } else if (option == 6) {
                std::cout << "Индекс начала: ";
                int s; std::cin >> s;
                std::cout << "Индекс конца: ";
                int e; std::cin >> e;
                Sequence<int>* sub = currentSeq->GetSubSequence(s, e);
                std::cout << "Срез: ";
                OutputSequence(sub);
                delete sub;
            } else if (option == 7) {
                std::cout << "Сколько элементов во второй последовательности: ";
                int k; std::cin >> k;
                DynamicArray<int> tempArr(k);
                for (int i = 0; i < k; ++i) {
                    std::cout << "Элемент " << i << ": ";
                    int v; std::cin >> v;
                    tempArr.Set(i, v);
                }
                MutableArraySequence<int> second(tempArr);
                Sequence<int>* joined = currentSeq->Concat(&second);
                if (!isMutable) delete currentSeq;
                currentSeq = joined;
                OutputSequence(currentSeq);
            } else if (option == 8) {
                Sequence<int>* filtered = currentSeq->Where(PredicateEven);
                std::cout << "Четные числа: ";
                OutputSequence(filtered);
                delete filtered;
            } else if (option == 9) {
                Sequence<int>* mapped = currentSeq->Map(TransformSquare);
                std::cout << "Квадраты: ";
                OutputSequence(mapped);
                delete mapped;
            } else if (option == 10) {
                int total = currentSeq->Reduce<int>(AccumulateSum, 0);
                std::cout << "Сумма всех элементов: " << total << "\n";
            } else if (option == 11) {
                MinMaxAvg<int> stats = ComputerMinMaxAvg(currentSeq);
                std::cout << "Минимум:  " << stats.min << "\n"
                          << "Максимум: " << stats.max << "\n"
                          << "Среднее:  " << stats.avg << "\n";
            } else if (option == 12) {
                OutputSequence(currentSeq);
            } else {
                std::cout << "Неизвестный пункт.\n";
            }
        } catch (const std::exception& err) {
            std::cout << "Исключение: " << err.what() << "\n";
        }
    }

    delete currentSeq;
}

void ExecuteBitMenu() {
    try {
        std::cout << "\n--- Работа с BitSequence ---\n";
        std::cout << "Длина первой маски: ";
        int len; std::cin >> len;

        DynamicArray<Bit> raw(len);
        for (int i = 0; i < len; ++i) {
            std::cout << "Бит " << i << " (0/1): ";
            int b; std::cin >> b;
            raw.Set(i, Bit(b != 0));
        }

        BitSequence mask1(raw);
        OutputBitMask(&mask1);

        // Инверсия
        BitSequence* inverted = mask1.Not();
        std::cout << "NOT (инверсия): ";
        OutputBitMask(inverted);
        delete inverted;

        // Побитовое И с тестовой маской (чередование 1 и 0)
        DynamicArray<Bit> raw2(len);
        for (int i = 0; i < len; ++i) {
            raw2.Set(i, Bit(i % 2 == 0));
        }
        BitSequence mask2(raw2);
        std::cout << "Вторая маска (1010...): ";
        OutputBitMask(&mask2);

        BitSequence* andRes = mask1.And(mask2);
        std::cout << "Результат AND:          ";
        OutputBitMask(andRes);
        delete andRes;

    } catch (const std::exception& err) {
        std::cout << "Ошибка BitSequence: " << err.what() << "\n";
    }
}

void ExecuteBenchmark() {
    std::cout << "\n--- Замер скорости Append ---\n";
    std::cout << "Количество вставок (например, 25000): ";
    int iterations;
    std::cin >> iterations;

    MutableArraySequence<int> arraySeq;
    MutableListSequence<int> listSeq;

    auto tStartArr = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) arraySeq.Append(i);
    auto tEndArr = std::chrono::high_resolution_clock::now();

    auto tStartList = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) listSeq.Append(i);
    auto tEndList = std::chrono::high_resolution_clock::now();

    auto timeArr = std::chrono::duration_cast<std::chrono::milliseconds>(tEndArr - tStartArr).count();
    auto timeList = std::chrono::duration_cast<std::chrono::milliseconds>(tEndList - tStartList).count();

    std::cout << "Время DynamicArray: " << timeArr << " мс\n"
              << "Время LinkedList:   " << timeList << " мс\n";
}

int main() {
    while (true) {
        std::cout << "\n========== ЛАБОРАТОРНАЯ РАБОТА 2 ==========\n"
                  << "1. Тестирование последовательностей (Sequence)\n"
                  << "2. Тестирование битовых масок (BitSequence)\n"
                  << "3. Тест производительности (Benchmark)\n"
                  << "0. Завершить работу\n"
                  << "Выбор > ";

        int mainChoice;
        std::cin >> mainChoice;

        if (mainChoice == 0) break;
        if (mainChoice == 1) ExecuteSequenceMenu();
        else if (mainChoice == 2) ExecuteBitMenu();
        else if (mainChoice == 3) ExecuteBenchmark();
        else std::cout << "Некорректная команда.\n";
    }

    std::cout << "Программа завершена.\n";
    return 0;
}