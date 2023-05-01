<details>  
<summary>Теория:</summary>

<h1>Как устроены шаблоны</h1>

<p>Типы в С++ фиксированы. Поэтому принципы работы шаблонов могут быть непонятны. Разобраться в этом часто помогают сообщения об ошибках. Вызовем <code>ComputeTermFreqs</code> для вектора структур:</p>

<pre><code class="language-cpp">struct Animal {
    string name;
    int age;
};

Animal FindMaxFreqAnimal(const vector&lt;Animal&gt;&amp; animals) {
    int max_freq = 0;
    Animal max_freq_animal;

    // вот здесь вызываем шаблонную функцию
    for (const auto&amp; [animal, freq] : ComputeTermFreqs(animals)) {
        if (freq &gt; max_freq) {
            max_freq = freq;
            max_freq_animal = animal;
        }
    }
    return max_freq_animal;
}
</code></pre>

<p>Получим сообщение об ошибке компиляции. Нас интересует эта часть:</p>

<pre><code class="language-cpp">tf_05.cpp:14:21:   required from ‘std::map&lt;Term, int&gt; ComputeTermFreqs(const std::vector&lt;Term&gt;&amp;) [with Term = Animal]’
tf_05.cpp:27:63:   required from here
/usr/include/c++/10/bits/stl_function.h:386:20: error: no match for ‘operator&lt;’ (operand types are ‘const Animal’ and ‘const Animal’)
  386 |       { return __x &lt; __y; }
      |                ~~~~^~~~~
</code></pre>

<p>Шаблонная функция не скомпилировалась, потому что не смогла сравнить двух животных  операцией <code>&lt;</code>. Компилятор требует сравнения тут: <code>required from</code> ‘<code>std::map&lt;Term, int&gt; ComputeTermFreqs(const std::vector&lt;Term&gt;&amp;) [with Term = Animal]</code>’. То есть <code>ComputeTermFreqs</code> с <code>Term = Animal</code> — это отдельная функция. А вот <code>ComputeTermFreqs</code> с <code>Term = int</code> компилировалась и работала.</p>

<p>Чтобы стало понятнее, вспомним другой пример. Когда мы считали количество двуногих и четвероногих жильцов, применили функцию <code>ComputeTermFreqs</code>. Указать, с каким <code>Term</code> вызвать функцию, можно явно:</p>

<pre><code class="language-cpp">const vector&lt;int&gt; leg_counts = {4, 2, 4, 4};
//                          конкретный Term в угловых скобках!
const auto legs_stat = ComputeTermFreqs&lt;int&gt;(leg_counts);

cout &lt;&lt; &quot;Двуногих &quot;s &lt;&lt; legs_stat.at(2) &lt;&lt; &quot;, &quot;s
     &lt;&lt; &quot;четвероногих &quot;s &lt;&lt; legs_stat.at(4) &lt;&lt; endl;
// Двуногих 1, четвероногих 3
</code></pre>

<p>Эта конструкция не зря напоминает о <code>vector&lt;int&gt;</code> — позже мы познакомимся и с шаблонными классами.</p>

<p>Если промахнуться с явно указанным типом, ничего не выйдет. Функция <code>ComputeTermFreqs&lt;string&gt;</code> ожидает на вход вектор строк:</p>

<pre><code class="language-cpp">const vector&lt;int&gt; leg_counts = {4, 2, 4, 4};
const auto legs_stat = ComputeTermFreqs&lt;string&gt;(leg_counts);
// ошибка компиляции: const vector&lt;string&gt;&amp; не может ссылаться на const vector&lt;int&gt;

cout &lt;&lt; &quot;Двуногих &quot;s &lt;&lt; legs_stat.at(2) &lt;&lt; &quot;, &quot;s
     &lt;&lt; &quot;четвероногих &quot;s &lt;&lt; legs_stat.at(4) &lt;&lt; endl;
</code></pre>

<p>Важные свойства шаблонных функций:</p>

<ol>
<li><code>ComputeTermFreqs&lt;int&gt;</code>, <code>ComputeTermFreqs&lt;string&gt;</code> и функции с любыми другими <code>Term</code> в угловых скобках — это разные функции. Компилятор копирует их, подставляя нужный тип вместо <code>Term</code>. Конкретная <code>ComputeTermFreqs&lt;Animal&gt;</code> может не скомпилироваться, но по умолчанию никаких требований к типу нет: главное, чтобы все операции были определены.</li>
<li>При вызове шаблонной функции можно указать в угловых скобках значение её шаблонного параметра. А можно и не указывать — тогда компилятор постарается вывести шаблонные параметры из типов аргументов. Например, передали <code>const vector&lt;int&gt;</code>, ожидался <code>const vector&lt;Term&gt;&amp;</code> — значит, <code>Term = int</code>.</li>
</ol>

</details>  

<details>  
<summary>Задание :</summary>
<p>Реализуйте шаблонную функцию <code>ComputeTfIdfs</code>, которая вычисляет TF-IDF заданного слова для каждого документа из набора.</p>

<ul>
<li>Первый параметр <code>documents</code> — контейнер документов. Циклом <code>for (const auto&amp; document : documents)</code> можно перебрать все документы в нём, а в каждом документе — все слова. Тип слова, документа и набора документов может быть произвольным — ваша функция должна быть готова к любым, если есть возможность итерирования.
Гарантируется, что и набор документов, и каждый отдельный документ имеют методы <code>begin</code>, <code>end</code> и <code>size</code>. Например, документом может быть строка, а словом — символ.</li>
<li>Второй параметр <code>term</code> — слово, у которого нужно вычислить TF-IDF. Его тип совпадает с типом слов, которые получаются при итерировании по документу.</li>
<li>Функция должна вернуть вектор вещественных чисел, который совпадает по длине с количеством документов. В первом элементе должен лежать TF-IDF слова <code>term</code> для первого документа, в последнем элементе — TF-IDF слова <code>term</code> для последнего документа.</li>
</ul>

<p>Напоминаем, что TF-IDF — это произведение TF и IDF. TF слова <code>term</code> в документе — доля слов документа, совпадающих с <code>term</code>. IDF вычисляется для слова независимо от конкретного документа и равен логарифму (функция <code>log</code> из <code>&lt;cmath&gt;</code>) от <code>documents.size() / document_freq</code>, где знаменатель — это количество документов, содержащих <code>term</code>.</p>

<p>Пример из тренажёра должен вывести <code>0.081093 0.101366 0</code>.</p>

<h3>Подсказка</h3>

<ul>
<li>Ваша функция должна иметь два шаблонных параметра: <code>Documents</code> для набора документов и <code>Term</code> для слова. Указывайте их через запятую. Тип документа при этом никак не называйте: достаточно написать <code>auto</code>.</li>
<li>Используйте алгоритм <code>count</code> для подсчёта количества вхождений слова в документ.</li>
<li>Не забывайте перед делением приводить целые числа к <code>double</code> оператором <code>static_cast</code>.</li>
</ul>
</details>  
