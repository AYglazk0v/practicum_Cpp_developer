<details>  
<summary>Теория</summary>  
<h1>Методы классов</h1>

<p>Обратившись к полю класса и потерпев неудачу, вы столкнулись с инкапсуляцией — одним из принципов объектно-ориентированного программирования. Для внешнего пользователя класса объект должен выглядеть не как простой набор полей (словарь и множество), а как объект, с которым можно взаимодействовать через методы.</p>

<p>Внешний пользователь — это любой человек, работающий с готовым классом. Даже когда автор и пользователь — одно и то же лицо, полезно смотреть на свой код со стороны и проектировать его так, чтобы потом не было мучительно больно.</p>

<p>Добавьте в класс метод <code>AddDocument</code>. Он заменит одноимённую функцию:</p>

<pre><code class="language-cpp">class SearchServer {
		void AddDocument(int document_id, const string&amp; document) {
				for (const string&amp; word : SplitIntoWordsNoStop(document, stop_words_)) {
						word_to_documents_[word].insert(document_id);
				}
		}

		map&lt;string, set&lt;int&gt;&gt; word_to_documents_;
    set&lt;string&gt; stop_words_;
};
</code></pre>

<p>Этот метод выглядит как функция, но находится в классе. Он больше не принимает параметры <code>word_to_documents</code> и <code>stop_words</code>. Метод запускается через точку в контексте конкретного объекта — <code>server.AddDocument(id, document)</code>. При этом у него два явных параметра (id и содержимое документа) и один неявный — сам объект <code>server</code> типа <code>SearchServer</code>. Чтобы обратиться к полям неявного параметра, достаточно упомянуть их по названию, без точки: <code>word_to_documents_[word].insert(document_id);</code>.</p>

<p><code>insert</code> — такой же метод для множества, как <code>AddDocument</code> —  метод для класса <code>SearchServer</code>. У них даже суть похожа.</p>

<p>Правда, написать <code>server.AddDocument(id, document)</code> пока не выходит — компилятор говорит, что метод тоже приватный: <code>'void SearchServer::AddDocument(int, const string&amp;)' is private within this context</code>. Чтобы этого не происходило, нужно указать явно, что метод в классе публичный, а поля — приватные:</p>

<pre><code class="language-cpp">class SearchServer {
public:
		void AddDocument(int document_id, const string&amp; document) {
				for (const string&amp; word : SplitIntoWordsNoStop(document, stop_words_)) {
						word_to_documents_[word].insert(document_id);
				}
		}

private:
		map&lt;string, set&lt;int&gt;&gt; word_to_documents_;
    set&lt;string&gt; stop_words_;
};
</code></pre>

<p>Теперь вызвать метод <code>AddDocument</code> получится. Но обратиться к полям всё ещё не выходит. Как и задумано. Из тела метода доступ к полям класса всё же есть, ведь это код, написанный самим автором класса. А вот из функции <code>main</code> — нет.</p>

<p>По умолчанию все поля класса приватные, с чем вы и столкнулись в предыдущем уроке. Это единственное отличие класса и структуры. В структурах все поля по умолчанию публичные, но это не мешает добавить туда приватные поля и методы. Договорённость стилистическая: обычно структуры создают как простое объединение полей, а классы — как объекты с интерфейсом, то есть с набором методов.</p>

</details> 

<details>  
<summary>Задание 1:</summary>  
<p>Вы уже можете начать рефакторить код поисковой системы. Перенесите основную логику в класс <code>SearchServer</code>.</p>

<p>Начните с метода <code>AddDocument</code>, чтобы получился такой класс, как в примере. Названия полей остаются на ваше усмотрение, но сам класс надо назвать <code>SearchServer</code>, а его пока единственный метод — <code>AddDocument</code>. Проверки <code>main</code> и других функций, не нужных в <code>AddDocument</code>, в этот раз не будет. Но их код ещё предстоит переделать. Не теряйте его. Для комментирования кода в этой задаче используйте <em>многострочные</em> комментарии.</p>

<h3>Подсказка</h3>

<p>Замените функцию <code>AddDocument</code> на класс <code>SearchServer</code> из урока. Функции <code>SplitIntoWords</code> и <code>SplitIntoWordsNoStop</code> оставьте, а остальной код можете закомментировать.</p>

