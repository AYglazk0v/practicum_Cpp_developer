<details>
<summary>Теория:</summary>

# Выводим результаты поиска страницами

Итераторы можно применять далеко не в самых очевидных случаях. Представьте, что ваш поисковый сервер содержит сотни тысяч или даже миллионы документов. Из них тысячи подходят под запрос пользователя. В этом случае вывод всех запросов на экран разом не будет хорошей идеей. Нужно разбивать результаты на страницы.

Пригодятся итераторы. Предположим, у нас есть контейнер. Тогда одна страница — это некий диапазон определённого размера из этого контейнера. Первый элемент входит в страницу, а последний — нет. Зато последний будет первым элементом на следующей странице. То есть можем получить контейнер с результатами, а потом на основе него создать вектор диапазонов, где диапазон будет просто парой итераторов. Первый итератор укажет на начало страницы, а второй — на её конец.

Отвечать за разделение по страницам может новый класс. Назовём его  `Paginator`. Пусть это будет класс-шаблон. Он работает так же, как метод-шаблон, только при создании объекта такого класса надо указать тип. С подобными классами вы уже встречались. Например, все классы контейнеров — шаблоны. Синтаксис будет выглядеть так:

```cpp
template <typename Iterator>
class Paginator {
		// тело класса
};
```

</details>

<details>
<summary>Задание:</summary>

## Задание

Научите свою поисковую систему разбивать результаты на страницы. Вам понадобится конструктор объекта и методы  `begin`,  `end`  и  `size`. Чтобы работать с классом  `Paginator`  было удобнее, создайте функцию  `Paginate`, возвращающую объект класса  `Paginator`.

Сохраните решение себе локально или в  `git`. Оно понадобится вам для финального проекта спринта.

### Пример использования

```cpp
template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

int main() {
    SearchServer search_server("and with"s);

    search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, {1, 1, 1});

    const auto search_results = search_server.FindTopDocuments("curly dog"s);
    int page_size = 2;
    const auto pages = Paginate(search_results, page_size);

    // Выводим найденные документы по страницам
    for (auto page = pages.begin(); page != pages.end(); ++page) {
        cout << *page << endl;
        cout << "Page break"s << endl;
    }
}

```

### Пример вывода

```
{ document_id = 2, relevance = 0.402359, rating = 2 }{ document_id = 4, relevance = 0.229073, rating = 2 }
Page break
{ document_id = 5, relevance = 0.229073, rating = 1 }
Page break

```

### Подсказка

Страница — это два итератора, указывающие на начало и на конец. Было бы удобно иметь небольшой класс, который позволит работать с парами итераторов. Можно называть его  `IteratorRange`  и сделать ему методы  `begin`,  `end`  и  `size`. В таком случае внутри объекта  `Paginator`  вы просто спрячете вектор таких вот  `IteratorRange`  и будете заполнять его в конструкторе объекта  `Paginator`.

Вам понадобятся операторы вывода для типа  `Document`  и для типа  `IteratorRange`.

Не забудьте, что  `Paginator`  должен уметь работать с разными типами контейнеров. Не ограничивайте его итераторами произвольного доступа. Вам могут пригодиться  `distance`  и  `advance`.

</details>
