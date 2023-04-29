<details>  
<summary>Теория</summary>  
<h1>Алгоритм accumulate</h1>

<p>Стандартные функции <code>count</code>, <code>count_if</code>, <code>sort</code> и <code>reverse</code> входят в библиотеку <code>&lt;algorithm&gt;</code>. Но это не единственная библиотека алгоритмов в С++. Для числовых операций применяют алгоритмы из библиотеки <code>&lt;numeric&gt;</code>.</p>

<p>Алгоритм <code>accumulate</code> складывает все элементы вектора:</p>

<pre><code  class="language-cpp">int sum = accumulate(xs.begin(), xs.end(), 0);

</code></pre>

<p><code>accumulate</code> выполняет операции слева направо. Это важно, когда ему передаётся некоммутативная операция — та, которая зависит от порядка операндов. Например, сложение чисел коммутативно: <code>2 + 3 == 3 + 2</code>. А сложение строк — некоммутативно: <code>&quot;happy&quot; + &quot;bee&quot; == &quot;happybee&quot;</code>, <code>&quot;bee&quot; + &quot;happy&quot; == &quot;beehappy&quot;</code>.</p>

<p>По умолчанию <code>accumulate</code> суммирует все элементы вектора. Если нужна сумма элементов на конкретном участке, нужно указать начальную и конечную точки и значение, с которого начинается счёт. Обычно это ноль. Но иногда подсчёт идёт от результата предыдущих вычислений:</p>

<pre><code  class="language-cpp">#include &lt;iostream&gt;

#include &lt;numeric&gt;

#include &lt;vector&gt;

  

using namespace std;

  

int main() {

int size;

cin &gt;&gt; size; samples[

  

vector&lt;int&gt; values;

for (int i = 0; i &lt; size; ++i) {

int value;

cin &gt;&gt; value;

values.push_back(value);

}

  

int sum = accumulate(values.begin(), values.end(), 0);

cout &lt;&lt; sum &lt;&lt; endl;

}

</code></pre>

  

<p>Сила <code>accumulate</code> в возможности задать свою функцию для свёртки. Например, вы хотите накопить не только прожитые годы, но и количество интересных событий — это второй элемент пары. Тогда четвёртым аргументом нужно передать <code>accumulate</code> свою функцию, которая объяснит, как именно сворачивать значения из контейнера:</p>

<pre><code  class="language-cpp">// эта функция показывает, как accumulate будет сворачивать пары

// в нашем случае - просто сложит поэлементно:

// в левой паре - все предыдущие накопленные значения

// в правой паре - новое значение, которое аккумулируется в общую сумму

pair&lt;int, int&gt; PairFold(pair&lt;int, int&gt; accumulated, pair&lt;int, int&gt; additional) {

return {accumulated.first + additional.first, accumulated.second + additional.second};

}

  

// теперь мы храним пары значений - {число лет, число событий}

vector&lt;pair&lt;int, int&gt;&gt; values;

// ...

// вызов accumulate для получения результата

pair&lt;int, int&gt; sum = accumulate(values.begin(), values.end(), pair{0, 0}, PairFold);

</code></pre>
</details>  
  
<details>  
<summary>Задание 1:</summary>  
    
<p>Вычислите среднее арифметическое вектора чисел. Считайте, что среднее арифметическое пустого массива равно нулю.</p>

<h3>Подсказка</h3>

<p>Делить на ноль нельзя. Результатом должно быть вещественное число без округления. Для этого числа надо записывать с точкой: 2.0, 0.0. Тогда результат выражения будет вещественным числом.</p>
</details>  
  
<details>  
<summary>Задание 2:</summary>  
  <p>Напишите функцию, которая принимает строку-запрос и множество стоп-слов и возвращает новую строку. В новой строке после запроса идут стоп-слова через пробел, перед каждым стоит дефис. Передайте в <code>accumulate</code> четвёртым аргументом функцию, сворачивающую множество стоп-слов в «хвост» нужного формата.</p>

<h3>Пример</h3>

<table>

<thead>

<tr>

<th  align="center">Запрос</th>

<th  align="center">Стоп-слова</th>

<th  align="center">Выходная строка</th>

</tr>

</thead>

  

<tbody>

<tr>

<td  align="center">&ldquo;some tasty oranges&rdquo;</td>

<td  align="center">{&ldquo;of&rdquo;, &ldquo;in&rdquo;}</td>

<td  align="center">&ldquo;some tasty oranges -in -of&rdquo;</td>

</tr>

</tbody>

</table>

<p>Так как порядок ввода стоп-слов нам не важен, и мы планируем использовать контейнер <code>set</code>, порядок стоп-слов в выходной строке будет алфавитным, вне зависимости от того, в каком порядке стоп-слова оказались в этом контейнере.</p>

<h3>Подсказка</h3>

<p>Функция-помощница должна склеивать две строки через <code>&quot; -&quot;</code>.</p>
</details>
