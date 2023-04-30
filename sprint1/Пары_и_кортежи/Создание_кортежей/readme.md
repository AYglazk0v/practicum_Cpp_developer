<details>  
<summary>Создание кортежей:</summary>

<p>Вы научились сравнивать структуры, применяя кортеж. Познакомимся с ним поближе.</p>

<p>При создании кортежа объекты копируются в него:</p>

<pre><code class="language-cpp">string name = &quot;Василий&quot;s;
const tuple animal_info(name, 5, 4.1);
name = &quot;Вася&quot;s;  // в animal_info остался Василий
</code></pre>

<p>У этого есть недостаток. Когда создаём кортеж для немедленного сравнения с другим кортежем, тяжёлые объекты могут скопироваться зря. Так произойдёт и с сортировкой <code>AnimalObservation</code>:</p>

<pre><code class="language-cpp">struct AnimalObservation {
	string name;
	int days_ago;
	int health_level;

	auto MakeKey() const {
		return tuple(days_ago, health_level, name);
		// потенциально тяжёлая строка name копируется в создаваемый кортеж
	}
};

// ...

sort(observations.begin(), observations.end(),
	[](const AnimalObservation&amp; lhs, const AnimalObservation&amp; rhs) {
		return lhs.MakeKey() &lt; rhs.MakeKey();
	});
</code></pre>

<p>Чтобы избавиться от лишнего копирования, применим функцию <code>tie</code> из библиотеки <code>&lt;tuple&gt;</code>:</p>

<pre><code class="language-cpp">auto MakeKey() const {
	return tie(days_ago, health_level, name);
}
</code></pre>

<p>Разберёмся, в чём разница. Специально вызовем ошибку компиляции и посмотрим на тип, возвращаемый сейчас методом <code>MakeKey</code>:</p>

<pre><code class="language-cpp">+AnimalObservation{}.MakeKey();  // создали структуру AnimalObservation,
                                 // не инициализируя поля, сразу вызвали MakeKey(),
								 // а к результату применили оператор +
</code></pre>

<p>Бо́льшую часть текста ошибки занимает описание типа, к которому не удалось применить оператор +:</p>

<pre><code>no match for 'operator+'
(operand type is 'std::tuple&lt;const int&amp;, const int&amp;, const std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;&amp;&gt;')
</code></pre>

<p>Функция <code>tie</code> вернула кортеж, хранящий не сами объекты, а ссылки на них: <code>const int&amp;</code>, <code>const int&amp;</code> и <code>const string&amp;</code>. Тип <code>string</code> записан своим оригинальным именем с применением <code>basic_string</code>. Ссылки при этом оказались константными благодаря константности самого метода <code>MakeKey</code>.</p>

<p>Создание кортежа из ссылок не требует копировать объекты. Однако этот подход неуниверсален, поскольку создать ссылку на временный объект нельзя:</p>

<pre><code class="language-cpp">struct Document {
    int id;
    Status status;
    double relevance;
    int rating;
    
    auto MakeKey() const {
        return tie(status, -rating, -relevance);
    }
};

// ошибка компиляции: cannot bind non-const lvalue reference of type 'int&amp;'
// to an rvalue of type 'int'
</code></pre>

<p>Ошибка компиляции буквально переводится так: нельзя привязать ссылку <code>int&amp;</code> к временному объекту типа <code>int</code>. Дело в том, что объекты <code>-rating</code> и <code>-relevance</code> создались внутри метода. Они уничтожаются сразу по окончании выражения, где были созданы. Ссылку на такие временные объекты создать нельзя.</p>

<p>Если создаёте кортеж и из ссылок, и из самих значений, укажите типы явно:</p>

<pre><code class="language-cpp">struct AnimalObservation {
    string name;
    int days_ago;
    int health_level;

    // в заголовке метода теперь указан полный тип
	  tuple&lt;int, int, const string&amp;&gt; MakeKey() const {
        // создаём и сразу возвращаем объект:
        // явно указывать его тип не нужно, достаточно указать аргументы конструктора
        // в фигурных скобках
        return {days_ago, -health_level, name};
    }
};
</code></pre>

</details>

<details>  
<summary>Возврат нескольких значений из функции:</summary>
<h1>Возврат нескольких значений из функции</h1>

<p>Вы научились создавать и сравнивать кортежи. А чтобы получить доступ к отдельным элементам кортежа, придётся написать довольно громоздкую конструкцию:</p>

<pre><code class="language-cpp">const tuple animal_info(&quot;Василий&quot;s, 5, 4.1);
cout &lt;&lt; &quot;Пациент &quot;s &lt;&lt; get&lt;0&gt;(animal_info)
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;1&gt;(animal_info) &lt;&lt; &quot; лет&quot;s
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;2&gt;(animal_info) &lt;&lt; &quot; кг&quot;s &lt;&lt; endl;
// Пациент Василий, 5 лет, 4.1 кг
</code></pre>

