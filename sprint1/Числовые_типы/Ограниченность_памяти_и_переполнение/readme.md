<details>  
<summary>Введение:</summary>
<h1>Целочисленные типы</h1>

<p>Во вводном курсе мы рассмотрели стандартный целочисленный тип <code>int</code>. Все числа, записанные в коде в виде цифр, по умолчанию имеют тип <code>int</code>, если они достаточно маленькие (см. таблицу ниже). Так, если написать <code>auto x = 1</code>, переменная <code>x</code> автоматически получит тип <code>int</code>. Но это не единственный целочисленный тип данных в С++.</p>

<p>Целочисленные типы содержат положительные и отрицательные целые числа без дробной части. Они отличаются размером, занимаемой памятью и диапазоном значений. Есть знаковые (signed) и беззнаковые (unsigned) целочисленные типы. Знаковые хранят положительные и отрицательные числа и ноль, а беззнаковые — только натуральные числа, включая ноль.</p>

<h2>Целочисленные типы С++</h2>

<table>
<thead>
<tr>
<th align="center"><strong>Тип</strong></th>
<th align="center"><strong>Количество байтов</strong></th>
<th align="center"><strong>Количество бит</strong></th>
<th align="center"><strong>Диапазон значений</strong></th>
</tr>
</thead>
<tbody>
<tr>
<td align="center">int</td>
<td align="center">4</td>
<td align="center">32</td>
<td align="center">от -2 147 483 648 до 2 147 483 647</td>
</tr>
<tr>
<td align="center">unsigned int</td>
<td align="center">4</td>
<td align="center">32</td>
<td align="center">от 0 до 4 294 967 295</td>
</tr>
<tr>
<td align="center">int8_t</td>
<td align="center">1</td>
<td align="center">8</td>
<td align="center">от -128 до 127</td>
</tr>
<tr>
<td align="center">uint8_t</td>
<td align="center">1</td>
<td align="center">8</td>
<td align="center">от 0 до 255</td>
</tr>
<tr>
<td align="center">int16_t</td>
<td align="center">2</td>
<td align="center">16</td>
<td align="center">от -32 768 до 32 767</td>
</tr>
<tr>
<td align="center">uint16_t</td>
<td align="center">2</td>
<td align="center">16</td>
<td align="center">от 0 до 65 535</td>
</tr>
<tr>
<td align="center">int32_t</td>
<td align="center">4</td>
<td align="center">32</td>
<td align="center">от -2 147 483 648 до 2 147 483 647</td>
</tr>
<tr>
<td align="center">uint32_t</td>
<td align="center">4</td>
<td align="center">32</td>
<td align="center">от 0 до 4 294 967 295</td>
</tr>
<tr>
<td align="center">int64_t</td>
<td align="center">8</td>
<td align="center">64</td>
<td align="center">от -9 223 372 036 854 775 808 до 9 223 372 036 854 775 807</td>
</tr>
<tr>
<td align="center">uint64_t</td>
<td align="center">8</td>
<td align="center">64</td>
<td align="center">от 0 до 18 446 744 073 709 551 615</td>
</tr>
</tbody>
</table>
<p>Все типы с суффиксом <code>_t</code> из таблицы доступны при подключении библиотеки <code>cstdint</code>. Гарантируется, что они имеют указанный размер и диапазон значений.</p>

<p>Тип <code>int</code> (и его беззнаковый товарищ <code>unsigned int</code>) стандартный и действительно, как правило, занимает четыре байта, но это может быть не так на процессорах старой или нестандартной архитектуры.</p>

<p>В старом коде вы можете встретить и другие типы, размер которых чётко не определён. Но для основных архитектур всё-таки можем привести сравнение:</p>

