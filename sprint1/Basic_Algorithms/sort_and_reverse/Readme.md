<details>
<summary>Теория</summary>

<h1>Алгоритмы sort и reverse</h1>

<p>В этом уроке вы познакомитесь со стандартными алгоритмами сортировки из библиотеки <code>&lt;algorithm&gt;</code>.</p>

<h3>Алгоритм sort</h3>

<p><code>sort</code> сортирует указанный контейнер или обычный массив по неубыванию:</p>

<pre><code  class="language-cpp">sort(xs.begin(), xs.end()); // теперь элементы xs будут в возрастающем порядке

</code></pre>

<p>Этот алгоритм умеет сравнивать любые типы данных — строки, числа, пары строк и чисел:</p>

<pre><code  class="language-cpp">#include &lt;iostream&gt;

#include &lt;vector&gt;

#include &lt;string&gt;

#include &lt;algorithm&gt;

using namespace std;

int main() {

vector&lt;int&gt; numbers = {1, 5, 12, 3, 7};

vector&lt;string&gt; continents = {&quot;Eurasia&quot;s, &quot;Antarctica&quot;s, &quot;North America&quot;s, &quot;Australia&quot;s};

vector&lt;pair&lt;int, string&gt;&gt; numbers_with_names = {{2, &quot;Two&quot;s}, {1, &quot;First&quot;s}, {1, &quot;One&quot;s}, {3, &quot;Three&quot;s}};


sort(numbers.begin(), numbers.end());

sort(continents.begin(), continents.end());

sort(numbers_with_names.begin(), numbers_with_names.end());

for (const auto&amp; element: numbers) {

cout &lt;&lt; element &lt;&lt; endl;

}

cout &lt;&lt; endl;

for (const auto&amp; element: continents) {

cout &lt;&lt; element &lt;&lt; endl;

}

cout &lt;&lt; endl;

for (const auto&amp; element: numbers_with_names) {

cout &lt;&lt; element.first &lt;&lt;  &quot;  &quot;  &lt;&lt; element.second &lt;&lt; endl;

}

}

</code></pre>

<p>Числа сортируются по возрастанию, строки — по алфавиту, пары — сначала по первому элементу, а затем по второму:</p>

<pre><code  class="language-cpp">1

3

5

7

12

Antarctica

Australia

Eurasia

North America

1 First

1 One

2 Two

3 Three

</code></pre>

<h3>Алгоритм reverse</h3>

<p>Чтобы отсортировать элементы в убывающем порядке, используют <code>sort</code>, а потом разворачивают вектор алгоритмом <code>reverse</code>:</p>

<pre><code  class="language-cpp">reverse(xs.begin(), xs.end());

</code></pre>

<p>Например, если написать <code>reverse(numbers_with_names.begin(), numbers_with_names.end());</code> перед выводом ответа в предыдущем примере, вектор пар окажется перевёрнутым:</p>

<pre><code  class="language-cpp">3 Three

2 Two

1 One

1 First

</code></pre>

</details>

<details>
<summary>Задание 1:</summary>

<p>Считайте количество людей, имя и возраст каждого. Выведите все имена с новой строки по старшинству — от старых к молодым.</p>

<p>Вектор пар сортируется лексикографически. Например, у вас есть вектор:</p>  

<pre><code  class="language-cpp">vector&lt;pair&lt;int, double&gt;&gt; values = {{5, -1.1}, {5, -1.0}, {1, 0.0}};

</code></pre>

<p>После сортировки значения будут в таком порядке:</p>

<pre><code  class="language-cpp">{1, 0.0}

{5, -1.1} // 1 &lt; 5

{5, -1.0}, // 5 == 5, -1.1 &lt; -1.0

</code></pre>

<h3>Подсказка</h3>

<p>Подумайте, в каком порядке должны лежать поля в паре, чтобы получилась сортировка по возрасту.</p>

</details>

<details>
<summary>Задание 2:</summary>

<p>Время ответа сервера зависит от запроса. Поэтому нужна метрика, которая позволяет адекватно оценить время работы в типичном случае.</p>

<p>Первая идея — смотреть на среднее время. Но это может приводить к неадекватным ответам. Например, 10% запросов не выполняются, так как время истекает, а таймаут выставлен на две секунды. Если поднять таймаут до трёх секунд, среднее время увеличится, хотя по сути ничего не изменилось.</p>

<p>Больше подойдёт медиана и её аналоги — процентили. Например 99-й процентиль — это значение минимального элемента, который больше 99% всех элементов массива. Чтобы не сортировать весь массив, в реальной жизни такие статистики считают алгоритмом <a  href="https://en.cppreference.com/w/cpp/algorithm/nth_element">nth_element</a>. В задании примените медиану. Для отсортированного массива это значение среднего элемента, либо полусумма двух средних элементов, если число элементов чётное:</p>

<pre><code  class="language-cpp">CalcMedian({1, 2, 3, 4}) == 2.5;

CalcMedian({1, 2, 3}) == 2;

CalcMedian({1, 2, 1000}) == 2; // медиана устойчива к небольшому числу сильно отклоняющихся от нормы значений

</code></pre>

<p>Напишите функцию, которая принимает на вход вектор времени ответа и вычисляет медиану этого вектора. Вектор может быть неотсортированным.</p>

<p>Чтобы проверить число на чётность, используйте оператор остатка от деления:</p>  

<pre><code  class="language-cpp">if (x % 2 == 0) {

// чётное

} else {

// нечётное

}

</code></pre>

<h3>Подсказка</h3>

<p>Исходный массив необязательно будет отсортирован.</p>

</details>
