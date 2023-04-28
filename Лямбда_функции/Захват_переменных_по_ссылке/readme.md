<details>  
<summary>Теория:</summary>  
<h1>Захват переменных по ссылке</h1>

<p>Чтобы обратиться к локальной переменной внутри лямбда-функции, переменную нужно явно захватить, указав её имя внутри квадратных скобок:</p>

<pre><code class="language-cpp">int CountMinusWords(const vector&lt;string&gt;&amp; words, char minus_prefix) {
    return count_if(
        words.begin(), words.end(),
        [minus_prefix](const string&amp; w) {
            return !w.empty() &amp;&amp; w[0] == minus_prefix;
        }
    );
}
</code></pre>

<p>С этим требованием вы познакомились в прошлом уроке. Пора узнать, зачем оно нужно. Компилятор не может сам понять, какие переменные используются, а какие нет. Дело в том, что все локальные переменные, захваченные лямбда-функцией, копируются внутрь неё. При этом происходит глубокое копирование. Как вы уже знаете, оно занимает много времени.</p>

<p>Допустим, у вас есть поисковый запрос и множество стоп-слов. Вы хотите посчитать, сколько слов запроса не будут стоп-словами. Подойдёт алгоритм <code>count_if</code>:</p>

<pre><code class="language-cpp">int CountQueryWords(const vector&lt;string&gt;&amp; query, const set&lt;string&gt;&amp; minus_words) {
    return count_if(
        query.begin(), query.end(),
        [minus_words](const string&amp; w) { // Множество minus_words копируется внутрь лямбда-функции
            return minus_words.count(w) == 0;
        }
    );
}
</code></pre>

<p>При вызове алгоритма <code>count_if</code> множество <code>minus_words</code> копируется внутрь передаваемой в него лямбды. <code>set</code> внутри устроено сложно. Если в нём большое количество элементов, а функция <code>CountQueryWords</code> вызывается часто, копирование замедлит программу.</p>

<p>Более того, в копировании нет смысла: вы принимаете множество <code>minus_words</code> по константной ссылке и только читаете его. Копия <code>minus_words</code> для этого не нужна — прочитать можно из оригинала. Такая ситуация нарушает основной принцип С++ — «не платите за то, что не используете».</p>

<p>Написать вызов алгоритма <code>count_if</code> нужно так, чтобы обратиться ко множеству <code>minus_words</code> изнутри лямбда-функции, но избежать глубокого копирования.</p>

<p>Переменную <code>minus_words</code> можно захватить по ссылке. Для этого перед именем переменной в квадратных скобках добавляют символ <code>&amp;</code>:</p>

<pre><code class="language-cpp">int CountQueryWords(const vector&lt;string&gt;&amp; query, const set&lt;string&gt;&amp; minus_words) {
    return count_if(
        query.begin(), query.end(),
        [&amp;minus_words](const string&amp; w) { // Обратите внимание на символ &amp; перед minus_words
            return minus_words.count(w) == 0;
        }
    );
}
</code></pre>

<p>Символ <code>&amp;</code> перед именем переменной говорит компилятору, чтобы вместо глубокого копирования переменной <code>minus_words</code> внутрь лямбды он сохранил только ссылку на неё.</p>

<p>Если лямбда-функция захватывает несколько переменных, способ захвата нужно прописать для каждой переменной отдельно:</p>

<pre><code class="language-cpp">int CountShortQueryWords(const vector&lt;string&gt;&amp; query,
    const set&lt;string&gt;&amp; minus_words, int max_length)
{
    return count_if(query.begin(), query.end(),
        [max_length, &amp;minus_words](const string&amp; w) {
            return w.size() &lt;= max_length &amp;&amp; minus_words.count(w) == 0;
        });
}
</code></pre>

<p>Захват переменной по ссылке сокращает глубокое копирование и время работы программы.</p>

</details> 

<details>  
<summary>Задание:</summary>  
<p>В приют привозят собак разных пород. В словаре <code>shelter</code> по названию хранится количество особей каждого вида на текущий момент. Приют может принять определённое количество собак каждой породы. Оно указано в словаре <code>max_amount</code>. Напишите функцию, которая будет на основании вектора с именами поступивших собак обновлять словарь <code>shelter</code> и возвращать общее количество попавших в приют собак. Гарантируется, что никакие другие собаки, кроме известных из словаря <code>max_amount</code>, в приют не приедут.</p>

<h3>Пример входных данных для функции</h3>

<pre><code class="language-cpp">map&lt;string, int&gt; shelter {
    {&quot;landseer&quot;s, 1},
    {&quot;otterhound&quot;s, 2},
    {&quot;pekingese&quot;s, 2},
    {&quot;pointer&quot;s, 3}
};
const map&lt;string, int&gt; max_amount {
    {&quot;landseer&quot;s, 2},
    {&quot;otterhound&quot;s, 3},
    {&quot;pekingese&quot;s, 4},
    {&quot;pointer&quot;s, 7}
};
const vector&lt;string&gt; new_dogs {
    &quot;landseer&quot;s,
    &quot;otterhound&quot;s,
    &quot;otterhound&quot;s,
    &quot;otterhound&quot;s,
    &quot;pointer&quot;s
};
</code></pre>

<h3>Пример вывода функции</h3>

<pre><code class="language-jsx">3
</code></pre>

<h3>Подсказка</h3>

<p>Примените <code>count_if</code> и захват по ссылке в лямбда-выражении.</p>
</details> 
