<details>  
<summary>Теория:</summary>  
<h1>Захват переменных по значению</h1>

<p>У лямбда-функций есть другие случаи применения. Допустим, у вас есть вектор целых чисел, и вы хотите узнать, сколько в нём чисел больше пяти. Это можно сделать простым циклом:</p>

<pre><code class="language-cpp">int CountMoreThanFive(const vector&lt;int&gt;&amp; v) {
    int result = 0;
    for (int value : v) {
        if (value &gt; 5) {
            ++result;
        }
    }
    return result;
}

int main() {
    const vector&lt;int&gt; v = {1, 3, 5, 2, 6, 7, 10, 2, 3};
    cout &lt;&lt; CountMoreThanFive(v) &lt;&lt; endl;
}
</code></pre>

<p>Многовато кода для такой простой задачи. Проще использовать алгоритм <code>count_if</code> и простую лямбда-функцию. Тогда даже отдельная функция <code>CountMoreThanFive</code> не понадобится:</p>

<pre><code class="language-cpp">const vector&lt;int&gt; v = {1, 3, 5, 2, 6, 7, 10, 2, 3};
cout &lt;&lt; count_if(begin(v), end(v), [](int x) { return x &gt; 5; }) &lt;&lt; endl;
</code></pre>

<p>И код лаконичнее, и работы меньше.</p>

<p>Теперь вы решили посчитать количество чисел со значением, которое не больше пяти, а больше некоторого числа из стандартного ввода. Для этого вы заменили 5 на <code>threshold</code>:</p>

<pre><code class="language-cpp">int main() {
    int threshold;
    cin &gt;&gt; threshold;

    const vector&lt;int&gt; v = {1, 3, 5, 2, 6, 7, 10, 2, 3};
    cout &lt;&lt; count_if(begin(v), end(v), [](int x) { return x &gt; threshold; }) &lt;&lt; endl;
		//                                                        ^ заменили 5 на threshold 
}
</code></pre>

<p>Такой код не скомпилируется:</p>

<pre><code>..\src\hw.cpp: In lambda function:
..\src\hw.cpp:26:63: error: 'threshold' is not captured
     cout &lt;&lt; count_if(begin(v), end(v), [](int x) { return x &gt; threshold; }) &lt;&lt; endl;
                                                               ^~~~~~~~~
..\src\hw.cpp:26:41: note: the lambda has no capture-default
     cout &lt;&lt; count_if(begin(v), end(v), [](int x) { return x &gt; threshold; }) &lt;&lt; endl;
                                         ^
..\src\hw.cpp:22:9: note: 'int threshold' declared here
     int threshold;
         ^~~~~~~~~
</code></pre>

<p>Компилятор говорит: “&rsquo;threshold&rsquo; is not captured”. Дело в том, что С++ спроектирован по принципу «не платите за то, чем не пользуетесь». Это касается и лямбда-функций. В общем случае внешние по отношению к лямбда-функции объекты могут требовать дополнительного времени и памяти. Если программист сделает опечатку в коде лямбда-функции и случайно обратится к внешней переменной, такие расходы возникнут внезапно. Поэтому, чтобы использовать внешнюю переменную внутри лямбда-функции, её надо явно захватить — указать имя переменной в квадратных скобках перед параметрами лямбда-функции:</p>

<pre><code class="language-cpp">int main() {
    int threshold;
    cin &gt;&gt; threshold;

    const vector&lt;int&gt; v = {1, 3, 5, 2, 6, 7, 10, 2, 3};
    cout &lt;&lt; count_if(begin(v), end(v), [threshold](int x) { return x &gt; threshold; }) &lt;&lt; endl;
//                                      ^ вот здесь теперь указана переменная threshold
}
</code></pre>

<p>Теперь код компилируется и корректно работает. Явный захват внешних переменных в лямбда-функции выражает ваше намерение их применить и «заплатить только за то, что будете использовать». Это безопасный и предсказуемый подход.</p>

<p>Из переменной, захваченной таким образом, можно только читать:</p>

<pre><code class="language-cpp">int main() {
    int x = 0;
    auto lambda = [x]() { ++x; };
    lambda(); // да, лямбда-функции можно присваивать переменным, а потом вызывать как обычные функции
}
</code></pre>

<p>Код выше не компилируется:</p>

<pre><code>..\src\hw.cpp: In lambda function:
..\src\hw.cpp:23:29: error: increment of read-only variable 'x'
     auto lambda = [x]() { ++x; };
                             ^
</code></pre>

<p>В C++ есть способ менять захваченные переменные внутри лямбда-функции. Его изучим позже.</p>

</details> 

<summary>Задание:</summary>  
<details>  
<p>Поисковые системы логируют запросы, чтобы потом исследовать их и улучшать качество поиска. Смоделируйте такое исследование и напишите программу, которая изучает популярность определённой темы у пользователей. На вход программа получает список поисковых запросов и слово. Программа должна вывести количество поисковых запросов, содержащих данное слово.</p>

<h3>Пример ввода</h3>

<pre><code class="language-cpp">3
apple juice 
bionic beaver
funny little beaver
beaver
</code></pre>

<h3>Пример вывода</h3>

<pre><code class="language-cpp">2
</code></pre>

<h3>Подсказка</h3>

<p>Примените <code>find</code> класса <code>string</code>, чтобы искать, входит ли одна строка в другую. Этот метод возвращает позицию, на которой искомая строка встречается впервые, либо специальное значение <code>string::npos</code>, если вхождение не найдено.</p>
</details> 


