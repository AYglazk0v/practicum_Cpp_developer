<details>  
<summary>Теория:</summary>



<h1>Функциональные объекты</h1>

<p>Вы уже сталкивались с функциями, в которые можно передавать другие функции. Так, в функцию <code>sort</code> передаётся компаратор: либо в виде названия функции, либо в виде лямбда-функции, написанной при вызове сортировки. А функции <code>count_if</code> и <code>all_of</code> принимают проверяющую функцию.</p>

<p>В этом уроке вы научитесь писать такие функции. И вам снова будут полезны шаблоны.</p>

<p>Зададимся целью написать удобную обёртку для функции <code>sort</code>. Пусть она принимает контейнер и указание, по какой характеристике сортировать объекты. Указание должно быть в виде функции:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;vector&gt;

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

void PrintNames(const vector&lt;Animal&gt;&amp; animals) {
    for (const Animal&amp; animal : animals) {
        cout &lt;&lt; animal.name &lt;&lt; ' ';
    }
    cout &lt;&lt; endl;
}

// как написать SortBy?

int main() {
    vector&lt;Animal&gt; animals = {
        {&quot;Мурка&quot;s,   10, 5},
        {&quot;Белка&quot;s,   5,  1.5},
        {&quot;Георгий&quot;s, 2,  4.5},
        {&quot;Рюрик&quot;s,   12, 3.1},
    };

    PrintNames(animals);
		// без сортировки: Мурка Белка Георгий Рюрик

    SortBy(animals, [](const Animal&amp; animal) { return animal.name; });
    PrintNames(animals);
		// сортировка по имени: Белка Георгий Мурка Рюрик

    SortBy(animals, [](const Animal&amp; animal) { return -animal.weight; });
    PrintNames(animals);
		// сортировка по убыванию веса: Мурка Георгий Рюрик Белка
}
</code></pre>

<p>Написать функцию <code>SortBy</code>, принимающую только контейнер, проще простого:</p>

<pre><code class="language-cpp">template &lt;typename Container&gt; 
void SortBy(Container&amp; container) {
    sort(container.begin(), container.end());
}
</code></pre>

<p>Здесь контейнером может быть как вектор с любыми элементами, так и строка.</p>

<p>Чтобы понять, как принять функцию вторым параметром, разберёмся, чего мы от функции ожидаем и какими они бывают.</p>

<p>Допустим, <code>key_mapper</code> — функция, которая по объекту возвращает ключ для сравнения объекта с соседями, а <code>x</code> — такой объект. Тогда естественно ожидать, что <code>key_mapper(x)</code> и есть ключ для данного объекта.</p>

<p>Ожидания оправдываются, когда <code>key_mapper</code> — обычная функция:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;vector&gt;

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

const string&amp; GetName(const Animal&amp; animal) {
    return animal.name;
}
   
int main() {
    vector&lt;Animal&gt; animals = {
        {&quot;Мурка&quot;s,   10, 5},
        {&quot;Белка&quot;s,   5,  1.5},
        {&quot;Георгий&quot;s, 2,  4.5},
        {&quot;Рюрик&quot;s,   12, 3.1},
    };

    auto key_mapper = GetName;
    cout &lt;&lt; key_mapper(animals.front()) &lt;&lt; endl;
		// выведет имя первого животного: Мурка
}
</code></pre>

<p>Но можно положить в <code>key_mapper</code> лямбда-функцию. И всё тоже сработает ожидаемо:</p>

<pre><code class="language-cpp">auto key_mapper = [](const Animal&amp; animal) { return animal.name; };
cout &lt;&lt; key_mapper(animals.front()) &lt;&lt; endl;
// и снова имя первого животного: Мурка
</code></pre>

<p>В обоих примерах мы написали <code>auto</code>. В случае, когда <code>key_mapper</code> — обычная функция, мы не указали тип функции явно, потому что он слишком длинный. В случае лямбды у нас уникальный тип без имени, поэтому без <code>auto</code> не обойтись.</p>

<p>Итак, чтобы принять в <code>SortBy</code> функцию, достаточно указать, что вторым параметром <code>SortBy</code> должен быть объект <code>key_mapper</code> произвольного, то есть шаблонного типа. Такой объект, что выражение <code>key_mapper(x)</code> для элемента <code>x</code> возвращает ключ для сортировки.</p>

<p>Сделаем же тип параметра <code>key_mapper</code> вторым шаблонным параметром функции!</p>

<pre><code class="language-cpp">template &lt;typename Container, typename KeyMapper&gt;
void SortBy(Container&amp; container, KeyMapper key_mapper) {
    // теперь можно сортировать контейнер
    // с компаратором key_mapper(lhs) &lt; key_mapper(rhs)
}
</code></pre>

<p><code>key_mapper</code> принимается в функции по значению. Так принято, потому что функции, как правило, — лёгкие объекты.</p>

<p>Любой объект, который можно использовать как функцию, называется функциональным объектом. Позже вы научитесь создавать такие объекты самостоятельно.</p>

</details>  

