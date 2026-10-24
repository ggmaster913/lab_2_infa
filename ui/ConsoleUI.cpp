#include <iostream>
#include <chrono>
#include <limits>
#include <cstdlib>

#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"
#include "BitSequence.h"
#include "SequenceStats.h"

// читает целое число, при ошибке просит ввести снова
int ReadInt() {
    int value;
    while (!(std::cin >> value)) { // >> берёт цифры подряд, пока идут: 12abc вернёт 12, а abc останется в буфере на следующий раз
        if (std::cin.eof()) { // ввод закончился совсем (Ctrl+D), читать больше нечего
            std::cout << "\nКонец ввода. Завершение работы.\n";
            std::exit(0); // сразу завершает программу, деструкторы локальных объектов не вызываются, память освободит ОС
        }
        std::cin.clear(); // снимает с cin состояние ошибки, иначе он так и останется в ошибке и дальше ничего не прочитает
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // выкидываем остаток строки с неправильным вводом, иначе >> снова споткнётся о него
        std::cout << "Ошибка: нужно целое число. Повторите ввод: ";
    }
    return value;
}

// передаются в Where, Map и Reduce
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
    int containerChoice = ReadInt();
    while (containerChoice != 1 && containerChoice != 2) {
        std::cout << "Нужно 1 или 2. Повторите ввод: ";
        containerChoice = ReadInt();
    }

    std::cout << "1. Изменяемая (Mutable)\n2. Неизменяемая (Immutable)\nВыбор: ";
    int mutChoice = ReadInt();
    while (mutChoice != 1 && mutChoice != 2) {
        std::cout << "Нужно 1 или 2. Повторите ввод: ";
        mutChoice = ReadInt();
    }
    isMutable = (mutChoice == 1);

    std::cout << "Количество начальных элементов: ";
    int count = ReadInt();
    while (count < 0) {
        std::cout << "Количество не может быть отрицательным. Повторите ввод: ";
        count = ReadInt();
    }

    DynamicArray<int> buffer(count > 0 ? count : 0); // сначала читаем в массив, для списка потом переливаем
    for (int i = 0; i < count; ++i) {
        std::cout << "Элемент " << i << ": ";
        int val = ReadInt();
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

        int option = ReadInt();
        if (option == 0) break; // до switch, тк break в switch не выходит из while

        try {
            switch (option) { // у каждого case свои {}: иначе прыжок на следующий case обходил бы инициализацию переменных и не компилировалось
            case 1: {
                std::cout << "Значение: ";
                int v = ReadInt();
                Sequence<int>* next = currentSeq->Append(v);
                if (!isMutable) delete currentSeq; // иммутабл вернул новый объект, старый удаляем, иначе утечка
                currentSeq = next;
                OutputSequence(currentSeq);
                break;
            }
            case 2: {
                std::cout << "Значение: ";
                int v = ReadInt();
                Sequence<int>* next = currentSeq->Prepend(v);
                if (!isMutable) delete currentSeq;
                currentSeq = next;
                OutputSequence(currentSeq);
                break;
            }
            case 3: {
                std::cout << "Значение: ";
                int v = ReadInt();
                std::cout << "Индекс: ";
                int idx = ReadInt();
                Sequence<int>* next = currentSeq->InsertAt(v, idx);
                if (!isMutable) delete currentSeq;
                currentSeq = next;
                OutputSequence(currentSeq);
                break;
            }
            case 4: {
                std::cout << "Индекс: ";
                int idx = ReadInt();
                std::cout << "Элемент [" << idx << "] = " << currentSeq->Get(idx) << "\n";
                break;
            }
            case 5: {
                std::cout << "Первый: " << currentSeq->GetFirst() 
                          << ", Последний: " << currentSeq->GetLast() << "\n";
                break;
            }
            case 6: {
                std::cout << "Индекс начала: ";
                int s = ReadInt();
                std::cout << "Индекс конца: ";
                int e = ReadInt();
                Sequence<int>* sub = currentSeq->GetSubSequence(s, e);
                std::cout << "Срез: ";
                OutputSequence(sub);
                delete sub;
                break;
            }
            case 7: {
                std::cout << "Сколько элементов во второй последовательности: ";
                int k = ReadInt();
                DynamicArray<int> tempArr(k);
                for (int i = 0; i < k; ++i) {
                    std::cout << "Элемент " << i << ": ";
                    int v = ReadInt();
                    tempArr.Set(i, v);
                }
                MutableArraySequence<int> second(tempArr); // на стеке: Concat копирует эл, удалять не надо
                Sequence<int>* joined = currentSeq->Concat(&second);
                if (!isMutable) delete currentSeq;
                currentSeq = joined;
                OutputSequence(currentSeq);
                break;
            }
            case 8: {
                Sequence<int>* filtered = currentSeq->Where(PredicateEven);
                std::cout << "Четные числа: ";
                OutputSequence(filtered);
                delete filtered;
                break;
            }
            case 9: {
                Sequence<int>* mapped = currentSeq->Map(TransformSquare);
                std::cout << "Квадраты: ";
                OutputSequence(mapped);
                delete mapped;
                break;
            }
            case 10: {
                int total = currentSeq->Reduce<int>(AccumulateSum, 0);
                std::cout << "Сумма всех элементов: " << total << "\n";
                break;
            }
            case 11: {
                MinMaxAvg<int> stats = ComputeMinMaxAvg(currentSeq);
                std::cout << "Минимум:  " << stats.min << "\n"
                          << "Максимум: " << stats.max << "\n"
                          << "Среднее:  " << stats.avg << "\n";
                break;
            }
            case 12: {
                OutputSequence(currentSeq);
                break;
            }
            default: {
                std::cout << "Неизвестный пункт.\n";
            }
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
        int len = ReadInt();

        DynamicArray<Bit> raw(len);
        for (int i = 0; i < len; ++i) {
            std::cout << "Бит " << i << " (0/1): ";
            int b = ReadInt();
            raw.Set(i, Bit(b != 0));
        }

        BitSequence mask1(raw);
        OutputBitMask(&mask1);

        BitSequence* inverted = mask1.Not();
        std::cout << "NOT (инверсия): ";
        OutputBitMask(inverted);
        delete inverted;

        // вторая маска 1010..., чтоб было с чем делать AND
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
    int iterations = ReadInt();

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

        int mainChoice = ReadInt();

        if (mainChoice == 0) break; // до switch, тк break в switch не выходит из while

        switch (mainChoice) {
        case 1: ExecuteSequenceMenu(); break;
        case 2: ExecuteBitMenu(); break;
        case 3: ExecuteBenchmark(); break;
        default: std::cout << "Некорректная команда.\n";
        }
    }

    std::cout << "Программа завершена.\n";
    return 0;
}