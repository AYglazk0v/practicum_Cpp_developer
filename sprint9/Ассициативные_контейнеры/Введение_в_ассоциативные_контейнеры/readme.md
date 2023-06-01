<details>
<summary>Теория +  тестирование:</summary>

# Введение в ассоциативные контейнеры

Когда вам нужна статья в энциклопедии, вы ищите слово — название статьи. Слово — это  **ключ**, а статья — полезное  **значение**, которое ассоциируется с ключом и к которому ключ даёт доступ. Это пример  **ассоциативного контейнера**.

В C++ вам уже знаком ассоциативный контейнер  `std::map`. Ранее вы изучали работу контейнеров  `vector`,  `list`  и  `deque`, но  `map`  устроен хитрее. В этой теме речь пойдёт о том, как устроен  `map`  и ассоциативные контейнеры, с которыми вы ещё не встречались.

Ассоциативные контейнеры умеют выполнять важную задачу — эффективно искать нужный элемент по ключу. Часто возникает ситуация, когда ключ — это само значение. Так происходит в  `set`, который похож на  `map`. Казалось бы, никакой ассоциации нет — ничто ни с чем не ассоциируется. Но  `set`  всё равно относят к ассоциативным контейнерам, потому что он умеет быстро находить нужный элемент. Чтобы задать особые критерии сравнения элементов множества и искать в  `set`  по этим критериям, используйте собственный компаратор. В следующем примере множество кошек учитывает не все поля структуры  `Cat`, а только породу. Получается, что с каждой породой кошек ассоциирована некоторая кошка этой породы:

```cpp
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <set>

using namespace std;

struct Cat {
    string breed;
    string name;
    int age;
};

ostream& operator<<(ostream& os, const Cat& cat) {
    return os <<'{' << cat.breed << ", "s << cat.name << ", "s << cat.age << '}';
}

// Компаратор, сравнивающий породы кошек
struct CatBreedComparator {
    // Помечаем компаратор как «прозрачный», чтобы с его помощью можно было сравнивать
    // не только кошек с кошками, но и со строками, задающими породу кошек
    using is_transparent = std::true_type;
    
    bool operator()(const Cat& lhs, const Cat& rhs) const {
        return lhs.breed < rhs.breed;
    }
    bool operator()(const Cat& lhs, const string& rhs_breed) const {
        return lhs.breed < rhs_breed; 
    }
    bool operator()(const string& lhs_breed, const Cat& rhs) const {
        return lhs_breed < rhs.breed;
    }
};

int main() {
    // Множество кошек. Кошки считаются эквивалентными, если их породы совпадают.
    // В такой контейнер не получится поместить двух кошек одинаковой породы.
    set<Cat, CatBreedComparator> cats;
    
    cats.insert({"siamese"s, "Tom"s, 3});
    cats.insert({"bengal"s, "Leo"s, 2});
    cats.insert({"birman"s, "Tiger"s, 5});
    // Ginger вставлен не будет, так как множество уже содержит кошку сиамской породы
    cats.insert({"siamese"s, "Ginger"s, 2});
    
    // Выводим кошек в стандартный вывод, по одной на каждой строке
    copy(cats.begin(), cats.end(), ostream_iterator<Cat>(cout, "\n"));
    
    // Кошка, эквивалентная Мурке бенгальской породы, во множестве cats есть - это Leo
    assert(cats.count({"bengal"s, "Murka"s, 6}) == 1);
    
    // Компаратор CatBreedComparator позволяет искать кошек, указывая их породу
    if (auto it = cats.find("siamese"s); it != cats.end()) {
        cout << "Found cat: "s << *it << endl; // Выведет {siamese, Tom, 3}
    }
}

```

Контейнер  `vector`  относят к линейным контейнерам из-за способа хранить данные: он кладёт элементы в память последовательно, как бы выстраивая их в одну линию. Но даже вектор имеет некоторые признаки ассоциативного контейнера. Вспомните камеру хранения на железнодорожном вокзале. В ней вещи хранятся в ячейках. Чтобы получить доступ к содержимому, нужно иметь ключ — номер ячейки. Аналог такой камеры хранения в C++ —  `vector`. Он хранит значения, а ключи — это индексы элементов. Как и ассоциативный контейнер, вектор обеспечивает быстрый доступ к элементу по ключу.

