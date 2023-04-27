
<details>  
<summary>Теория_1</summary>  
<h1>Сортировка вектора структур</h1>

<p>Вы начали внедрять структуру <code>Document</code> в код поисковой системы. С функцией <code>FindAllDocuments</code> справились. Пора научить поисковую систему выводить сначала самые релевантные документы. Для этого нужно написать основную функцию — <code>FindTopDocuments</code>:</p>

<pre><code class="language-cpp">// For each document returns its id and relevance
vector&lt;pair&lt;int, int&gt;&gt; FindTopDocuments(
				const map&lt;string, set&lt;int&gt;&gt;&amp; word_to_documents,
				const set&lt;string&gt;&amp; stop_words,
				const string&amp; query) {
			
		auto matched_documents = FindAllDocuments(word_to_documents, stop_words, query);
		
		// сортировка по убыванию релевантности
	  // раньше тут были sort и reverse	
	
		if (matched_documents.size() &gt; MAX_RESULT_DOCUMENT_COUNT) {
				matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
	
		// исправление порядка полей в парах
	
		return matched_documents;
}
</code></pre>

<p>Теперь везде вместо пар структура, поэтому исправлять порядок полей в конце функции больше не придётся. Зато надо сортировать структуры в определённом порядке. Попробуйте просто взять и вызвать <code>sort</code> для вектора документов:</p>

<pre><code class="language-cpp">#include &lt;algorithm&gt;
#include &lt;vector&gt;

using namespace std;

struct Document {
    int id;
    int relevance;
};

int main() {
    vector&lt;Document&gt; documents;
    sort(documents.begin(), documents.end());
}
</code></pre>

<p>Сообщение об ошибке будет громоздким:</p>

<pre><code>In file included from &lt;...&gt;/include/c++/bits/stl_algobase.h:71,
                 from &lt;...&gt;/include/c++/algorithm:61,
                 from test.cpp:1:
&lt;...&gt;/include/c++/bits/predefined_ops.h: In instantiation of 'constexpr bool __gnu_cxx::__ops::_Iter_less_iter::operator()(_Iterator1, _Iterator2) const [with _Iterator1 = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;; _Iterator2 = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;]':
&lt;...&gt;/include/c++/bits/stl_algo.h:81:17:   required from 'void std::__move_median_to_first(_Iterator, _Iterator, _Iterator, _Iterator, _Compare) [with _Iterator = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
&lt;...&gt;/include/c++/bits/stl_algo.h:1921:34:   required from '_RandomAccessIterator std::__unguarded_partition_pivot(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
&lt;...&gt;/include/c++/bits/stl_algo.h:1953:38:   required from 'void std::__introsort_loop(_RandomAccessIterator, _RandomAccessIterator, _Size, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;; _Size = long long int; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
&lt;...&gt;/include/c++/bits/stl_algo.h:1968:25:   required from 'void std::__sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
&lt;...&gt;/include/c++/bits/stl_algo.h:4834:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;]'
test.cpp:13:44:   required from here
&lt;...&gt;/include/c++/bits/predefined_ops.h:43:23: error: no match for 'operator&lt;' (operand types are 'Document' and 'Document')
       { return *__it1 &lt; *__it2; }
                ~~~~~~~^~~~~~~~
In file included from &lt;...&gt;/include/c++/bits/stl_algobase.h:67,
&lt;...&gt;
</code></pre>

<p>Ошибка в 13-й строке test.cpp — вызов <code>sort</code>.</p>

<pre><code>&lt;...&gt;
&lt;...&gt;/include/c++/bits/stl_algo.h:4834:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator&lt;Document*, std::vector&lt;Document&gt; &gt;]'
test.cpp:13:44:   required from here
</code></pre>

<p>В сообщении говорится: вызвана функция <code>std::sort</code>, из неё — <code>std::__sort</code>, затем <code>std::__introsort_loop</code>. Писать <code>sort</code> вместо <code>std::sort</code> позволяет <code>using namespace std;</code>. То, как реализация сортировки разбита на подфункции, сейчас неважно. Основная проблема — в сообщении ниже:</p>

<pre><code>&lt;...&gt;/include/c++/bits/predefined_ops.h:43:23: error: no match for 'operator&lt;' (operand types are 'Document' and 'Document')
       { return *__it1 &lt; *__it2; }
                ~~~~~~~^~~~~~~~
</code></pre>