<ul>
<li><code>char</code> — <code>int8_t</code> или <code>uint8_t</code>. Да-да, символ в C++ — это просто число с кодом этого символа.</li>
<li><code>signed char</code> — <code>int8_t</code></li>
<li><code>unsigned char</code> — <code>uint8_t</code></li>
<li><code>short int</code> или <code>short</code> — <code>int16_t</code></li>
<li><code>unsigned short int</code> или <code>unsigned short</code> — <code>uint16_t</code></li>
<li><code>long int</code> или <code>long</code> — <code>int32_t</code></li>
<li><code>unsigned long int</code> или <code>unsigned long</code> — <code>uint32_t</code></li>
<li><code>long long int</code> или <code>long long</code> — <code>int64_t</code></li>
<li><code>unsigned long long int</code> или <code>unsigned long long</code> — <code>uint64_t</code></li>
</ul>

<p>В новом коде принято использовать <code>int</code> или типы с суффиксом <code>_t</code> из <code>cstdint</code>.</p>

<p>Разберёмся, как выбрать наиболее подходящий для решения задачи целочисленный тип данных.</p>

<ul>
<li>Если вам достаточно размера и диапазона значений стандартного типа <code>int</code> — используйте его.</li>
<li>Если хотите хранить числа порядка триллиона, размер типа <code>int</code> не подойдёт — нужен <code>int64_t</code>.</li>
<li>Если возникла необходимость экономить память на числах, применяйте типы меньшей размерности, такие как <code>int8_t</code>, <code>int16_t</code>.</li>
<li>Если вы пишете программу, которая будет запускаться на неизвестной архитектуре, и нуждаетесь в 32-битном типе — выберите <code>int32_t</code>.</li>
</ul>

<p>Размеры всех типов данных заучивать необязательно: в С++ есть оператор <code>sizeof</code>, который возвращает размер в байтах. Например, мы забыли размер целочисленного типа <code>int16_t</code>. Применим <code>sizeof</code>:</p>

<pre><code class="language-cpp">cout &lt;&lt; sizeof(int16_t) &lt;&lt; endl;
</code></pre>

<p>Программа напомнит, что размер <code>int16_t</code> составляет два байта.</p>

<p>Можно применить <code>sizeof</code> и к выражению. Так мы узнаем, что размер вектора занимает восемь байт:</p>

<pre><code class="language-cpp">cout &lt;&lt; sizeof(vector&lt;string&gt;{}.size()) &lt;&lt; endl;
</code></pre>

<p>Размер пустого вектора строк нулевой. Но зато каков этот ноль! Целых восемь байт.</p>

<p><code>sizeof</code> удобен, когда вы уже мыслите в битах и на ходу строите логические цепочки: тип занимает восемь байт → это 64 бита → 2^64 больше 10^18 → число из 18 цифр поместится легко. Но можно и явно попросить компилятор вывести минимальное и максимальное значение любого целочисленного типа. Для этого подключим библиотеку <code>limits</code>:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;limits&gt;

using namespace std;

int main() {
    cout &lt;&lt; &quot;sizeof = &quot;s &lt;&lt; sizeof(int) &lt;&lt; endl;
    cout &lt;&lt; &quot;min = &quot;s &lt;&lt; numeric_limits&lt;int&gt;::min() &lt;&lt; endl;
    cout &lt;&lt; &quot;max = &quot;s &lt;&lt; numeric_limits&lt;int&gt;::max() &lt;&lt; endl;
}
</code></pre>

<p>Запустим код и увидим всё про тип <code>int</code> на нашем процессоре:</p>

<pre><code class="language-cpp">sizeof = 4
min = -2147483648
max = 2147483647
</code></pre>

<p>Вы узнали, какие целочисленные типы есть и какой объём памяти они занимают. Научились выяснять по целочисленному типу его размер, минимальное и максимальное значение. В C++ память для целочисленных типов ограничена. Далее разберёмся, что такое переполнение целочисленного типа.</p>

<p>Проверьте себя. Выберите наиболее подходящий и экономный тип данных для целого числа в каждой ситуации. Исходите из того, что мы живём здесь и сейчас и не планируем рост указанных величин.</p>

