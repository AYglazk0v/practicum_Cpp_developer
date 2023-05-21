<details>
<summary>Теория:</summary>

# Move-итераторы

Последним штрихом к вашему арсеналу способов перемещения объектов будут move-итераторы.

Посмотрите на пример, написанный любителем творчества Пелевина:

```cpp
void Print(const string& book_list_name, const vector<string>& book_list) {
    cout << book_list_name << endl;
    for (const string& s : book_list) {
        cout << s << endl;
    }  
}

int main() {
    vector<string> my_books = {"Chapaev i pustota"s, "Nepobedimoe solnce"s, "Generation P"s, "Jizn' nasekomyh"s,
                               "Taynie vidy na goru Fudzi"s};
    vector<string> given_to_read(my_books.begin(), my_books.begin() + 2);
    vector<string> to_present(my_books.begin() + 2, my_books.end());

    Print("my_books", my_books);
    cout << endl;
    Print("given_to_read", given_to_read);
    cout << endl;
    Print("to_present", to_present);
}

```

Вектор  `my_books`  содержит названия книг, которыми этот любитель владеет. Вектор  `given_to_read`  — книги, которые он дал почитать и ожидает к возврату. Вектор  `to_present`  — книги, которые владелец хотел бы подарить друзьям, чтобы приобщить их к прекрасному.

Для создания векторов  `given_to_read`  и  `to_present`  использованы конструкторы по итераторам. Названия книг просто скопированы из вектора-полки в два других. На экран будет выведено:

```
my_books
Chapaev i pustota
Nepobedimoe solnce
Generation P
Jizn' nasekomyh
Taynie vidy na goru Fudzi

given_to_read
Chapaev i pustota
Nepobedimoe solnce

to_present
Generation P
Jizn' nasekomyh
Taynie vidy na goru Fudzi

```

Начальный вектор названий остался без изменений. В целом, логика верная. Книги, которые отданы почитать, всё еще принадлежат автору, как и те, которые он только собирается подарить. Поэтому удалять что-либо из вектора  `my_books`  не нужно.

Прошло время, и книги, приготовленные в подарок, переданы новым владельцам. Просто переименуем переменную  `to_present`  в  `presented`:

```cpp
 vector<string> presented(book_shelf.begin() + 2, book_shelf.end());

```

Но теперь, следуя логике, следует не копировать названия, а просто перенести их. Не хотелось бы кардинально переписывать программу, писать цикл, использовать  `push_back`  и  `move`. Можно просто изменить семантику итераторов — превратить их из обычных в итераторы перемещения. Для этого нужна библиотека  `iterator`  и функция  `make_move_iterator`:

```cpp
int main() {
    vector<string> my_books = {"Chapaev i pustota"s, "Nepobedimoe solnce"s, "Generation P"s, "Jizn' nasekomyh"s,
                               "Taynie vidy na goru Fudzi"s};
    vector<string> given_to_read(my_books.begin(), my_books.begin() + 2);
    vector<string> presented(make_move_iterator(my_books.begin() + 2), make_move_iterator(my_books.end()));

    Print("my_books", my_books);
    cout << endl;
    Print("given_to_read", given_to_read);
    cout << endl;
    Print("presented", presented);
}
```

</details>

<details>
<summary>Задание 1:</summary>

Вернитесь к решению задачи о считалке Иосифа и перепишите его с использованием итераторов перемещения. В тестах будет проверяться наличие  `make_move_iterator`  в коде функции.

### Подсказка

Вместо цикла для создания  `pool`  можно использовать конструктор с аргументами-итераторами.

</details>

<details>
<summary>Задание 2:</summary>

В прошлом уроке вы разделили вектор лексем на предложения. Теперь перепишите своё решение, используя  `make_move_iterator`. В тестах будет проверяться наличие соответствующей функции в коде.

### Подсказка

Используйте метод  `push_back`  для итогового вектора с итераторами начала и конца диапазона в качестве аргументов.

</details>