<p>Где-то в коде сортировки оператор <strong><code>&lt;</code></strong> сравнивает два документа со странными названиями <code>*__it1</code> и <code>*__it2</code>. На звёздочки пока не обращайте внимания. Важна фраза <code>no match for 'operator&lt;' (operand types are 'Document' and 'Document')</code>: не найден оператор <code>&lt;</code> для объектов <code>Document</code> и <code>Document</code>.</p>

<p>Для сортировки документов достаточно уточнить, в каком случае считается, что один из них меньше другого. После сортировки меньшие документы будут в начале, а бóльшие — в конце.</p>

<p>В коде ниже — функция-компаратор, которая принимает два документа по константной ссылке. Первый документ называется <code>lhs</code>, а второй <code>rhs</code>. Функция возвращает <code>true</code>, если <code>lhs</code> меньше <code>rhs</code>:</p>

<pre><code class="language-cpp">bool HasDocumentLessId(const Document&amp; lhs, const Document&amp; rhs) {
		return lhs.id &lt; rhs.id;
}
</code></pre>

<p>Название у функции говорящее: «правда ли id документа меньше». Теперь, чтобы  отсортировать документы по возрастанию <code>id</code>, нужно вызвать сортировку:</p>

<pre><code class="language-cpp">sort(documents.begin(), documents.end(), HasDocumentLessId);
</code></pre>

<p>Строго говоря, сортировка будет не по возрастанию, а по неубыванию. Некоторые документы могут иметь одинаковый <code>id</code>. После сортировки они будут расположены в произвольном порядке. Подумайте, важно ли это, когда сортируете только по одному из полей. Сортировка по нескольким полям будет рассмотрена позже.</p>

<p>Допустим, вы поменяли <code>&lt;</code> на <code>&gt;</code> в функции-компараторе: <code>return lhs.id &gt; rhs.id;</code>. Тогда произойдёт сортировка по убыванию — точнее, по невозрастанию. Меньшими будут считаться документы с бо́льшим id, поэтому они окажутся в начале вектора. Но смысл функции поменяется — значит, ей нужно другое название, например <code>HasDocumentGreaterId</code>.</p>

</details> 
<details>  
<summary>Задание 1:</summary>  
<p>В коде поисковой системы документы должны быть отсортированы по убыванию релевантности. Напишите функцию-компаратор, которую нужно передать в такую сортировку.</p>

<p>Функция должна называться <code>HasDocumentGreaterRelevance</code>.</p>

<h3>Подсказка</h3>

<p>Возьмите в качестве шаблона компаратор для сортировки по возрастанию id:</p>

<pre><code class="language-cpp">bool HasDocumentLessId(const Document&amp; lhs, const Document&amp; rhs) {
		return lhs.id &lt; rhs.id;
}
</code></pre>

<ol>
<li>Замените <code>id</code> на <code>relevance</code>. Не только в коде, но и в названии!</li>
<li>Сортируйте не по возрастанию, а по убыванию: замените <code>&lt;</code> на <code>&gt;</code> и отразите это в названии.</li>
</ol>
</details>

<details>  
<summary>Задание 2:</summary>  
<p>Внедрите структуру <code>Document</code> в код поисковой системы.</p>

<p><strong>Пример ввода</strong></p>

<pre><code>a the on cat
6
a fat cat sat on a mat and ate a fat rat
fat rat
a fat cat rat
a fat cat sat
a fat cat
a fat dog
funny fat cat on a mat with rat
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>{ document_id = 0, relevance = 3 }
{ document_id = 1, relevance = 2 }
{ document_id = 2, relevance = 2 }
{ document_id = 3, relevance = 1 }
{ document_id = 4, relevance = 1 }
</code></pre>

<p>Возможен и другой вывод, например:</p>

<pre><code>{ document_id = 0, relevance = 3 }
{ document_id = 2, relevance = 2 }
{ document_id = 1, relevance = 2 }
{ document_id = 5, relevance = 1 }
{ document_id = 4, relevance = 1 }
</code></pre>

<h3>Подсказка</h3>

<p>Замените все <code>pair&lt;int, int&gt;</code> на <code>Document</code>, а при сортировке вектора документов используйте компаратор из прошлой задачи:</p>

<pre><code class="language-cpp">bool HasDocumentGreaterRelevance(const Document&amp; lhs, const Document&amp; rhs) {
		return lhs.relevance &gt; rhs.relevance;
}
</code></pre>

<p>Вызов <code>reverse</code> больше не нужен!</p>
</details>

