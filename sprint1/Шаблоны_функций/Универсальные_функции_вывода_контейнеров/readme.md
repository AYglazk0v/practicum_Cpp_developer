<details>  
<summary>Теория:</summary>

<h1>Универсальные функции вывода контейнеров</h1>

<p>Возможно, вы успели огорчиться, что в C++ нельзя простым движением и оператором <code>&lt;&lt;</code> вывести содержимое контейнера:</p>

<pre><code class="language-cpp">const vector&lt;string&gt; cats = {&quot;Мурка&quot;s, &quot;Белка&quot;s, &quot;Георгий&quot;s, &quot;Рюрик&quot;s};
cout &lt;&lt; cats &lt;&lt; endl;
</code></pre>

<p>Не беда. C++ легко обучить. Чтобы шалость удалась, переопределим оператор вывода в поток для вектора:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;vector&gt;

using namespace std;

ostream&amp; operator&lt;&lt;(ostream&amp; out, const vector&lt;string&gt;&amp; container) {
    for (const string&amp; element : container) {
        out &lt;&lt; element &lt;&lt; &quot; &quot;s;
    }
    return out;
}  
   
int main() {
    const vector&lt;string&gt; cats = {&quot;Мурка&quot;s, &quot;Белка&quot;s, &quot;Георгий&quot;s, &quot;Рюрик&quot;s};
    cout &lt;&lt; cats &lt;&lt; endl;
}
</code></pre>

<p>Этот код ожидаемо выводит <code>Мурка Белка Георгий Рюрик</code> . Разберёмся, как мы этого добились.</p>

<p>Мы написали функцию со специальным названием <code>operator&lt;&lt;</code>. У неё два аргумента: ссылка на выходной поток (<code>o</code> в <code>ostream</code> означает <code>output</code>) и константная ссылка на вектор. Своим существованием она говорит компилятору: «Если кто-то напишет <code>out &lt;&lt; container</code>, вызови меня: <code>operator&lt;&lt;(out, container)</code>». Вернуть ссылку на поток нужно, чтобы объединять вывод в цепочки, как в выражении <code>cout &lt;&lt; cats &lt;&lt; endl</code>.</p>

<p>Позже вы изучите переопределение и для других операторов, с другими целями. А пока начнём постепенно улучшать этот код.</p>

</details>  

<details>  
<summary>Задание 1:</summary>
<p>Доработайте оператор вывода вектора в поток, чтобы он работал с любым вектором — при условии, что сами его элементы можно выводить оператором  <code>&lt;&lt;</code>. Так, для вектора целых чисел код из тренажёра должен выводить <code>10 5 2 12</code> .</p>

<h3>Подсказка</h3>

<p>Сделайте функцию шаблонной: замените тип <code>string</code> на шаблонный параметр <code>Element</code>.</p>

</details>  

<details>  
<summary>Задание 2:</summary>
<p>Вы могли заметить лишний пробел, который выводится после последнего элемента. Избавьтесь от него и замените пробел на запятую с пробелом. Код из предыдущей задачи теперь должен выводить <code>10, 5, 2, 12</code>.</p>

<h3>Подсказка</h3>

<p>Выводите <code>&quot;, &quot;s</code> перед каждым элементом кроме первого. Понять, что элемент первый, поможет <code>bool</code>-переменная.</p>
</details>  

<details>  
<summary>Задание 3:</summary>
<p>Научите функцию вывода работать не только с векторами, но и со множествами.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">const set&lt;string&gt; cats = {&quot;Мурка&quot;s, &quot;Белка&quot;s, &quot;Георгий&quot;s, &quot;Рюрик&quot;s};
cout &lt;&lt; cats &lt;&lt; endl;
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>Белка, Георгий, Мурка, Рюрик
</code></pre>

<h3>Подсказка</h3>

<p>Шаблонизировать функцию типом контейнера вместо типа элемента не получится. Теперь даже при выводе простого числа компилятор не знает, вызывать ли ему стандартную функцию вывода числа или сделать вид, что число — это контейнер. Поэтому для начала ограничьтесь двумя версиями функции: для вектора и для множества.</p>
</details>  

