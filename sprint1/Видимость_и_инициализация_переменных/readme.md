<details>  
<summary>Введение:</summary>
<p>В этой теме рассмотрим простые, но важные понятия: области видимости и инициализацию переменных. Вы вспомните, насколько самоотвержено C++ экономит ресурсы, и научитесь избегать связанных с этим ошибок.</p>
</details>  

<details>  
<summary>Области видимости переменных:</summary>

<h1>Области видимости переменных</h1>

<p>Объекты в С++ имеют некоторую область видимости — часть программы, где к ним можно обратиться по имени. Переменные используются в области своей видимости.</p>

<p>Возьмём функцию <code>ReadLine</code> из кода поисковой системы. Переменная <code>s</code> объявлена в начале функции, поэтому может применяться во всей функции:</p>

<pre><code class="language-cpp">string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}
</code></pre>

<p>Поменяем местами первые две строки:</p>

<pre><code class="language-cpp">string ReadLine() {
    getline(cin, s);
    string s;
    return s;
}
</code></pre>

<p>Эту функцию можем даже не вызывать — компилятор категорически откажется с этим работать:</p>

<pre><code>test.cpp: In function ‘std::string ReadLine()’:
test.cpp:7:18: error: ‘s’ was not declared in this scope
    7 |     getline(cin, s);
</code></pre>

<p>Фраза <code>‘s’ was not declared in this scope</code> значит буквально «имя <code>s</code> в месте вызова <code>getline</code> не объявлено». <code>Scope</code> — это и есть та самая область видимости.</p>

<p>Общее правило видимости таково: переменная видна, начиная от своего объявления и заканчивая ближайшей закрывающей операторной скобкой. Эти скобки применяют и независимо — как раз чтобы ограничить область видимости.</p>

<p>В этом фрагменте реализации метода <code>FindAllDocuments</code> переменная <code>inverse_document_freq</code> не может быть использована в первом условном операторе, а также после цикла:</p>

<pre><code class="language-cpp">for (const string&amp; word : query.plus_words) {
    // тут пока не видно
    if (word_to_document_freqs_.count(word) == 0) {
        continue;
    }
    // и тут всё ещё нет
    const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
    // а тут уже видно
    for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
        const auto&amp; document_data = documents_.at(document_id);
        if (document_predicate(document_id, document_data.status, document_data.rating)) {
            document_to_relevance[document_id] += term_freq * inverse_document_freq;
        }
    }
} 
// и снова не видно!
</code></pre>

<p>Переменная <code>word</code> объявлена в заголовке цикла — и потому видна во всём цикле, но не снаружи. Аналогично область видимости параметров функции — это всё тело функции.</p>

<p>Компилятор не запретит нам объявить переменную с уже существующим именем во вложенной области видимости:</p>

<pre><code class="language-cpp">int ComputeAverageRating(const vector&lt;int&gt;&amp; rating) {
    if (rating.empty()) {
        return 0;
    }
    int rating_sum = 0;
    for (const int rating : rating) {  // ого!
        rating_sum += rating;
    }
    return rating_sum / static_cast&lt;int&gt;(rating.size());
}
</code></pre>

<p>Этот код будет отлично работать. Но горе тому программисту, который попробует разобраться, что такое <code>rating</code> внутри цикла. Особенно если размер цикла превысил десяток-другой строк.</p>

<p>Начинающие программисты часто не думают об областях видимости и объявляют все необходимые переменные как можно раньше. А зря: это может привести к ошибкам.</p>

<p>Рассмотрим код, который по вектору слов <code>words</code> строит вектор <code>pure_words</code>. Слова в этом векторе очищены от спецсимволов, таких как дефис, плюс и восклицательный знак:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;string&gt;
#include &lt;vector&gt;

using namespace std;

int main() {
    const vector&lt;string&gt; words = {&quot;!кот&quot;s, &quot;-пушистый&quot;s};
    vector&lt;string&gt; pure_words;

    string pure_word;
    for (const string&amp; word : words) {
        for (const char c : word) {
            if (c != '-' &amp;&amp; c != '+' &amp;&amp; c != '!') {  // если c не спецсимвол
                pure_word.push_back(c);
            }
        }
        pure_words.push_back(pure_word);
    }

    for (const string&amp; word : pure_words) {
        cout &lt;&lt; word &lt;&lt; endl;
    }
}
</code></pre>

<p>На выходе будет не то, что ожидалось:</p>

<pre><code>кот
котпушистый
</code></pre>

<p>Это частая ошибка излишне экономных программистов: автор не хотел создавать новую строчку <code>pure_word</code> на каждой итерации цикла, но, похоже, забыл её очистить после <code>push_back</code>. Если эту переменную сразу объявить максимально глубоко, проблемы не будет:</p>

<pre><code class="language-cpp">for (const string&amp; word : words) {
    string pure_word;  // новая переменная для каждой итерации
    for (const char c : word) {
        if (c != '-' &amp;&amp; c != '+' &amp;&amp; c != '!') {  // если c не спецсимвол
            pure_word.push_back(c);
        }
    }
    pure_words.push_back(pure_word);
}
</code></pre>

<p>Код работает, но его всё ещё можно улучшить. Над словом <code>word</code> совершается конкретное действие — очистка от спецсимволов. Вынесем его в функцию:</p>

<pre><code class="language-cpp">string PurifyWord(const string&amp; word) {
    string pure_word;
    for (const char c : word) {
        if (c != '-' &amp;&amp; c != '+' &amp;&amp; c != '!') {  
            pure_word.push_back(c);
        }
    }
    return pure_word;
}

