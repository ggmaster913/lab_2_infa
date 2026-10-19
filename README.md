# Лабораторная работа №2 — динамический массив, связный список, последовательность

Полиморфные АТД `DynamicArray<T>`, `LinkedList<T>` и `Sequence<T>` с реализациями
на массиве и на списке, в изменяемом и неизменяемом вариантах, плюс битовая
последовательность `BitSequence`.

## Что реализовано

Обязательная часть:

| Что это | Файл |
|---|---|
| Динамический массив `DynamicArray<T>` | `include/DynamicArray.h` |
| Связный список `LinkedList<T>` | `include/LinkedList.h` |
| Последовательность `Sequence<T>` (абстрактный класс) | `include/Sequence.h` |
| `ArraySequence<T>` и `ListSequence<T>` | `include/ArraySequence.h`, `include/ListSequence.h` |
| Изменяемые и неизменяемые варианты | `include/Mutable*Sequence.h`, `include/Immutable*Sequence.h` |
| `Map`, `Where`, `Reduce` | `include/Sequence.h`, `include/ArraySequence.h`, `include/ListSequence.h`, `include/MutableArraySequence.h` |
| `BitSequence` (интерфейс `Sequence<Bit>`) с AND, OR, XOR, NOT | `include/Bit.h`, `include/BitSequence.h` |
| Замер производительности `Append` на массиве и на списке | `ui/ConsoleUI.cpp` |

Пункты варианта (таблица 2 методички, нужно не менее 9 баллов):

| Пункт | Что это | Баллы | Файл |
|---|---|---|---|
| Б-3.1.1 | `MutableListSequence` и `ImmutableListSequence` | 2 | `include/MutableListSequence.h`, `include/ImmutableListSequence.h` |
| Б-3.2.1 | `MutableArraySequence` и `ImmutableArraySequence` | 2 | `include/MutableArraySequence.h`, `include/ImmutableArraySequence.h` |
| М-1 | Интерфейс `ICollection<T>` (`Get`, `GetCount`, копирующий конструктор) | 2 | `include/ICollection.h`, `include/DynamicArray.h` |
| М-4 | Перегрузка операторов: `[]` у массива, списка и последовательностей, `&`, `\|`, `^`, `~`, `==` у `Bit` | 2 | `include/*.h` |
| П-1 | `(min, max, avg)` за один проход через `Reduce` | 1 | `include/SequenceStats.h` |
| | **Итого** | **9** | |

Неизменяемые последовательности при изменении возвращают новый объект, исходный
не меняется. Некорректные аргументы бросают `std::out_of_range` или
`std::invalid_argument`; у неизменяемых проверка делается до создания копии,
поэтому при ошибке утечки нет.

## Структура проекта

```
include/
  DynamicArray.h, LinkedList.h, ICollection.h
  Sequence.h, ArraySequence.h, ListSequence.h
  MutableArraySequence.h, ImmutableArraySequence.h
  MutableListSequence.h, ImmutableListSequence.h
  Bit.h, BitSequence.h
  SequenceStats.h
tests/
  Tests.cpp
ui/
  ConsoleUI.cpp
```

## Сборка

Нужны CMake 3.14 или новее и компилятор с поддержкой C++17.
GoogleTest скачивается автоматически при первой настройке.

```bash
cmake -S . -B build
cmake --build build
```

## Тесты

```bash
./build/tests
```

26 тестов: 5 на `DynamicArray`, 7 на `LinkedList`, 8 на последовательности, 5 на `BitSequence`, 1 на `(min, max, avg)`.

## Консольное меню

```bash
./build/ui
```

1. Последовательность — выбор массива или списка, изменяемой или неизменяемой, ввод элементов и все операции над ней: `Append`, `Prepend`, `InsertAt`, `Get`, `GetFirst`/`GetLast`, `GetSubSequence`, `Concat`, `Where`, `Map`, `Reduce`, `(min, max, avg)`.
2. `BitSequence` — ввод маски, NOT и AND с маской 1010….
3. Тест производительности — время `Append` для массива и списка.

Неверный ввод (буквы, отрицательное количество) не ломает меню: программа просит ввести заново.
