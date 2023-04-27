<details>  
<summary>Теория</summary>  
<h1>Параллельные алгоритмы</h1>

<p>Алгоритмы из стандартной библиотеки С++ упрощают работу программиста и повышают эффективность кода. Но есть способ ещё увеличить эффективность — делать несколько дел одновременно, а потом объединить результат. В С++ для этого подключают библиотеку <code>&lt;execution&gt;</code> и выполняют алгоритмы параллельно.</p>

<p>У алгоритма <code>accumulate</code> из библиотеки <code>numeric</code> есть близнец — алгоритм <code>reduce</code>, который поддерживает параллельное выполнение. Чтобы использовать <code>reduce</code>, передайте ему первым параметром объект <code>execution::par</code> — политику исполнения, объявленную в <code>&lt;execution&gt;</code>. Это запустит параллельную работу, и код станет быстрее.</p>

<p>Сравните время суммирования вектора чисел для алгоритмов <code>accumulate</code> и <code>reduce</code>. Для этого надо подключить библиотеку <code>&lt;chrono&gt;</code>:</p>

<pre><code  class="language-cpp">#include &lt;iostream&gt;
#include &lt;chrono&gt; //эта библиотека отвечает за концепты, связанные со временем
#include &lt;vector&gt;
#include &lt;numeric&gt;
#include &lt;execution&gt;

using namespace std;
int main()
{
	std::vector&lt;int&gt; v(200'000'001, 1);
	const auto startTime = chrono::high_resolution_clock::now();
	int result = std::accumulate(v.begin(), v.end(), 0);
	const auto endTime = chrono::high_resolution_clock::now();
	cout &lt;&lt;  &quot;std::accumulate execution time: &quot;s &lt;&lt; chrono::duration_cast&lt;chrono::duration&lt;int, milli&gt;&gt;(endTime - startTime).count() &lt;&lt;  &quot; ms.&quot;s &lt;&lt; endl;

	const auto startTimeReduce = chrono::high_resolution_clock::now();
	int resultReduce = std::reduce(execution::par, v.begin(), v.end(), 0);
	const auto endTimeReduce = chrono::high_resolution_clock::now();

	cout &lt;&lt;  &quot;std::reduce execution time: &quot;s &lt;&lt; chrono::duration_cast&lt;chrono::duration&lt;int, milli&gt;&gt;(endTimeReduce - startTimeReduce).count() &lt;&lt;  &quot; ms.&quot;s &lt;&lt; endl;
}
</code></pre>

  

<p><code>reduce</code> работает быстрее:</p>

  

<pre><code  class="language-cpp">std::accumulate execution time: 1646 ms.
std::reduce execution time: 708 ms.
</code></pre>

  

<p>Но у <code>reduce</code> есть ограничения. Он подходит только для коммутативных и ассоциативных операций — то есть для тех, которые не меняют результат при перестановке аргументов. Пример такой операции — сложение чисел.</p>

  

<p>Параллельной бывает и сортировка. Чтобы её запустить, функции <code>sort</code> первым параметром передают политику исполнения — <code>execution::par</code>. Сравним обычную и параллельную версию сортировки из стандартной библиотеки:</p>

  

<pre><code  class="language-cpp">#include &lt;iostream&gt;
#include &lt;algorithm&gt;
#include &lt;chrono&gt;
#include &lt;random&gt;
#include &lt;ratio&gt;
#include &lt;vector&gt;
#include &lt;execution&gt;

using namespace std;

const size_t testSize = 1'000'000;
const int iterationCount = 5;

int main() {
random_device rd;

	cout &lt;&lt;  &quot;Testing with &quot;s &lt;&lt; testSize &lt;&lt;  &quot; integers...&quot;s &lt;&lt; endl;
	vector&lt;int&gt; integers(testSize);
	for (auto&amp; i : integers) {
		i = static_cast&lt;int&gt;(rd());
	}

	for (int i = 0; i &lt; iterationCount; ++i)
	{
		vector&lt;int&gt; sorted(integers);
		const auto startTime = chrono::high_resolution_clock::now();
		sort(sorted.begin(), sorted.end());
		const auto endTime = chrono::high_resolution_clock::now();
		cout &lt;&lt;  &quot;Sequential execution time: &quot;s &lt;&lt; chrono::duration_cast&lt;chrono::duration&lt;int, milli&gt;&gt;(endTime - startTime).count() &lt;&lt;  &quot; ms.&quot;s &lt;&lt; endl;
	}

	  for (int i = 0; i &lt; iterationCount; ++i)
	{
		vector&lt;int&gt; sorted(integers);
		const auto startTime = chrono::high_resolution_clock::now();
		sort(execution::par, sorted.begin(), sorted.end());
		const auto endTime = chrono::high_resolution_clock::now();
		cout &lt;&lt;  &quot;Parallel execution time: &quot;s &lt;&lt; chrono::duration_cast&lt;chrono::duration&lt;int, milli&gt;&gt;(endTime - startTime).count() &lt;&lt;  &quot; ms.&quot;s &lt;&lt; endl;
	}
}

</code></pre>

  

<p>Получится:</p>

  

<pre><code  class="language-cpp">Testing with 1000000 integers...
Sequential execution time: 358 ms.
Sequential execution time: 387 ms.
Sequential execution time: 359 ms.
Sequential execution time: 360 ms.
Sequential execution time: 360 ms.
Parallel execution time: 80 ms.
Parallel execution time: 81 ms.
Parallel execution time: 89 ms.
Parallel execution time: 112 ms.
Parallel execution time: 125 ms.

</code></pre>

  

<p>Параллельная сортировка, как и другие параллельные алгоритмы, экономят время разработчика и пользователя.</p>
</details>  
  
<details>  
<summary>Задание:</summary>  

<p>Код поисковой системы можно ускорить. Примените политику параллельного исполнения. Новая для вас библиотека <code>utility</code> содержит реализацию пары.</p>

<h3>Подсказка</h3>
<p>Используйте <code>execution::par</code> там, где есть сортировка.</p>
</details>

