<details>  
<summary>Введение</summary>  
<h1>Введение</h1>

<p>В этой теме вы познакомитесь с лямбда-функциями — «синтаксическим сахаром», который делает код более лаконичным и понятным.</p>

<p>Как любое другое средство языка, лямбда-функции могут принести пользу, а могут навредить. Вы узнаете о границах применимости таких функций и будете учиться извлекать из них только пользу.</p>

</details> 
<details>  
<summary>Теория:</summary>  
<h1>Компаратор для сортировки</h1>

<p>В вашей поисковой системе есть метод <code>FindTopDocuments</code>, который сортирует документы по релевантности:</p>

<pre><code class="language-cpp">vector&lt;Document&gt; FindTopDocuments(const string&amp; query) const {			
		auto matched_documents = FindAllDocuments(query);
		
		sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
		if (matched_documents.size() &gt; MAX_RESULT_DOCUMENT_COUNT) {
			  matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
}
</code></pre>

<p>Посмотрите на вызов сортировки. В нём вы применили компаратор <code>HasDocumentGreaterRelevance</code>. У этого компаратора довольно простое тело:</p>

<pre><code class="language-cpp">bool HasDocumentGreaterRelevance(const Document&amp; lhs, const Document&amp; rhs) {
	return lhs.relevance &gt; rhs.relevance;
}
</code></pre>

<p>Но у метода <code>FindTopDocuments</code> есть проблема. Читая его, нужно переходить в другую часть кода, чтобы посмотреть критерий сортировки. У вашего компаратора понятное имя, однако лучше видеть именно код, который будет выполняться.</p>

<p>Сам компаратор недлинный. Удобнее, если его тело находится рядом с вызовом сортировки. Так его можно быстро прочитать и понять, по какому критерию сортируются документы.</p>

<p>Когда удобнее и понятнее иметь в месте вызова тело функции, а не её название, применяют лямбда-функции, или безымянные функции. Вот как можно переписать метод <code>FindTopDocuments</code> с лямбда-функцией в качестве компаратора:</p>

<pre><code class="language-cpp">vector&lt;Document&gt; FindTopDocuments(const string&amp; query) const {			
		auto matched_documents = FindAllDocuments(query);
		
		sort(
			matched_documents.begin(),   
			matched_documents.end(), 
			[](const Document&amp; lhs, const Document&amp; rhs) {    // Это лямбда-функция
				return lhs.relevance &gt; rhs.relevance;
			}
		);
		if (matched_documents.size() &gt; MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}
</code></pre>

<p>Третьим параметром функции <code>sort</code> вместо <code>HasDocumentGreaterRelevance</code> передано что-то похожее на определение функции. Сначала идёт пара квадратных скобок — <code>[]</code>. Они сообщают компилятору, что это лямбда-функция. Дальше в круглых скобках — список параметров, как у обычной функции. После в фигурных скобках идёт тело — тоже как в обычных функциях.</p>

<p>У лямбда-функции нигде не указывают тип возвращаемого значения. Компилятор сам выводит его по типу выражения в команде <code>return</code>. Ваша лямбда-функция возвращает результат сравнения двух целых чисел, поэтому тип возвращаемого значения — <code>bool</code>.</p>

<p>Добавив лямбда-функцию в метод <code>FindTopDocuments</code>, сразу видим, по какому критерию выполняется сортировка. К тому же больше не надо запоминать, что значит имя <code>HasDocumentGreaterRelevance</code>. Оно всё равно использовано всего один раз. Читатель теперь не перегружен лишними именами.</p>

</details> 

<details>  
<summary>Задание 1:</summary>  
<p>Внедрите лямбда-функцию в код метода <code>FindTopDocuments</code> вашей поисковой системы.</p>
<h3>Подсказка</h3>
<p>Выполните ту же замену, которую мы сделали в тексте урока.</p>
</details> 

<details>  
<summary>Задание 2:</summary>  
<h1>Компаратор для сортировки</h1<p>В стандартном потоке дана одна строка, состоящая из числа N и следующих за ним N строк S. Между собой число и строки разделены пробелом.</p>

<p>Отсортируйте строки S в лексикографическом порядке по возрастанию, игнорируя регистр букв, и выведите их в стандартный поток вывода через пробел.</p>

<p>После последней строки также выведите пробел и, затем, перевод строки.</p>

<h3>Ограничения</h3>

<ul>
<li>0 &lt;= N &lt;= 1000</li>
<li>1 &lt;= |S| &lt;= 15</li>
<li>Каждая строка S[i] может состоять из следующих символов: [0-9,a-z,A-Z]</li>
</ul>

<h3>Пример</h3>

<p><strong>Ввод</strong></p>

<pre><code class="language-cpp">4 quantity AMOUNT Selection Search
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code class="language-cpp">AMOUNT quantity Search Selection
</code></pre>

<h3>Подсказка</h3>

<p>Обратите внимание на <a href="http://www.cplusplus.com/reference/cctype/tolower">функцию tolower</a> и <a href="https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare">алгоритм lexicographical_compare</a>.</p>
>