</details>  

<details>  
<summary>Теория:</summary>  
<p>На вводном курсе вы изучили стандартный целочисленный тип <code>int</code>. В этой теме познакомитесь с другими целочисленными типами С++, узнаете их размеры и правила преобразования друг к другу. Научитесь избегать переполнения типов. И убедитесь, что компилятор — ваш заклятый друг.</p>

<p>Вы уже знаете основы С++ и умеете писать на нём довольно сложные программы. Выполните задания и вспомните всё.</p>
<h1>Ограниченность памяти и переполнение</h1>

<p>Вы уже знаете, что целочисленные типы данных отличаются диапазоном значений. Если значение переменной находится вне диапазона значений указанного типа, происходит переполнение.</p>

<p>Рассмотрим примеры. Возьмём максимальное значение типа <code>int</code> и прибавим к нему единицу:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;limits&gt;

using namespace std;

int main() {
    cout &lt;&lt; numeric_limits&lt;int&gt;::max() + 1 &lt;&lt; endl;
}
</code></pre>

<p>Современные компиляторы даже выдадут предупреждение, что мы встали на скользкую дорожку:</p>

<pre><code> warning: integer overflow in expression of type ‘int’ results in ‘-2147483648’ [-Woverflow]
</code></pre>

<p>И действительно, при запуске вместо ожидаемого 2 147 483 648 получаем отрицательное число — минимальное значение типа <code>int</code>:</p>

<pre><code class="language-cpp">-2147483648
</code></pre>

<p>Это произошло, потому что значение выражения в <code>int</code> не поместилось.</p>

<p>Теперь возьмём значение меньше минимального:</p>

<pre><code class="language-cpp">cout &lt;&lt; numeric_limits&lt;int&gt;::min() - 1 &lt;&lt; endl;
</code></pre>

<p>Снова предупреждение и снова не то, чего ожидали:</p>

<pre><code class="language-cpp">2147483647
</code></pre>

<p>Берём число на единицу больше, чем максимум, — получаем минимум. Берём число на единицу меньше, чем минимум, — получаем максимум. Такое в С++ переполнение.</p>

<p>Более практичный пример. Допустим, у нас есть два числа типа <code>int</code>: два миллиарда и один миллиард. Вычислим среднее арифметическое. Оба числа не превышают максимального значения <code>int</code>. Предполагаем, что их среднее арифметическое не превысит два миллиарда и тоже поместится в <code>int</code>. Спокойно пишем программу:</p>

<pre><code class="language-cpp">// разряды в больших числах удобно разделять символом '
int x = 2'000'000'000;
int y = 1'000'000'000;
cout &lt;&lt; (x + y) / 2 &lt;&lt; endl;
</code></pre>

<p>Вместо ожидаемых полутора миллиардов получаем:</p>

<pre><code class="language-cpp">-647483648
</code></pre>

<p>С++ сначала выполнил действие в скобках: сложил <code>x</code> и <code>y</code>. Получилось три миллиарда — число, которое превышает максимальное значение типа <code>int</code>. После этого всё и пошло наперекосяк.</p>

<p>Выбирая тип данных, нужно продумать каждый сегмент программы. Если промежуточные вычисления не поместятся в ваш тип, рискуете получить большое отрицательное число — верный признак переполнения. А когда выполняете серию арифметических операций, переполнение можете и вовсе не заметить.</p>

<p>Посмотрим, как это работает с беззнаковыми типами. Сохраним переменную <code>x</code> со значением два миллиарда в беззнаковую переменную <code>y</code> и выведем обе:</p>

<pre><code class="language-cpp">int x = 2'000'000'000;
unsigned int y = x;
cout &lt;&lt; x &lt;&lt; &quot; &quot;s &lt;&lt; y &lt;&lt; endl;
</code></pre>

<p>Запустим код и увидим, что всё в порядке, ведь это значение помещается и в <code>int</code>, и в <code>unsigned int</code>:</p>

