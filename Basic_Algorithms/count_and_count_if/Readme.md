<details>  
<summary>Теория</summary>  
  <h1>Алгоритмы count и count_if</h1>

<p>Чтобы посчитать количество определённых элементов в контейнере или отсортировать их, подойдут циклы. Но в С++ есть более удобный способ — библиотека <code>&lt;algorithm&gt;</code>.</p>

<p>В неё входят <strong>стандартные алгоритмы</strong> — функции, выполняющие распространённые операции с контейнерами. Преимущества стандартных алгоритмов:</p>

<ul>

<li>код с ними компактнее, чем с циклами;</li>

<li>меньше ошибок — алгоритмы не раз протестированы и точно работают.</li>

</ul>

<p>Этот урок посвящён стандартным алгоритмам для счёта элементов. Чтобы посчитать, сколько раз конкретный элемент встречается в контейнере, применяют алгоритм <code>count</code>. Так можно узнать количество единиц в векторе чисел:</p>

<pre><code  class="language-cpp">const vector&lt;int&gt; xs = {1, 2, 1, 1, 5};

cout &lt;&lt; count(xs.begin(), xs.end(), 1) &lt;&lt; endl; // 3

</code></pre>

<p>Функции <code>begin</code> и <code>end</code> в коде означают, что операция проводится над всем содержимым вектора.</p>

<p>Алгоритм <code>count</code> работает с векторами любых типов. Например, у вас есть список пропавших за день животных. Применив <code>count</code>, можно узнать, сколько попугаев захотели свободы:</p>

<pre><code  class="language-cpp">const vector&lt;string&gt; escapees = {

&quot;cat&quot;s, &quot;dog&quot;s, &quot;parrot&quot;s, &quot;cat&quot;s, &quot;rat&quot;s, &quot;parrot&quot;s, &quot;cat&quot;s, &quot;dog&quot;s

};

cout &lt;&lt; count(escapees.begin(), escapees.end(), &quot;parrot&quot;s); // 2 попугая

</code></pre>

<p>У функции <code>count</code> есть сестра — <code>count_if</code>. Она принимает третьим аргументом не значение, а ссылку на функцию. И считает значения в контейнере, для которых эта функция возвращает <code>true</code>:</p>

<pre><code  class="language-cpp">bool IsPositive(int x) {

return x &gt; 0;

}

int main() {

// ...

int num_positive = count_if(xs.begin(), xs.end(), IsPositive);

}

</code></pre>

<p>Алгоритмы <code>count</code> и <code>count_if</code> — наиболее удобный и компактный способ посчитать конкретные элементы в контейнере.</p>
  
</details>  
  
<details>  
<summary>Задание 1:</summary>  
<p>Ранее вы реализовали функцию <code>PrintWorryCount</code>. Перепишите её, используя <code>count</code>.  
</p>
</details>  
  
<details>  
<summary>Задание 2:</summary>  
  <p>Напишите функцию, которая принимает вектор строк и возвращает число строк, начинающихся на большую или маленькую а  
нглийскую букву &ldquo;a&rdquo;.  
</p>
</details>