<details>  
<summary>Задание 4:</summary>
<p>Избавьтесь от дублирования кода. Вынесите общую функциональность вывода в шаблонную функцию <code>Print</code>, принимающую ссылку на поток вывода и объект шаблонного типа, элементы которого нужно вывести. Вызовите её из операторов вывода вектора и множества.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">const set&lt;string&gt; cats = {&quot;Мурка&quot;s, &quot;Белка&quot;s, &quot;Георгий&quot;s, &quot;Рюрик&quot;s};
cout &lt;&lt; cats &lt;&lt; endl;

const vector&lt;int&gt; ages = {10, 5, 2, 12};
cout &lt;&lt; ages &lt;&lt; endl;
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>Белка, Георгий, Мурка, Рюрик
10, 5, 2, 12
</code></pre>

<h3>Подсказка</h3>

<p>Не забудьте назвать вспомогательную функцию <code>Print</code>, первым её аргументом сделать ссылку на поток, а вторым — контейнер шаблонного типа. Точно так же вы делали с набором документов <code>Documents</code>.</p>
</details>  

<details>  
<summary>Задание 5:</summary>
<p>Сделайте вывод более нарядным: выводите содержимое вектора в квадратных скобках, а множества — в фигурных. Функция <code>Print</code> должна остаться прежней и не обрамлять вывод в скобки.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">const set&lt;string&gt; cats = {&quot;Мурка&quot;s, &quot;Белка&quot;s, &quot;Георгий&quot;s, &quot;Рюрик&quot;s};
cout &lt;&lt; cats &lt;&lt; endl;

const vector&lt;int&gt; ages = {10, 5, 2, 12};
cout &lt;&lt; ages &lt;&lt; endl;
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>{Белка, Георгий, Мурка, Рюрик}
[10, 5, 2, 12]
</code></pre>

<h3>Подсказка</h3>

<p>Не правьте функцию <code>Print</code>: достаточно добавить вывод скобок в каждую версию перегруженного <code>operator&lt;&lt;</code>. Не забудьте, что выводить нужно в <code>out</code>, а не в <code>cout</code>.</p>
</details>  

<details>  
<summary>Задание 6:</summary>
<p>Научитесь выводить словари. Содержимое словаря должно быть заключено в угловые скобки, а каждая пара ключ-значение — в круглые.</p>

<p>Пусть функция <code>Print</code> выводит словарь без угловых скобок. Ни для каких других типов, кроме векторов, множеств и словарей, вызывать <code>Print</code> не нужно.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">const set&lt;string&gt; cats = {&quot;Мурка&quot;s, &quot;Белка&quot;s, &quot;Георгий&quot;s, &quot;Рюрик&quot;s};
cout &lt;&lt; cats &lt;&lt; endl;

const vector&lt;int&gt; ages = {10, 5, 2, 12};
cout &lt;&lt; ages &lt;&lt; endl;

const map&lt;string, int&gt; cat_ages = {
    {&quot;Мурка&quot;s, 10}, 
    {&quot;Белка&quot;s, 5},
    {&quot;Георгий&quot;s, 2}, 
    {&quot;Рюрик&quot;s, 12}
};
cout &lt;&lt; cat_ages &lt;&lt; endl;
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>{Белка, Георгий, Мурка, Рюрик}
[10, 5, 2, 12]
&lt;(Белка, 5), (Георгий, 2), (Мурка, 10), (Рюрик, 12)&gt;
</code></pre>

<h3>Подсказка</h3>

<ol>
<li>Напишите ещё одну версию <code>operator&lt;&lt;</code> с двумя шаблонными параметрами — для <code>map&lt;Key, Value&gt;</code>.</li>
<li>Функция <code>Print</code>, итерируясь по словарю, будет пытаться вывести пару — напишите и для неё <code>operator&lt;&lt;</code>.</li>
<li>При вызове одной функции из другой компилятор «видит» только те из них, что написаны выше. Поэтому выше всех должен быть оператор вывода пары, а сразу за ним — универсальная <code>Print</code>.</li>
</ol>
</details>  
