
<details>  
<summary>Теория_1</summary>  
<h1>Зачем нужны классы</h1>

<p>Вы добавили структуру <code>Document</code> в код поисковой системы. Распаковать структуру при итерировании по вектору можно так:</p>

<pre><code class="language-cpp">for (auto [document_id, relevance] : FindTopDocuments(word_to_documents, stop_words, query)) {
		// ...
}
</code></pre>

<p>Тогда обращаться к полям структуры через точку не придётся — они сразу разложены по переменным.</p>

<p>У вас в поисковой системе три функции:</p>

<pre><code class="language-cpp">void AddDocument(
				map&lt;string, set&lt;int&gt;&gt;&amp; word_to_documents,
        const set&lt;string&gt;&amp; stop_words,
        int document_id,
				const string&amp; document) {
		
		// ...
}

vector&lt;Document&gt; FindAllDocuments(
				const map&lt;string, set&lt;int&gt;&gt;&amp; word_to_documents,
				const set&lt;string&gt;&amp; stop_words,
				const string&amp; query) {
	
		// ...
}

vector&lt;Document&gt; FindTopDocuments(
				const map&lt;string, set&lt;int&gt;&gt;&amp; word_to_documents,
				const set&lt;string&gt;&amp; stop_words,
				const string&amp; query) {
			
		// ...
}
</code></pre>

<p>В каждую функцию передаётся основа поисковой системы — словарь <code>word_to_documents</code>. Для парсинга документов и для поискового запроса нужно множество стоп-слов.</p>

<p>Когда проектируете код, то есть разбиваете его на функции и выделяете самостоятельные типы данных, важно думать о масштабируемости. Охапка из трёх функций масштабируется плохо. Если станете менять тип <code>word_to_documents</code>, придётся делать это сразу в трёх местах. Добавлять новый параметр — например, словарь частотности слов — тоже нужно во все три функции. А что будет, если появятся новые функции, и представить страшно.</p>

<p>Чтобы при масштабировании объём кода не вышел из-под контроля, нужно объединить несколько объектов по смыслу и объявить новый тип данных. Вы уже делали так со <code>struct</code>. А теперь создайте <code>class</code>:</p>

<pre><code class="language-cpp">class SearchServer {
		map&lt;string, set&lt;int&gt;&gt; word_to_documents_;
    set&lt;string&gt; stop_words_;
};
</code></pre>

<p>Отличия классов и структур:</p>

<ul>
<li>у <code>struct</code> поля по умолчанию открытые, а у <code>class</code> — приватные;</li>
<li>наследование  у <code>struct</code> тоже открытое по умолчанию, а у <code>class</code> — приватное.</li>
</ul>

<p>Публичные поля структуры уязвимы — кто угодно может изменить содержимое поля в обход задуманной автором логики и сломать инварианты.</p>

<p>Класс скрывает свои данные. Он ориентирован на наличие в нём методов. Работать с классом по умолчанию можно только через методы. В этом идея инкапсуляции.</p>

<p>При масштабировании кода с классом сможем изменить способ хранения данных, переписав только код внутри класса. Но при этом нельзя будет менять заголовки методов.</p>

<p>Класс подходит для масштабирования поисковой системы, потому что с ним можно будет добавить новый объект в контекст всех функций, не меняя их заголовки.</p>

<p>Объединив объекты в <code>class</code>, защитите свой код от незапланированных изменений и сделаете его легко масштабируемым.</p>

</details> 

<summary>Задание:</summary>  
<p>Посмотрите, как работает тип данных <code>class</code>.</p>

<ol>
<li>Объявите в функции <code>main</code> переменную типа <code>SearchServer</code> и обратитесь к одному из её полей — например, выведите количество стоп-слов во множестве. Вы получите ошибку компиляции об обращении к приватному полю. Это нормально.</li>
</ol>

<p>Подчёркивания в названиях необязательны. Но они показывают, что обратиться к полям напрямую не выйдет:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;map&gt;
#include &lt;set&gt;
#include &lt;string&gt;
#include &lt;vector&gt;

using namespace std;

class SearchServer {
    map&lt;string, set&lt;int&gt;&gt; word_to_documents_;
    set&lt;string&gt; stop_words_;
};

int main() {
    SearchServer server;
    cout &lt;&lt; server.stop_words_.size() &lt;&lt; endl;
    return 0;
}
</code></pre>

<ol>
<li><p>Напишите прямо внутри класса функцию, которая будет возвращать нужный размер множества стоп-слов. Такая функция называется метод. Так как метод находится внутри класса, он имеет доступ к его полям. Чтобы вызвать метод, снабдите его пометкой <code>public:</code> как показано в примере кода.</p></li>

<li><p>Сделайте свободную функцию <code>GetStopWordsSize</code>. Вызовите в ней вышеописанный метод у переданного в функцию по константной ссылке сервера.</p></li>
</ol>

<h3>Подсказка</h3>

<p>Вызовите метод <code>size</code> множества в методе <code>GetStopWordsSize</code>, и метод <code>GetStopWordsSize</code> сервера в функции <code>GetStopWordsSize</code>.</p>

</details>

