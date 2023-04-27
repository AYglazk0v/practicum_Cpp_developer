<details>  
<summary>Теория_1</summary>  
<h1>Введение</h1>

<p>Вы уже увидели, как создавать и менять контейнеры — строку, вектор, множество, словарь, пару. В этой теме вы разработаете свой тип данных «поисковая система» с соответствующим функционалом.</p>

<p>Типы без дополнительной логики называют структурами, а типы с методами — классами. Приятная особенность C++ в том, что все его встроенные типы кроме стандартных <code>int</code>, <code>char</code>, <code>double</code>, <code>bool</code> — это классы, реализованные на C++. Если научитесь писать собственные типы, лучше разберётесь в стандартных.</p>

<p>Как и функции, структуры и классы делают код более понятным. Его проще тестировать и переиспользовать. Разница в том, что функция — законченное действие с ясным входом и выходом, а класс и структура — это объекты. У объекта может быть дополнительная логика в виде методов.</p>

<p>Данная тема — первый шаг в изучении объектно-ориентированного программирования. Она затронет одно из главных понятий ООП — инкапсуляцию.</p>
</details>  
  
 <details>  
<summary>Теория_2</summary>  


<h1>Недостатки пар</h1>

<p>В прошлой теме вы научили свою поисковую систему показывать самые релевантные результаты. Для этого вы использовали пару целых чисел <code>{document_id, relevance}</code>. Функция <code>FindDocuments</code> стала выглядеть так:</p>

<pre><code class="language-cpp">// For each document returns its id and relevance
vector&lt;pair&lt;int, int&gt;&gt; FindTopDocuments(
		const map&lt;string, vector&lt;int&gt;&gt;&amp; word_to_documents,
		const set&lt;string&gt;&amp; stop_words,
		const string&amp; query);
</code></pre>

<p>Но у контейнера <code>pair</code> есть недостатки.</p>

<ol>
<li>Непонятность. Без текстового комментария неясно, что хранится в <code>pair&lt;int, int&gt;</code>. А текстовые комментарии ненадёжны. Если второй <code>int</code> станет обозначать возраст, а не релевантность, разработчик может забыть обновить комментарий. Тогда об изменениях никто не узнает. Этот недостаток виден в коде выше.</li>
<li>Непрозрачность. В некоторых сценариях она доходит до криптографичности, как в этом коде обработки результата функции:</li>
</ol>

<pre><code class="language-cpp">for (const pair&lt;int, int&gt;&amp; document : FindTopDocuments(word_to_documents,
                                                       stop_words,
                                                       query)) {
		if (document.second &gt; THRESHOLD) {
				cout &lt;&lt; document.first &lt;&lt; endl;
		}
}
</code></pre>

<p>Нужны дополнительные комментарии, чтобы понять, что за <code>first</code> и <code>second</code> и в чём смысл сравнения <code>second</code> с неким порогом. Проще было бы воспринимать такое тело цикла:</p>

<pre><code class="language-cpp">if (document.relevance &gt; THRESHOLD) {
		cout &lt;&lt; document.id &lt;&lt; endl;
}
</code></pre>

<p>Сразу ясно: в цикле выводятся id всех документов с достаточно высокой релевантностью.</p>

<ol>
<li>Трудоёмкость. Если захотите вместо <code>int</code> поставить тип релевантности <code>double</code>, <code>pair&lt;int, int&gt;</code> придётся везде заменить на <code>pair&lt;int, double&gt;</code>. Особенно неприятно, когда пара <code>int</code> хранит в программе что-нибудь ещё. Например, рост и возраст человека.</li>
</ol>

<p>Пары — полезный контейнер со своей областью применения. Но для данного этапа разработки поисковой системы больше подойдёт другой тип данных.</p>

</details>  

 <details>  
<summary>Теория_3</summary>  
<h1>Структура</h1>

