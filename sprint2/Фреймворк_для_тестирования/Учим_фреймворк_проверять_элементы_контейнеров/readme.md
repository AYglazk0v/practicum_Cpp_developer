<details>  
<summary>Теория:</summary>

# Учим фреймворк проверять элементы контейнеров

В предыдущем уроке вы познакомились с возможностями макросов и применили их, чтобы улучшить фреймворк. Теперь он выводит подробную диагностическую информацию. В этом уроке научим ваш фреймворк проверять содержимое элементов контейнера.

Пока он этого делать не умеет. Рассмотрим тестирований функций  `TakePositives`  и  `TakeEvens`, которые возвращают  `vector`:

-   `TakeEvens`  — возвращает чётные элементы массива;
-   `TakePositives`  — возвращает положительные элементы переданного ей массива.

```cpp
vector<int> TakeEvens(const vector<int>& numbers) {
    vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}

vector<int> TakePositives(const vector<int>& numbers) {
    vector<int> positives;
    for (int x : numbers) {
        // Ошибка допущена намеренно, чтобы продемонстрировать вывод при несработавшем AssertEqual
        if (x >= 0) {
            positives.push_back(x);
        }
    }
    return positives;
}

int main() {
    const vector<int> numbers = {1, 0, 2, -3, 6, 2, 4, 3};
    const vector<int> expected_evens = {0, 2, 6, 2, 4};
    ASSERT_EQUAL(TakeEvens(numbers), expected_evens);

    const vector<int> expected_positives = {1, 2, 6, 2, 4, 3};
    ASSERT_EQUAL(TakePositives(numbers), expected_positives);
}

```

Попытаемся скомпилировать код и увидим ошибку. Компилятор не знает, как вывести вектор целых чисел, передаваемый в функцию  `AssertEqualImpl`:

```cpp
template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        ...
        **cout << t << " != "s << u << "."s;**
        ...
    }
}

```

Чтобы устранить ошибку компиляции, научите компилятор выводить в поток элементы контейнеров  `vector`,  `set`  и  `map`  с содержимым произвольного типа.
</details>  

<details>  
<summary>Задание:</summary>

## Задание

Добавьте во фреймворк возможность сравнивать содержимое контейнеров  `vector`,  `set`  и  `map`, реализовав для них шаблонные операторы вывода в поток.

Формат вывода элементов  `vector`:

```
[элемент1, элемент2, элемент3, ... элементN]

```

Формат вывода элементов  `set`:

```
{элемент1, элемент2, элемент3, ... элементN}

```

Формат вывода элементов  `map`:

```
{ключ1: значение1, ключ2: значение2, ... ключN: значениеN}

```

После последнего элемента данных коллекций запятая выводиться не должна.

### Подсказка

Вы уже реализовывали шаблонные операторы вывода в поток. Вспомните, как вы это делали.

</details>  