<p>Обратиться к полю можно и по его типу вместо индекса. При условии, если это единственное поле указанного типа:</p>

<pre><code class="language-cpp">const tuple animal_info(&quot;Василий&quot;s, 5, 4.1);
cout &lt;&lt; &quot;Пациент &quot;s &lt;&lt; get&lt;string&gt;(animal_info)
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;int&gt;(animal_info) &lt;&lt; &quot; лет&quot;s
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;double&gt;(animal_info) &lt;&lt; &quot; кг&quot;s &lt;&lt; endl;
// Пациент Василий, 5 лет, 4.1 кг
</code></pre>

<p>У обоих способов те же недостатки, что у пары с полями <code>first</code> и <code>second</code>: неясно, что лежит в месте использования. Больше подойдёт структура с полями <code>name</code>, <code>age</code> и <code>weight</code>.</p>

<p>Тем не менее кортежи используют для возврата нескольких значений из функции. При этом к их содержимому обращаются, применив знакомую вам распаковку:</p>

<pre><code class="language-cpp">class SearchServer {
public:
	tuple&lt;vector&lt;string&gt;, DocumentStatus&gt; MatchDocument(const string&amp; raw_query, int document_id) const {
		// ...
	}

	// ...
};

// ...

const auto [words, status] = search_server.MatchDocument(&quot;белый кот&quot;s, 2);
</code></pre>

<p>Из метода возвращаются два объекта: <code>vector&lt;string&gt;</code> и <code>DocumentStatus</code>. В отличие от случаев, когда структура специально определена, у них нет самостоятельных названий. Догадаться о смысле объектов можно по типам и названию метода. Первый объект — это вектор слов запроса, которые нашлись в документе <code>document_id</code>, а второй объект — статус документа.</p>

<p>Допустим, мы применяем кортеж, чтобы вернуть объекты одинаковых или более тривиальных типов. Для этого потребовалось бы определить структуру с конкретными названиями полей:</p>

<pre><code class="language-cpp">// так непонятно
tuple&lt;int, int, double&gt; CheckDocument(/* ... */);

struct DocumentCheckResult {
	int word_count;
	int rating;
	double relevance;
};

// а так гораздо лучше
DocumentCheckResult CheckDocument(/* ... */);
</code></pre>

<p>Распаковка справится и с кортежем, и со своей структурой:</p>

<pre><code class="language-cpp">const auto [word_count, rating, relevance] = CheckDocument(/* ... */);
</code></pre>

<p>Распаковка — компактный способ сохранить несколько значений из функции.</p>

</details>  

<details>  
<summary>Задание:</summary>
<p>Реализуйте метод <code>MatchDocument</code>:</p>

<pre><code class="language-cpp">tuple&lt;vector&lt;string&gt;, DocumentStatus&gt; MatchDocument(const string&amp; raw_query, int document_id) const;
</code></pre>

<p>В первом элементе кортежа верните все плюс-слова запроса, содержащиеся в документе. Слова не должны дублироваться. Пусть они будут отсортированы по возрастанию.</p>

<p>Если документ не соответствует запросу (нет пересечений по плюс-словам или есть минус-слово), вектор слов нужно вернуть пустым.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">void PrintMatchDocumentResult(int document_id, const vector&lt;string&gt;&amp; words, DocumentStatus status) {
    cout &lt;&lt; &quot;{ &quot;s
         &lt;&lt; &quot;document_id = &quot;s &lt;&lt; document_id &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;status = &quot;s &lt;&lt; static_cast&lt;int&gt;(status) &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;words =&quot;s;
    for (const string&amp; word : words) {
        cout &lt;&lt; ' ' &lt;&lt; word;
    }
    cout &lt;&lt; &quot;}&quot;s &lt;&lt; endl;
}

int main() {
    SearchServer search_server;
    search_server.SetStopWords(&quot;и в на&quot;s);

    search_server.AddDocument(0, &quot;белый кот и модный ошейник&quot;s,        DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, &quot;пушистый кот пушистый хвост&quot;s,       DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, &quot;ухоженный пёс выразительные глаза&quot;s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    search_server.AddDocument(3, &quot;ухоженный скворец евгений&quot;s,         DocumentStatus::BANNED, {9});

    const int document_count = search_server.GetDocumentCount();
    for (int document_id = 0; document_id &lt; document_count; ++document_id) {
        const auto [words, status] = search_server.MatchDocument(&quot;пушистый кот&quot;s, document_id);
        PrintMatchDocumentResult(document_id, words, status);
    }
}
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>{ document_id = 0, status = 0, words = кот}
{ document_id = 1, status = 0, words = кот пушистый}
{ document_id = 2, status = 0, words =}
{ document_id = 3, status = 2, words =}
</code></pre>

<h3>Подсказка</h3>

<p>Проанализируйте запрос существующим методом и пройдитесь по плюс- и минус-словам. Совпадающие плюс-слова добавьте к ответу. Совпадение любого минус-слова значит немедленный возврат из функции с пустым вектором.</p>
</details>  