Используем  `map`  для поиска слов, наиболее часто встречаемых в тексте:

```cpp
// файл wordstat.cpp

#include <iostream>
#include <map>
#include <string>
#include <tuple>

using namespace std;

int main() {
    string word;
    map<string, int> counts_map;

    while (cin >> word) {
        ++counts_map[move(word)];
    }

    cout << "Слово - Количество упоминаний в тексте"s << endl;
    // выводим первые 10 слов
    for (auto [iter, i] = tuple(counts_map.begin(), 0); i < 10 && iter != counts_map.end(); ++i, ++iter) {
        cout << iter->first << " - "s << iter->second << endl;
    }
}

```

Для примера возьмём книгу Джефри Чосера «Кентерберийские рассказы»‎. Мы сохранили её в файл canterbury.txt. При запуске перенаправим содержимое этого файла в  `cin`  программы, используя в командной строке символ  `<`:

```
> g++ wordstat.cpp -o wordstat -std=c++17 -O3
> ./wordstat < canterbury.txt
Слово - Количество упоминаний в тексте
a - 3025
aaron - 1
abak - 2
abasshed - 1
abate - 1
abated - 1
abateth - 1
abaundone - 2
abaundoneth - 2
abaysed - 1

```

Видно, сколько раз встречался артикль «a»‎ и некоторые другие слова. Но решить мы хотели другую задачу — слова отсортированы по алфавиту, а не по частоте. Чтобы исправить это, переместим слова в вектор и применим алгоритм  `sort`. В качестве компаратора для  `sort`  используем лямбда-функцию, сравнивающую частоты слов:

```cpp
// файл wordstat.cpp

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

vector<pair<string, int>> GetSortedWordCounts(vector<string> words) {
    map<string, int> counts_map;

    for (auto& word : words) {
        ++counts_map[move(word)];
    }

    vector<pair<string, int>> counts_vector(move_iterator(counts_map.begin()), move_iterator(counts_map.end()));
    sort(counts_vector.begin(), counts_vector.end(), [](const auto& l, const auto& r) {
        return l.second > r.second;
    });

    return counts_vector;
}

int main() {
    vector<string> words;
    string word;

    while (cin >> word) {
        words.push_back(word);
    }

    auto counts_vector = GetSortedWordCounts(move(words));

    cout << "Слово - Количество упоминаний в тексте"s << endl;
    // выводим первые 10 слов
    for (auto [iter, i] = tuple(counts_vector.begin(), 0); i < 10 && iter != counts_vector.end(); ++i, ++iter) {
        cout << iter->first << " - "s << iter->second << endl;
    }
}

```

Проверим вывод теперь:

```
Слово - Количество упоминаний в тексте
and - 8877
that - 5832
the - 5623
of - 5351
to - 4002
he - 3718
in - 3544
his - 3103
for - 3027
a - 3025

```

Вывод правдоподобный: наиболее встречающиеся слова, как и следовало ожидать, — предлоги, союзы, артикли, местоимения.

----------

Пусть в тексте содержится N слов. Вспомните тему об алгоритмической сложности и попробуйте оценить предложенный выше алгоритм.

-   O(1)
    
-   O(N)
    
-   O(N\log N)
    
-   O(N^2)
    
-   O(2^N)
    

Алгоритм состоит из двух частей:

1.  Наполнение  `map`.
2.  Перенос в вектор и его сортировка.

Обе части имеют одинаковую сложность — O(N\log N). Улучшить сложность этой программы как будто невозможно, ведь более быстрого алгоритма сортировки не существует. Тем не менее, в следующем уроке вы попытаетесь ускорить этот код.

----------

Теперь проанализируйте текст с помощью программы самостоятельно. Поскольку здесь исследуется текст английского поэта, вопрос тоже сформулирован по-английски: Which word is the most frequent 5-letter word in Chaucer’s book?.

Ответ: The word ”which” is the most frequent 5-letter word in Chaucer’s book..

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Пусть в тексте содержится N слов. Вспомните тему об алгоритмической сложности и попробуйте оценить предложенный выше алгоритм.

-   **(-)**  O(1)
    
-   **(-)**  O(N)
    
-   **(+)**  O(N\log N)
    
-   **(-)**  O(N^2)
    
-   **(-)**  O(2^N)

</details>