<pre><code class="language-cpp">2000000000 2000000000
</code></pre>

<p>Теперь сохраним в переменную <code>x</code> отрицательное значение:</p>

<pre><code class="language-cpp">int x = -2'000'000'000;
unsigned int y = x;
cout &lt;&lt; x &lt;&lt; &quot; &quot;s &lt;&lt; y &lt;&lt; endl;
</code></pre>

<p>Запустим код. В беззнаковый тип наше значение не поместилось:</p>

<pre><code class="language-cpp">-2000000000 2294967296
</code></pre>

<p>Так происходит переполнение беззнакового целочисленного типа.</p>

<p>С одним беззнаковым типом мы уже сталкивались — это тип <code>size_t</code>. Во-первых, именно его возвращает метод <code>size</code> контейнеров. Во-вторых, результат вызова оператора <code>sizeof</code> тоже имеет тип <code>size_t</code>. Это вполне логично: размер чего-либо не может быть отрицательным.</p>

<p>Размер самого типа <code>size_t</code> зависит от разрядности вашей операционной системы. Если система 32-битная — размер <code>size_t</code> составит 32 бита (четыре байта), если 64-битная — 64 бита (восемь байт).</p>

<p>Вернёмся к задаче о среднем рейтинге. Имея рейтинги −5, 1 и −2, найдём их среднее арифметическое:</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;vector&gt;

using namespace std;

int ComputeAverageRating(const vector&lt;int&gt;&amp; ratings) {
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = 0;
    for (const int rating : ratings) {
        rating_sum += rating;
    }
    return rating_sum / ratings.size();
}

int main() {
    cout &lt;&lt; ComputeAverageRating({1, -7, 3}) &lt;&lt; endl;
}
</code></pre>

<p>Средний рейтинг должен составить (−5+1−2) / 3 = −2. Но результат намекает на переполнение:</p>

<pre><code>1431655763
</code></pre>

<p>Дело в финальном выражении функции: <code>rating_sum / ratings.size()</code>. Чтобы разделить <code>int</code> на <code>size_t</code>, компилятор привёл оба операнда к беззнаковому типу. В нём −2 не сохранить.</p>

<p>Поэтому и помогало предварительно привести размер вектора к знаковому типу <code>int</code>:</p>

<pre><code class="language-cpp">return rating_sum / static_cast&lt;int&gt;(ratings.size());
</code></pre>

<p>Избегайте переполнения типов: даже если код работает при ваших текущих настройках, он может дать сбой на другом компиляторе или ОС. Ситуаций, когда переполнение приносит пользу, — мало. Едва ли вы с ними столкнётесь.</p>

<p>Выбирайте подходящие типы и преобразовывайте их оператором <code>static_cast</code>. В следующем уроке вы научитесь этому на конкретных примерах и узнаете о нюансах приведения операндов к общему типу.</p>

</details>  

<details>  
<summary>Задание:</summary> 
<p>Пощупайте переполнение типов <code>int64_t</code> и <code>uint64_t</code> на простом примере.</p>

<p>Запомните в переменную <code>min</code> минимальное значение типа <code>int64_t</code>, а в переменную <code>max</code> — максимальное значение <code>uint64_t</code>.</p>

<p>Никак не преобразуя типы, выведите 5 чисел, каждое на новой строке:</p>

<ul>
<li><code>min</code></li>
<li><code>max</code></li>
<li>Сумму <code>min</code> и <code>max</code></li>
<li>Удвоенный <code>min</code></li>
<li>Удвоенный <code>max</code></li>
</ul>

<p>Посмотрите на вывод — один из результатов вычислений будет неожиданным.</p>

<h3>Подсказка</h3>

<p>Обратите внимание на типы <code>min</code> и <code>max</code>: они должны быть <code>int64_t</code> и <code>uint64_t</code>. Вычисляя сумму и произведение, не преобразуйте аргументы: суть задачи именно в демонстрации переполнения.</p>
</details>  