<p>Итак, вместо <code>pair</code> нужен другой тип данных для хранения информации о документе: id и релевантности. Сценарий применения объектов этого типа уже есть:</p>

<pre><code class="language-cpp">if (document.relevance &gt; THRESHOLD) {
		cout &lt;&lt; document.id &lt;&lt; endl;
}
</code></pre>

<p>По сути требуется объединить два целых числа в один тип и дать им названия: <code>id</code> и <code>relevance</code>. Для такой задачи подходит структура — тип данных, в котором можно объединить переменные разных типов. В С++ структура обозначается ключевым словом <code>struct</code>. Новый тип данных объявляется вне функции <code>main</code>:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;

using namespace std;

struct Document {
    int id;
    int relevance;
};

int main() {
		Document document = {51894, 8};
    cout &lt;&lt; document.id &lt;&lt; &quot; &quot;s &lt;&lt; document.relevance &lt;&lt; endl;
}
</code></pre>

<p>Вывод программы:</p>

<pre><code>51894 8
</code></pre>

<p>Чтобы объявить структуру, нужны:</p>

<ol>
<li>Ключевое слово <code>struct</code>. Как и слова вроде <code>using</code>, <code>namespace</code> или <code>int</code>, его нельзя использовать в качестве названия переменной или функции.</li>
<li>Название нового типа. Обычно его пишут с большой буквы, а при наличии нескольких слов в нём — <code>ВотТак</code>. Но в ряде кодовых баз могут быть другие правила. Регистр имеет значение: в нашей программе <code>Document</code> — тип, а <code>document</code> — переменная этого типа. Удобно и понятно.</li>
<li>Типы и названия полей в фигурных скобках — в том же формате, что объявление переменных <code>int id;</code> и <code>int relevance;</code>.</li>
<li>Точка с запятой после фигурных скобок. Про неё часто забывают, потому что после таких же фигурных скобок, обозначающих тело функции или цикла, точка с запятой не ставится. Забывать не стоит.</li>
</ol>

<pre><code class="language-cpp">struct Document {
    int id;
    int relevance;
}
// ошибка в этой строке: «error: expected ';' after struct definition»

int main() {

}
</code></pre>

<p>Содержимое описывают в фигурных скобках:</p>

<pre><code class="language-cpp">Document document = {51894, 8};
</code></pre>

<p>Так <code>Document</code> стал полноправным типом! Его можно сделать константным — <code>const Document</code>. Можно передать в функцию, вернуть из функции. Можно даже создать вектор документов:</p>

<pre><code class="language-cpp">vector&lt;Document&gt; documents = {{51894, 8}, {51895, 1}, {51896, 9}};
</code></pre>

<p>Фигурные скобки позволяют создать структуру «на лету»:</p>

<pre><code class="language-cpp">int id;
int relevance;
cin &gt;&gt; id &gt;&gt; relevance;
documents.push_back({id, relevance});
</code></pre>

<p>К полям обращаются только по их названию. Если ошибиться в названии поля, компилятор сообщит об этом:</p>

<pre><code class="language-cpp">struct Document {
    int id;
    int relevance;
};

int main() {
    Document document = {51894, 8};
    document.size = 8;
		// error: 'struct Document' has no member named 'size'
}
</code></pre>

</details>  

</details>  
<summary>Задание:</summary>  

<p>Начнём избавляться от пар в коде поисковой системы. Вам понадобятся функции из урока «‎Ранжирование по релевантности» из предыдущей темы. Перепишите на структуру <code>Document</code> функцию <code>FindAllDocuments</code>, которая возвращает все подходящие документы. Объявите структуру <code>Document</code> так же, как в уроке. Замените все пары на структуры. Верните из функции вектор этих структур.</p>

<h3>Подсказка</h3>

<p>Замените вектор пар на вектор документов — <code>vector&lt;Document&gt;</code>. Важен порядок полей при добавлении документа в вектор: сначала id, потом релевантность!</p>
</body>
</details>

