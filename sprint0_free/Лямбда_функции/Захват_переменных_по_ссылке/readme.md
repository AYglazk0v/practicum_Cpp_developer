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

<details>  
<summary>Инструкция по эксплуатации:</summary>  

<p>Вы познакомились с лямбда-функциями — синтаксическим сахаром С++. Теперь вам предстоит узнать, как получить от них максимум пользы.</p>

<h3>Стремитесь делать лямбда-функции лаконичными</h3>

<p>Основная цель лямбда-функций — сделать код проще для понимания. Например, по такому коду сразу видно, что выполняется сортировка по убыванию:</p>

<pre><code class="language-cpp">sort(v.begin(), v.end(), [](int x, int y) { return x &gt; y; });
</code></pre>

<p>Сложная логика в лямбда-функциях потребует слишком много внимания. Такой код лучше вынести в отдельную функцию.</p>

<p>Вот как выглядит громоздкая и непонятная лямбда-функция:</p>

<pre><code class="language-cpp">struct Person {
    int age;
    string first_name;
    string last_name;
    bool graduated_from_university;
    string city;
};

void FindPeopleLikeMe(const vector&lt;Person&gt;&amp; people) {
    int people_like_me_count = count_if(people.begin(), people.end(),
        [](const Person&amp; p) {
            if (p.age &lt; 18 || p.age &gt; 40) {
                return false;
            }
            if (p.last_name != &quot;Ivanov&quot;) {
                return false;
            }
            if (p.first_name == &quot;Vasiliy&quot; || p.first_name == &quot;Petr&quot;) {
                return false;
            }
            if (p.city == &quot;Moscow&quot; || p.city.find(&quot;Petersburg&quot;) != string::npos) {
                return false;
            }
            return p.graduated_from_university;
        });
    // ...
}
</code></pre>

<p>То же самое можно записать проще:</p>

<pre><code class="language-cpp">struct Person {
    int age;
    string first_name;
    string last_name;
    bool graduated_from_university;
    string city;
};

bool IsLikeMe(const Person&amp; p) {
    if (p.age &lt; 18 || p.age &gt; 40) {
        return false;
    }
    if (p.last_name != &quot;Ivanov&quot;) {
        return false;
    }
    if (p.first_name == &quot;Vasiliy&quot; || p.first_name == &quot;Petr&quot;) {
        return false;
    }
    if (p.city == &quot;Moscow&quot; || p.city.find(&quot;Petersburg&quot;) != string::npos) {
        return false;
    }
    return p.graduated_from_university;
}

void FindPeopleLikeMe(const vector&lt;Person&gt;&amp; people) {
    int people_like_me_count = count_if(people.begin(), people.end(), IsLikeMe);
    // ...
}
</code></pre>

<h3>Используйте лямбда-функции в алгоритмах</h3>

<p>Лучшее место для лямбда-функций — предикаты в алгоритмах <code>sort</code>, <code>count_if</code> и в стандартных алгоритмах, о которых пойдёт речь позже.</p>

<h3>Контролируйте захватываемые локальные переменные</h3>

<p>Этот код считает, сколько людей посетило больше стран, чем грустный пользователь <code>me</code>:</p>

<p>Допустим, пользователь решил посчитать, сколько людей посетило больше стран, чем он (или она):</p>

<pre><code class="language-cpp">struct Person {
    int age;
    string first_name;
    string last_name;
    set&lt;string&gt; visited_countries;
};

int CountBetterTravellersThanMe(const Person&amp; me, const vector&lt;Person&gt;&amp; people) {
    return count_if(people.begin(), people.end(), [](const Person&amp; p) {
        return p.visited_countries.size() &gt; me.visited_countries.size();
    });
}
</code></pre>

<p>Код не скомпилируется, потому что переменная <code>me</code> не захвачена:</p>

<pre><code>..\src\hw.cpp: In lambda function:
..\src\hw.cpp:18:45: error: 'me' is not captured
         return p.visited_countries.size() &gt; me.visited_countries.size();