</details>
<details>  
<summary>Задание 2:</summary>  
<p>Вы ловко вызвали из метода <code>AddDocument</code> внешнюю функцию <code>SplitIntoWordsNoStop</code>. Делать так можно, но передача множества стоп-слов в функцию — спорное решение. Всё-таки это множество — часть поисковой системы.</p>

<p>Сделайте функцию <code>SplitIntoWordsNoStop</code> приватным методом: тогда никто снаружи класса не сможет её вызвать, а доступ к множеству стоп-слов она, как метод, получит «из коробки».</p>

<h3>Подсказка</h3>

<p>Перенесите функцию <code>SplitIntoWordsNoStop</code> внутрь класса, в самый его конец: под <code>private:</code> и поля. Оставьте у полученного метода один параметр — строку, а множество стоп-слов в теле метода получите из поля класса, как это уже сделано в <code>AddDocument</code>. Тогда при вызове <code>SplitIntoWordsNoStop</code> не нужно будет передавать стоп-слова.</p>

</details>

<details>  
<summary>Задание 3:</summary>  
<p>Для добавления документов в поисковый индекс есть метод <code>AddDocument</code>. Чтобы задать стоп-слова, превратите функцию <code>ParseStopWords</code> в метод <code>SetStopWords</code>. Он всё так же примет на вход строку с разделёнными пробелом словами, но не будет создавать и возвращать множество, а сразу добавит эти слова в соответствующее поле класса.</p>

<h3>Подсказка</h3>

<p>Перенесите функцию <code>ParseStopWords</code> внутрь класса (под <code>public:</code>!) и переименуйте в <code>SetStopWords</code>. Полученные стоп-слова добавляйте в множество <code>stop_words_</code> (это уже поле класса) с помощью метода <code>insert</code>.</p>

</details>

<details>  
<summary>Задание 4:</summary>  
<p>Звание интерфейса мечты нужно доказать. Ориентируясь на старый код функции <code>main</code>, напишите функцию <code>CreateSearchServer</code>: без параметров, считывает стоп-слова и документы, вызывает методы <code>SetStopWords</code> и <code>AddDocument</code>, возвращает готовый объект типа <code>SearchServer</code>.</p>

<h3>Подсказка</h3>

<p>Функция <code>CreateSearchServer</code> должна иметь пустой список параметров и возвращать <code>SearchServer</code>.</p>

<p>В первую очередь создайте объект для заполнения: <code>SearchServer search_server;</code></p>

<p>Затем считайте и задайте стоп-слова: <code>search_server.SetStopWords(/* тут строка из ввода */);</code></p>

<p>И, наконец, считайте количество документов и в цикле с соответствующим числом итераций вызывайте метод <code>search_server.AddDocument</code>, передавая id документа и его содержимое.</p>
</details>

<details>  
<summary>Задание 5:</summary>  
<p>Теперь вы готовы закончить рефакторинг самостоятельно. Сделайте функцию <code>FindAllDocuments</code> приватным методом, а <code>FindTopDocuments</code> — публичным. Константу <code>MAX_RESULT_DOCUMENT_COUNT</code>, структуру <code>Document</code> и компаратор <code>HasDocumentGreaterRelevance</code> оставьте пока снаружи класса.</p>

<p>Не забудьте о функции <code>main</code>. Создайте <code>SearchServer</code> с помощью уже готовой функции и вызовите метод <code>FindTopDocuments</code>.</p>

<h3>Подсказка</h3>

<ol>
<li>Перенесите <code>FindAllDocuments</code> в приватную часть класса, оставьте только один параметр — строку <code>query</code>. Переделать нужно все участки кода, в которых используется словарь <code>word_to_documents</code>, множество <code>stop_words</code> и функция (теперь уже метод) <code>SplitIntoWordsNoStop</code>.</li>
<li>Перенесите <code>FindTopDocuments</code> в публичную часть класса и проделайте те же преобразования.</li>
<li>В функции <code>main</code> вызовите <code>CreateSearchServer</code> и положите результат в переменную (пока не константную!). Затем вызовите метод <code>FindTopDocuments</code> и запустите цикл по его результату с выводом ответа.</li>
</ol>

<p>Все используемые в методах класса сущности — константы, типы, функции — объявите до кода класса.</p>

</details>

