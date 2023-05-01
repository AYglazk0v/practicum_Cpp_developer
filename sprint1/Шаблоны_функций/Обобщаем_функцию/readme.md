<details>  
<summary>Теория:</summary>

<h1>Обобщаем функцию</h1>

<p>Итак, функция <code>ComputeTermFreqs</code> умеет считать частоту строк по данному вектору. Функция короткая, но достойна существовать самостоятельно.</p>

<p>Вычислить частоту можно и для числовых данных. Подсчитаем количество двуногих и четвероногих обитателей квартиры:</p>

<pre><code class="language-cpp">int main() {
    const vector&lt;int&gt; leg_counts = {4, 2, 4, 4};
    const auto legs_stat = ComputeTermFreqs(leg_counts);
    cout &lt;&lt; &quot;Двуногих &quot;s &lt;&lt; legs_stat.at(2) &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;четвероногих &quot;s &lt;&lt; legs_stat.at(4) &lt;&lt; endl;
} 
</code></pre>

<p>И потерпим неудачу: наша функция умеет принимать только вектор строк:</p>

<pre><code>error: invalid initialization of reference of type ‘const std::vector&lt;std::__cxx11::basic_string&lt;char&gt; &gt;&amp;’ from expression of type ‘const std::vector&lt;int&gt;’
</code></pre>

<p>Но мы можем написать ещё одну функцию для типа <code>int</code>. И даже дать ей такое же название: компилятор сам поймёт по типам аргументов, какую функцию вызвать:</p>

<pre><code class="language-cpp">map&lt;string, int&gt; ComputeTermFreqs(const vector&lt;string&gt;&amp; terms) {
    map&lt;string, int&gt; term_freqs;
    for (const string&amp; term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

map&lt;int, int&gt; ComputeTermFreqs(const vector&lt;int&gt;&amp; terms) {
    map&lt;int, int&gt; term_freqs;
    for (int term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

int main() {
    const vector&lt;int&gt; leg_counts = {4, 2, 4, 4};
    const auto legs_stat = ComputeTermFreqs(leg_counts);
    cout &lt;&lt; &quot;Двуногих &quot;s &lt;&lt; legs_stat.at(2) &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;четвероногих &quot;s &lt;&lt; legs_stat.at(4) &lt;&lt; endl;
		// Двуногих 1, четвероногих 3
}
</code></pre>

<p>Это называется перегрузка функций. А ещё ужасный копипаст. Так делать не стоит.</p>

<p>Посмотрим, как надо. Версии функции <code>ComputeTermFreqs</code> отличаются в двух аспектах:</p>

<ol>
<li>Вместо <code>string</code> использован <code>int</code>.</li>
<li>Цикл <code>for</code> перебирает элементы по значению, а не по ссылке.</li>
</ol>

<p>От перебора по значению можно отказаться — функцию это серьёзно не замедлит. Отличие останется только в типе. Мы пока не знаем, с каким типом для слов функция будет работать, но уже можем её написать. Назовём тип <code>Term</code>:</p>

<pre><code class="language-cpp">// нам интересны Term = string и Term = int, пытаемся обобщить
map&lt;Term, int&gt; ComputeTermFreqs(const vector&lt;Term&gt;&amp; terms) {
    map&lt;Term, int&gt; term_freqs;
    for (const Term&amp; term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}
</code></pre>

<p>Функция зависит не только от конкретного вектора <code>terms</code>, но и от типа его элементов. Такая функция называется шаблонной и оформляется следующим образом:</p>

<pre><code class="language-cpp">template &lt;typename Term&gt;  // шаблонный параметр-тип с названием Term
map&lt;Term, int&gt; ComputeTermFreqs(const vector&lt;Term&gt;&amp; terms) {
    map&lt;Term, int&gt; term_freqs;
    for (const Term&amp; term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}
</code></pre>

<p>Вы увидели, как вызвать <code>ComputeTermFreqs</code> от вектора строк или вектора чисел и получить статистику частоты элементов. Без всякого копипаста.</p>


</details>  

<details>  
<summary>Задание :</summary>
<p>Примените шаблонную функцию <code>ComputeTermFreqs</code> и определите, какое животное встречается наибольшее число раз. Животное в этом задании задаётся парой <code>pair&lt;string, int&gt;</code>.</p>

<p>Если максимальное число раз встречаются несколько животных, выведите наименьшего из них. Гарантируется, что вектор содержит хотя бы одно животное.</p>

<p>Попробуйте сделать то же самое со структурой <code>Animal</code> вместо пары. Ничего не получится, если не научите компилятор сравнивать эти структуры. Это нормально.</p>

<h3>Подсказка</h3>

<p>Вызовите <code>ComputeTermFreqs</code> для вектора <code>animals</code>, проитерируйтесь по результату и найдите ключ с наибольшим значением. Если очередной ключ имеет частоту, равную уже найденной максимальной, не переписывайте им ключ-ответ.</p>
</details>  