</code></pre>

<p>Чтобы код скомпилировался, переменную <code>me</code> нужно захватить по значению:</p>

<pre><code class="language-cpp">int CountBetterTravellersThanMe(const Person&amp; me,
    const vector&lt;Person&gt;&amp; people) {
    return count_if(people.begin(), people.end(), [me](const Person&amp; p) {
        return p.visited_countries.size() &gt; me.visited_countries.size();
    });
}
</code></pre>

<p>Это самый простой, но неэффективный способ. Произойдёт глубокое копирование множества <code>visited_countries</code>. В общем случае такое копирование занимает много времени. Приемлемым решением будет захватить переменную <code>me</code> по ссылке:</p>

<pre><code class="language-cpp">int CountBetterTravellersThanMe(const Person&amp; me,
    const vector&lt;Person&gt;&amp; people) {
    return count_if(people.begin(), people.end(), [&amp;me](const Person&amp; p) {
        return p.visited_countries.size() &gt; me.visited_countries.size();
    });
}
</code></pre>

<p>Но есть способ лучше. От переменной <code>me</code> внутри лямбда-функции нужен только размер множества. Размер можно сохранить в целочисленную переменную и захватить в лямбде эту переменную, а не <code>me</code>:</p>

<pre><code class="language-cpp">int CountBetterTravellersThanMe(const Person&amp; me,
    const vector&lt;Person&gt;&amp; people) {
    int my_visited_countries = me.visited_countries.size();
    return count_if(people.begin(), people.end(),
        [my_visited_countries](const Person&amp; p) {
            return p.visited_countries.size() &gt; my_visited_countries;
        });
}
</code></pre>

<p>Так вы скопируете в лямбда-функцию всего одну целочисленную переменную.</p>

<p>Вы прошли путь от очевидного решения до способа, который не только заставляет программу компилироваться, но и работает эффективно.</p>

<h3>Захватывайте переменные простых типов по значению</h3>

<p>Может возникнуть желание захватывать все переменные по ссылке, ведь это никогда не приводит к глубокому копированию:</p>

<pre><code class="language-cpp">int main() {
    int threshold;
    cin &gt;&gt; threshold;

    const vector&lt;int&gt; v = {1, 3, 5, 2, 6, 7, 10, 2, 3};
    cout &lt;&lt; count_if(begin(v), end(v), [&amp;threshold](int x) { return x &gt; threshold; }) &lt;&lt; endl;
//                                      ^ threshold захвачен по ссылке
}
</code></pre>

<p>Делать так не стоит. Переменные, захваченные по ссылке, можно изменять изнутри лямбда-функции. Код ниже выводит <code>5</code>:</p>

<pre><code class="language-cpp">int main() {
    int x;
    auto lambda = [&amp;x]() { x = 5; };
    lambda();
    cout &lt;&lt; x &lt;&lt; endl;
}
</code></pre>

<p>У захвата переменной по ссылке две возможные цели:</p>

<ul>
<li>избежать долгого глубокого копирования;</li>
<li>дать лямбда-функции возможность изменить локальную переменную. Это нужно довольно редко.</li>
</ul>

<p>Переменные простых типов — <code>int</code>, <code>double</code>, <code>char</code>, <code>bool</code> — копируются быстро. Захват таких переменных по ссылке может вызвать у читателя вашего кода ощущение, что вы собираетесь их изменять.</p>

<p>Чтобы явно показать свои намерения, захватывайте переменные простых типов по значению, а переменные сложных типов — по ссылке.</p>

<h1>Заключение</h1>

<p>В этой теме вы узнали, как</p>

<ul>
<li>делать свой код удобным для чтения;</li>
<li>получать от лямбда-функции максимум пользы;</li>
<li>выбирать не самое очевидное, но самое эффективное решение задачи.</li>
</ul>

<p>Вы уже знаете очень многое. В продолжении курса вы не только расширите эти знания, но и откроете для себя новые возможности языка, усвоите важные детали и тонкости.</p>

</details> 
