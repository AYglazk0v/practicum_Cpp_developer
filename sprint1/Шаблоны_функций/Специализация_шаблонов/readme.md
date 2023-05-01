<details>  
<summary>Теория:</summary>
<h1>Специализация шаблонов</h1>

<p>Любой объект, который мы передадим вторым аргументом в <code>SortBy</code>, компилятор попытается использовать как функцию. И выдаст ошибку, если не сможет. Рассмотрим, как упростить вызов функции <code>SortBy</code> для частных случаев и не писать громоздкие лямбды.</p>

<p>В качестве примера объявим <code>enum class AnimalSortKey</code> с типами ключа для сортировки животных:</p>

<pre><code class="language-cpp">enum class AnimalSortKey {
    AGE,     // по полю age
    WEIGHT,  // по полю weight
    RELATIVE_WEIGHT  // по weight / age
};
</code></pre>

<p>И попробуем вызвать функцию <code>SortBy</code> с элементом этого перечисления в качестве ключа:</p>

<pre><code class="language-cpp">int main() {
    vector&lt;Animal&gt; animals = {
        {&quot;Мурка&quot;s,   10, 5},
        {&quot;Белка&quot;s,   5,  1.5},
        {&quot;Георгий&quot;s, 2,  4.5},
        {&quot;Рюрик&quot;s,   12, 3.1},
    };

	PrintNames(animals);
	// Мурка Белка Георгий Рюрик

    SortBy(animals, [](const Animal&amp; animal) { return animal.name; }, true);
    PrintNames(animals);
	// Рюрик Мурка Георгий Белка

    SortBy(animals, AnimalSortKey::RELATIVE_WEIGHT);
    PrintNames(animals);
	// ожидаем вывод: Рюрик Белка Мурка Георгий
}
</code></pre>

<p>Если ничего не менять, увидим большую ошибку компиляции. Вот пара фрагментов:</p>

<pre><code>test.cpp: In instantiation of ‘SortBy(Container&amp;, KeyMapper, bool) [with Container = std::vector&lt;Animal&gt;; KeyMapper = AnimalSortKey]::&lt;lambda(const auto:1&amp;, const auto:2&amp;)&gt; [with auto:1 = Animal; auto:2 = Animal]’:
...
test.cpp:18:34: error: ‘key_mapper’ cannot be used as a function
   18 |                 return key_mapper(lhs) &gt; key_mapper(rhs);
</code></pre>

<p>Компилятор не смог использовать <code>key_mapper</code> как функцию. Неудивительно, это же <code>enum</code>. В первой строке уточняется, где произошла неудача: в шаблонной лямбда-функции, где оба <code>auto</code> — это <code>Animal</code>. То есть <code>&lt;lambda(const auto:1&amp;, const auto:2&amp;)&gt; [with auto:1 = Animal; auto:2 = Animal]</code>.</p>

<p>При этом лямбда создана в <code>SortBy(Container&amp;, KeyMapper, bool)</code> с <code>Container = std::vector&lt;Animal&gt;</code> и <code>KeyMapper = AnimalSortKey</code>.</p>

<p>Итак, мы хотим написать конкретную версию функции <code>SortBy</code> с аргументами <code>vector&lt;Animal&gt;&amp; animals, AnimalSortKey sort_key, bool reverse = false</code>. Так и сделаем:</p>

<pre><code class="language-cpp">void SortBy(vector&lt;Animal&gt;&amp; animals, AnimalSortKey sort_key, bool reverse = false) {
    switch (sort_key) {
        case AnimalSortKey::AGE:
			// возвращается void, но return помогает сразу выйти из функции
            return SortBy(animals, [](const auto&amp; x) { return x.age; }, reverse);
        case AnimalSortKey::WEIGHT:
            return SortBy(animals, [](const auto&amp; x) { return x.weight; }, reverse);
        case AnimalSortKey::RELATIVE_WEIGHT:
            return SortBy(animals, [](const auto&amp; x) { return x.weight / x.age; }, reverse);
    }
}
</code></pre>

<p>Всё работает ожидаемо: мы применили механизм перегрузки функций. Особенность этого примера в том, что функция с конкретными типами аргументов конкурирует с шаблонной и побеждает именно благодаря своей конкретности.</p>

<p>Но ведь вес и возраст есть не только у животных. Можно сделать функцию шаблонной по типу контейнера и получить специализацию шаблона. То есть уточнённую версию изначальной функции с типом <code>AnimalSortKey</code> в качестве <code>KeyMapper</code>:</p>

<pre><code class="language-cpp">template &lt;typename Container&gt;
void SortBy(Container&amp; animals, AnimalSortKey sort_key, bool reverse = false) {
    switch (sort_key) {
        case AnimalSortKey::AGE:
            return SortBy(animals, [](const auto&amp; x) { return x.age; }, reverse);
        case AnimalSortKey::WEIGHT:
            return SortBy(animals, [](const auto&amp; x) { return x.weight; }, reverse);
        case AnimalSortKey::RELATIVE_WEIGHT:
            return SortBy(animals, [](const auto&amp; x) { return x.weight / x.age; }, reverse);
    }
}
</code></pre>

<p>Хороший разработчик теперь уберёт из функции упоминание животных, чтобы подчеркнуть её гибкость.</p>

<p>Любители интерпретируемых языков подумают о следующем алгоритме:</p>

