<details>  
<summary>Теория</summary>  
<h1>Константные методы</h1>

<p>Если попробовать объявить объект поисковой системы константным, ничего не выйдет:</p>

<pre><code class="language-cpp">int main() {
		const SearchServer search_server = CreateSearchServer();
	
		const string query = ReadLine();
		for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
		// error: passing 'const SearchServer' as 'this' argument discards qualifiers
				cout &lt;&lt; &quot;{ document_id = &quot; &lt;&lt; document_id &lt;&lt; &quot;, relevance = &quot; &lt;&lt; relevance &lt;&lt; &quot; }&quot; &lt;&lt; endl;
		}
}
</code></pre>

<p>Вызвать <code>FindTopDocuments</code> для константного объекта <code>search_server</code> нельзя. Аналогичная ошибка выскочит, если вызвать, например, <code>push_back</code> от константного вектора. Он константный, его не меняют.</p>

<p>Почему компилятор решил, что <code>FindTopDocuments</code> изменит объект, непонятно. Ведь метод <code>size</code> для константного объекта вызвать можно. Время разобраться, как сделать то же в классе.</p>

<p>Ответ логичен и запутан. Методы, не меняющие объект, в контексте которого вызваны, нужно помечать константными. Чтобы стало понятнее, посмотрите на метод <code>FindTopDocuments</code>:</p>

<pre><code class="language-cpp">class SearchServer {
public:
		void SetStopWords(const string&amp; text) { /* ... */ }

		void AddDocument(int document_id, const string&amp; document) { /* ... */ }

		vector&lt;Document&gt; FindTopDocuments(const string&amp; query) const /* ВНИМАНИЕ СЮДА! */ {
				auto matched_documents = FindAllDocuments(query);
				
				sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
				if (matched_documents.size() &gt; MAX_RESULT_DOCUMENT_COUNT) {
					matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
				}
				return matched_documents;
		}

private:
		map&lt;string, set&lt;int&gt;&gt; word_to_documents_;
    set&lt;string&gt; stop_words_;

		vector&lt;string&gt; SplitIntoWordsNoStop(const string&amp; text) { /* ... */ }

		vector&lt;Document&gt; FindAllDocuments(const string&amp; query) { /* ... */ }
};
</code></pre>

<p>В заголовок метода <code>FindTopDocuments</code> добавлено слово <code>const</code> — между списком параметров и телом метода. В итоге:</p>

<ol>
<li>Поля класса при обращении к ним в теле метода стали константными. Изменить их не получится. С <code>FindTopDocuments</code> таких проблем нет, так как в нём нет обращения к полям напрямую.</li>
<li>Из этого метода нельзя вызывать неконстантные методы, так что <code>FindAllDocuments</code> тоже придётся поправить.</li>
<li>Константный метод <code>FindTopDocuments</code> теперь можно вызвать для константного объекта.</li>
</ol>
</details> 

<details>  
<summary>Задание:</summary>  
<p>Сделайте константной переменную, в которую сохраняется результат вызова <code>CreateSearchServer</code>. Чтобы код продолжал компилироваться, пометьте константными все методы, не меняющие поля.</p>

<h3>Подсказка</h3>

<p>Если видите ошибку <code>passing 'const SearchServer' as 'this' argument discards qualifiers</code>, значит, вы вызвали неконстантный ****метод там, где это запрещено: в константном методе или для константного объекта.</p>

<p>Если ошибка другая — <code>discards qualifiers</code> для иного объекта или <code>no matching function for call</code> —  скорее всего, вы перестарались и пометили константным метод, который не должен им быть.</p>

</details>