<details>  
<summary>Задание 1:</summary>
<p>Реализуйте функцию <code>SortBy</code> в соответствии с указанными требованиями. В процессе у вас могут возникнуть проблемы. Постарайтесь решить их самостоятельно: читайте подсказки.</p>

<p>Код должен выводить:</p>

<pre><code>Мурка Белка Георгий Рюрик 
Белка Георгий Мурка Рюрик 
Мурка Георгий Рюрик Белка
</code></pre>

<h3>Подсказка</h3>

<ol>
<li>Чтобы написать компаратор, укажите типы параметров <code>lhs</code> и <code>rhs</code>. С ходу это сделать не получается, потому что известен только тип контейнера. Здесь поможет <code>auto</code>. Задайте список параметров <code>const auto&amp; lhs, const auto&amp; rhs</code>, и тогда сам компаратор окажется по сути шаблонной функцией.</li>
<li>Чтобы вызвать в компараторе-лямбде <code>key_mapper</code>, захватите его: <code>[key_mapper](const auto&amp; lhs, const auto&amp; rhs)</code>.</li>
</ol>

</details>  

<details>  
<summary>Задание 2:</summary>

<p>Мы ловко упорядочили животных по убыванию веса, отсортировав по возрастанию <code>-weight</code>. Но с именем так не выйдет. Добавьте в функцию необязательный третий параметр <code>reverse</code>. Если туда будет передано <code>true</code>, объекты нужно отсортировать в обратном порядке.</p>

<p>Не используйте функцию <code>reverse</code> из библиотеки <code>&lt;algorithm&gt;</code>.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">int main() {
    vector&lt;Animal&gt; animals = {
        {&quot;Мурка&quot;s,   10, 5},
        {&quot;Белка&quot;s,   5,  1.5},
        {&quot;Георгий&quot;s, 2,  4.5},
        {&quot;Рюрик&quot;s,   12, 3.1},
    };

    PrintNames(animals);

    SortBy(animals, [](const Animal&amp; animal) { return animal.name; }, true);
    PrintNames(animals);

    SortBy(animals, [](const Animal&amp; animal) { return animal.weight; });
    PrintNames(animals);

    return 0;
}
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>Мурка Белка Георгий Рюрик 
Рюрик Мурка Георгий Белка 
Белка Рюрик Георгий Мурка
</code></pre>

<h3>Подсказка</h3>

<p>Здесь будет достаточно любого решения, даже с небольшим копированием кода. Например, можно начать функцию с <code>if (reverse)</code> и в каждой ветке отсортировать с нужным компаратором.</p>

</details>  

<details>  
<summary>Задание 3:</summary>
<p>Вернёмся к поисковой системе. Научите метод <code>FindTopDocuments</code> вместо статуса принимать более универсальный фильтр документов — функцию-предикат. Она в свою очередь принимает id документа, статус и рейтинг и возвращает <code>bool</code>. Фильтрация документов должна производиться до отсечения топа из пяти штук.</p>

<p>Вызов <code>FindTopDocuments</code> без второго аргумента должен осуществлять поиск только по актуальным документам.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">void PrintDocument(const Document&amp; document) {
    cout &lt;&lt; &quot;{ &quot;s
         &lt;&lt; &quot;document_id = &quot;s &lt;&lt; document.id &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;relevance = &quot;s &lt;&lt; document.relevance &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;rating = &quot;s &lt;&lt; document.rating
         &lt;&lt; &quot; }&quot;s &lt;&lt; endl;
}

int main() {
    SearchServer search_server;
    search_server.SetStopWords(&quot;и в на&quot;s);

    search_server.AddDocument(0, &quot;белый кот и модный ошейник&quot;s,        DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, &quot;пушистый кот пушистый хвост&quot;s,       DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, &quot;ухоженный пёс выразительные глаза&quot;s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    search_server.AddDocument(3, &quot;ухоженный скворец евгений&quot;s,         DocumentStatus::BANNED, {9});

    cout &lt;&lt; &quot;ACTUAL by default:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s)) {
        PrintDocument(document);
    }

    cout &lt;&lt; &quot;ACTUAL:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s, [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL; })) {
        PrintDocument(document);
    }

    cout &lt;&lt; &quot;Even ids:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s, [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; })) {
        PrintDocument(document);
    }

    return 0;
}
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>ACTUAL by default:
{ document_id = 1, relevance = 0.866434, rating = 5 }
{ document_id = 0, relevance = 0.173287, rating = 2 }
{ document_id = 2, relevance = 0.173287, rating = -1 }
ACTUAL:
{ document_id = 1, relevance = 0.866434, rating = 5 }
{ document_id = 0, relevance = 0.173287, rating = 2 }
{ document_id = 2, relevance = 0.173287, rating = -1 }
Even ids:
{ document_id = 0, relevance = 0.173287, rating = 2 }
{ document_id = 2, relevance = 0.173287, rating = -1 }
</code></pre>

<h3>Подсказка</h3>

<p>Чтобы можно было вызвать <code>FindTopDocuments</code> с одним аргументом вместо двух, напишите одноимённый метод с одним параметром. В нём вызовите <code>FindTopDocuments</code> с двумя аргументами, второй из которых — предикат нужного вида.</p>

</details>  