// ...

for (const string&amp; word : words) {
    const string pure_word = PurifyWord(word);
    pure_words.push_back(pure_word);
}
</code></pre>

<p>Мы объявили переменную <code>pure_word</code> константной и тем самым явно отметили: <code>word</code> преобразуется в <code>pure_word</code> функцией <code>PurifyWord</code>. Больше с этой переменной ничего не происходит.</p>

<p>Без отдельной переменной программа тоже будет работать:</p>

<pre><code class="language-cpp">for (const string&amp; word : words) {
    pure_words.push_back(PurifyWord(word));
}
</code></pre>

<p>Этот код не только короче, но и эффективнее. Мы не только грамотно выбрали область видимости для переменной <code>pure_word</code>, но и разделили две разные области:</p>

<ul>
<li>удаление спецсимволов переносом в функцию,</li>
<li>добавление результата в вектор.</li>
</ul>

<p>Всё это сделало код более понятным и легко тестируемым.</p>

</details>  

<details>  
<summary>Инициализация переменных:</summary>

<h1>Инициализация переменных</h1>

<p>Вы уже сталкивались с опасностью неинициализированных числовых переменных. Разберём на примере. Объявим переменную <code>x</code>, прибавим к ней восемь и выведем:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;

using namespace std;

int main() {
    // другие операции
    int x;  // не задали начальное значение
    x += 8;
    cout &lt;&lt; x &lt;&lt; endl;
}
</code></pre>

<p>Если это первые команды в программе, она, скорее всего, выведет <code>8</code>. Если нет, можем получить произвольное число — например <code>21889</code>. При этом компилятор видит проблему и даёт предупреждение:</p>

<pre><code>test.cpp: In function ‘int main()’:
test.cpp:14:7: warning: ‘x’ is used uninitialized in this function [-Wuninitialized]
   14 |     x += 8;
      |     ~~^~~~
</code></pre>

<p>Это наглядная демонстрация одного из базовых принципов C++: не плати за то, что не используешь. Переменная не инициализирована. Автор кода написал: хочу переменную. Переменная появилась, а тратить лишнюю операцию на её зануление нет причин. В мультфильме «Вовка в Тридевятом царстве» двое из ларца получили от Вовки размытые инструкции. И сделали совсем не то, чего он ожидал. С++ тоже требует чётких инструкций: хотите нулевой <code>x</code> — так и напишите.</p>

<p>Инициализируйте все переменные, которые так или иначе относятся к числовым:</p>

<ul>
<li>целочисленные — <code>int</code>, <code>size_t</code>, <code>int64_t</code> и пр.;</li>
<li>вещественные — <code>float</code>, <code>double</code> и пр.;</li>
<li><code>char</code>;</li>
<li><code>bool</code>;</li>
<li><code>enum</code>.</li>
</ul>

<p>Создание таких локальных переменных — просто резервирование области памяти нужного размера. С точки зрения времени выполнения программы это бесплатно. С более сложными объектами — строками, векторами, словарями — дела обстоят иначе. Создавая их без дополнительной информации, вы гарантированно получите пустой объект.</p>

<p>Та же история с вашими собственными структурами и классами: по умолчанию их поля инициализируются подобно локальным переменным. Как правильно задавать начальные значения полей, вы узнаете в теме о конструкторах.</p>

<p>При этом стандартные классы заботятся о пользователе: созданная по умолчанию пара чисел <code>pair&lt;int, int&gt; x;</code> или вектор, длина которого равна пяти — <code>vector&lt;int&gt; v(5);</code>, — будут содержать нули.</p>

<p>Редкий пример ситуации, когда явная инициализация переменной избыточна:</p>

<pre><code class="language-cpp">int document_count;
cin &gt;&gt; document_count;
</code></pre>

<p>Переменная заполнилась конкретным значением сразу после объявления. Но в таком случае мы не можем объявить переменную константной. Отчасти поэтому, а отчасти из-за более сложного считывания в коде поисковой системы мы инициализировали количество документов по-другому:</p>

<pre><code class="language-cpp">const int document_count = ReadLineWithNumber();
</code></pre>

<p>Применим похожий приём, чтобы заполнить переменную разными значениями в зависимости от условия:</p>

<pre><code class="language-cpp">int result_temperature;
if (mode == Mode::MIN) {
    result_temperature = min(temperature_today, temperature_yesterday);
} else {
    result_temperature = max(temperature_today, temperature_yesterday);
}
</code></pre>

<p>Такой код опасен: если добавить команды перед <code>if</code> или заменить последний <code>else</code> на <code>else if</code> и добавить в <code>Mode</code> третье значение, в переменной <code>result_temperature</code> может появиться мусор. Исправить получится даже без функции. Применим так называемый тернарный оператор:</p>

<pre><code class="language-cpp">const int result_temperature =
    mode == Mode::MIN
    ? min(temperature_today, temperature_yesterday)
    : max(temperature_today, temperature_yesterday);
</code></pre>

<p>Выражение после символа <code>=</code> состоит из трёх частей:</p>

<ul>
<li>до <code>?</code> записано условие, как в <code>if</code>;</li>
<li>между <code>?</code> и <code>:</code> записано значение, которое будет результатом выражения, если условие истинно;</li>
<li>после <code>:</code> записано значение, которое будет результатом выражения, если условие ложно.</li>
</ul>

<p>Возьмите себе за правило всегда инициализировать переменные и поля числовых типов. И не забывайте включать предупреждения компилятора.</p>


</details>  