<ol>
<li>оставить одну функцию,</li>
<li>в начале у функции проверить тип <code>KeyMapper</code>,</li>
<li>пойти по нужной ветке: либо общей, либо конкретной.</li>
</ol>

<p>Применим шаблон <code>is_same_v</code> и напишем такой код:</p>

<pre><code class="language-cpp">template &lt;typename Container, typename KeyMapper&gt;
void SortBy(Container&amp; container, KeyMapper key_mapper, bool reverse = false) {
		// если KeyMapper — это AnimalSortKey...
    if (is_same_v&lt;KeyMapper, AnimalSortKey&gt;) {
        switch (key_mapper) {
            case AnimalSortKey::AGE:
                return SortBy(container, [](const auto&amp; x) { return x.age; }, reverse);
            case AnimalSortKey::WEIGHT:
                return SortBy(container, [](const auto&amp; x) { return x.weight; }, reverse);
            case AnimalSortKey::RELATIVE_WEIGHT:
                return SortBy(container, [](const auto&amp; x) { return x.weight / x.age; }, reverse);
        }
	    // вышли из функции, остальное снаружи if
    }

    if (reverse) {
        sort(container.begin(), container.end(),
             [key_mapper](const auto&amp; lhs, const auto&amp; rhs) {
                return key_mapper(lhs) &gt; key_mapper(rhs);
             });
    } else {
        sort(container.begin(), container.end(),
             [key_mapper](const auto&amp; lhs, const auto&amp; rhs) {
                return key_mapper(lhs) &lt; key_mapper(rhs);
             });
    }
}
</code></pre>

<p>Не компилируется. Сообщение об ошибке извещает: <code>In instantiation of ‘void SortBy(Container&amp;, KeyMapper, bool) [with Container = std::vector&lt;Animal&gt;; KeyMapper = main()::&lt;lambda(const Animal&amp;)&gt;]’: ... error: switch quantity not an integer</code>. При вызове <code>SortBy</code> с лямбдой из <code>main</code> в качестве <code>KeyMapper</code> компилятор споткнулся о <code>switch</code>. Дело в том, что оператор <code>if</code> работает уже при запуске программы. Поэтому компилируется вся функция целиком, а не конкретная её часть.</p>

<p>Подскажем, что выражение <code>is_same_v&lt;KeyMapper, AnimalSortKey&gt;</code> можно вычислить ещё до запуска программы, и применим <code>if constexpr</code>:</p>

<pre><code class="language-cpp">template &lt;typename Container, typename KeyMapper&gt;
void SortBy(Container&amp; container, KeyMapper key_mapper, bool reverse = false) {
    if constexpr (is_same_v&lt;KeyMapper, AnimalSortKey&gt;) {
				// ...
</code></pre>

<p>Получим другую ошибку компиляции: <code>‘key_mapper’ cannot be used as a function</code> с <code>KeyMapper = AnimalSortKey</code>. Снова компилируется лишняя часть функции, даже после гарантированного <code>return</code>. Исправим веткой <code>else</code>:</p>

<pre><code class="language-cpp">template &lt;typename Container, typename KeyMapper&gt;
void SortBy(Container&amp; container, KeyMapper key_mapper, bool reverse = false) {
    if constexpr (is_same_v&lt;KeyMapper, AnimalSortKey&gt;) {
        switch (key_mapper) {
            case AnimalSortKey::AGE:
                return SortBy(container, [](const auto&amp; x) { return x.age; }, reverse);
            case AnimalSortKey::WEIGHT:
                return SortBy(container, [](const auto&amp; x) { return x.weight; }, reverse);
            case AnimalSortKey::RELATIVE_WEIGHT:
                return SortBy(container, [](const auto&amp; x) { return x.weight / x.age; }, reverse);
        }
    } else {

        if (reverse) {
            sort(container.begin(), container.end(),
                 [key_mapper](const auto&amp; lhs, const auto&amp; rhs) {
                    return key_mapper(lhs) &gt; key_mapper(rhs);
                 });
        } else {
            sort(container.begin(), container.end(),
                 [key_mapper](const auto&amp; lhs, const auto&amp; rhs) {
                    return key_mapper(lhs) &lt; key_mapper(rhs);
                 });
        }

    }
}
</code></pre>

<p>Мы проделали интересное упражнение, но на практике предпочтительнее первый подход: написать отдельную функцию для конкретных типов параметров.</p>

</details>  

<details>  
<summary>Задание:</summary>

<p>Это задание — ваш итоговый проект первого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. А пока сохраните решение в своей IDE.</p>

<p>Верните методу <code>FindTopDocuments</code> возможность быть вызванным со статусом документа вместо лямбды.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">int main() {
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

    cout &lt;&lt; &quot;BANNED:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s, DocumentStatus::BANNED)) {
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
BANNED:
{ document_id = 3, relevance = 0.231049, rating = 9 }
Even ids:
{ document_id = 0, relevance = 0.173287, rating = 2 }
{ document_id = 2, relevance = 0.173287, rating = -1 }
</code></pre>

<h3>Подсказка</h3>

<p>Достаточно написать новый метод — <code>vector&lt;Document&gt; FindTopDocuments(const string&amp; raw_query, DocumentStatus status) const</code>. А в нём одну строчку: создали нужную лямбду и вызвали основную версию одноимённого метода.</p>

<p>Не забудьте избавиться от дублирования кода: версия <code>FindTopDocuments</code> с одним параметром теперь может вызывать новую версию этого метода — с <code>DocumentStatus</code>.</p>

</details>  

