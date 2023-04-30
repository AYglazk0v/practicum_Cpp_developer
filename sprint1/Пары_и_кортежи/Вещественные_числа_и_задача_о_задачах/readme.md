<details>  
<summary>Вещественные числа и задача о задачах:</summary>

# Вещественные числа и задача о задачах

Документы в выдаче вашей поисковой системы ранжируются по релевантности. Теперь вы умеете сортировать по нескольким параметрам и можете добавить второй уровень сортировки — например, по рейтингу.

Релевантность измеряется вещественными числами, и это всё осложняет. Память, отводимая под переменную типа  `double`, ограничена, а числа хранятся в двоичной записи. Поэтому их точность достаточно высока, но неидеальна. И успешно сравнивать вещественные числа на равенство можно далеко не всегда. Вот простой пример:

```cpp
#include <iostream>

using namespace std;

int main() {
    double x1 = 0;
    for (int i = 0; i < 3; ++i) {
        x1 += 1.0 / 5;
    }

    double x2 = 0;
    for (int i = 0; i < 9; ++i) {
        x2 += 1.0 / 15;
    }

    cout << "x1 = "s << x1 << endl;
		// выводит x1 = 0.6

    cout << "x2 = "s << x2 << endl;
		// выводит x2 = 0.6

    cout << "(x1 == x2) = "s << (x1 == x2) << endl;
		// выводит (x1 == x2) = 0
}

```

Оказывается, 0.6 ≠ 0.6! Разберёмся почему. Если перед выводом увеличить количество отображаемых значащих цифр до 20 командой  `cout.precision(20);`, увидим, что x1 = 0.60000000000000008882, а x2 = 0.5999999999999999778. То есть результат верный.

Чтобы избежать подобных сюрпризов, н**е сравнивайте вещественные числа на равенство:**  не используйте ни  `==`, ни  `!=`, ни  `<=`  и  `>=`. Если сделать это всё-таки нужно, вместо применения  `==`  вычислите разность чисел и проверьте, укладывается ли она в погрешность:

```cpp
#include <cmath>
#include <iostream>

using namespace std;

int main() {
    double x1 = 0;
    for (int i = 0; i < 3; ++i) {
        x1 += 1.0 / 5;
    }

    double x2 = 0;
    for (int i = 0; i < 9; ++i) {
        x2 += 1.0 / 15;
    }

    cout.precision(20);

    const double EPSILON = 1e-6;  // 10 в степени -6
    cout << "x1 = "s << x1 << endl;
		// выводит x1 = 0.6

    cout << "x2 = "s << x2 << endl;
		// выводит x2 = 0.6

    cout << "(x1 == x2) = "s << (x1 == x2) << endl;
		// выводит (x1 == x2) = 0

    cout << "(x1 ≈ x2) = "s << (abs(x1 - x2) < EPSILON) << endl;
		// выводит (x1 ≈ x2) = 1
}

```

Функция  `abs`  из библиотеки  `<cmath>`  вычисляет модуль — абсолютную величину — числа. В данном случае мы считаем x1 ≈ x2, если x1 - 10^(-6) < x2 < x1 + 10^(-6).

В конкретной задаче числа могут становиться достаточно большими или достаточно маленькими по абсолютной величине — допустим, близкими к нулю, как 10^(-6). В таких случаях вместо абсолютной погрешности используйте относительную: например, перед сравнением с  `EPSILON`  делите разность на одно из чисел.

</details>

<details>  
<summary>Возврат нескольких значений из функции:</summary>
<h1>Возврат нескольких значений из функции</h1>

<p>Вы научились создавать и сравнивать кортежи. А чтобы получить доступ к отдельным элементам кортежа, придётся написать довольно громоздкую конструкцию:</p>

<pre><code class="language-cpp">const tuple animal_info(&quot;Василий&quot;s, 5, 4.1);
cout &lt;&lt; &quot;Пациент &quot;s &lt;&lt; get&lt;0&gt;(animal_info)
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;1&gt;(animal_info) &lt;&lt; &quot; лет&quot;s
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;2&gt;(animal_info) &lt;&lt; &quot; кг&quot;s &lt;&lt; endl;
// Пациент Василий, 5 лет, 4.1 кг
</code></pre>

<p>Обратиться к полю можно и по его типу вместо индекса. При условии, если это единственное поле указанного типа:</p>

<pre><code class="language-cpp">const tuple animal_info(&quot;Василий&quot;s, 5, 4.1);
cout &lt;&lt; &quot;Пациент &quot;s &lt;&lt; get&lt;string&gt;(animal_info)
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;int&gt;(animal_info) &lt;&lt; &quot; лет&quot;s
	 &lt;&lt; &quot;, &quot;s &lt;&lt; get&lt;double&gt;(animal_info) &lt;&lt; &quot; кг&quot;s &lt;&lt; endl;
// Пациент Василий, 5 лет, 4.1 кг
</code></pre>

<p>У обоих способов те же недостатки, что у пары с полями <code>first</code> и <code>second</code>: неясно, что лежит в месте использования. Больше подойдёт структура с полями <code>name</code>, <code>age</code> и <code>weight</code>.</p>

<p>Тем не менее кортежи используют для возврата нескольких значений из функции. При этом к их содержимому обращаются, применив знакомую вам распаковку:</p>

<pre><code class="language-cpp">class SearchServer {
public:
	tuple&lt;vector&lt;string&gt;, DocumentStatus&gt; MatchDocument(const string&amp; raw_query, int document_id) const {
		// ...
	}

	// ...
};

// ...

const auto [words, status] = search_server.MatchDocument(&quot;белый кот&quot;s, 2);
</code></pre>

<p>Из метода возвращаются два объекта: <code>vector&lt;string&gt;</code> и <code>DocumentStatus</code>. В отличие от случаев, когда структура специально определена, у них нет самостоятельных названий. Догадаться о смысле объектов можно по типам и названию метода. Первый объект — это вектор слов запроса, которые нашлись в документе <code>document_id</code>, а второй объект — статус документа.</p>

<p>Допустим, мы применяем кортеж, чтобы вернуть объекты одинаковых или более тривиальных типов. Для этого потребовалось бы определить структуру с конкретными названиями полей:</p>

<pre><code class="language-cpp">// так непонятно
tuple&lt;int, int, double&gt; CheckDocument(/* ... */);

struct DocumentCheckResult {
	int word_count;
	int rating;
	double relevance;
};

// а так гораздо лучше
DocumentCheckResult CheckDocument(/* ... */);
</code></pre>

<p>Распаковка справится и с кортежем, и со своей структурой:</p>

<pre><code class="language-cpp">const auto [word_count, rating, relevance] = CheckDocument(/* ... */);
</code></pre>

<p>Распаковка — компактный способ сохранить несколько значений из функции.</p>

</details>  

<details>  
<summary>Задание 1:</summary>
<p>Исправьте сортировку документов в коде поисковой системы: при одинаковой — с точностью 10^(-6) — релевантности сортируйте документы по убыванию рейтинга.</p>

<p><strong>Пример</strong></p>

<pre><code class="language-cpp">void PrintDocument(const Document&amp; document) {
    cout &lt;&lt; &quot;{ &quot;s
         &lt;&lt; &quot;document_id = &quot;s &lt;&lt; document.id &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;relevance = &quot;s &lt;&lt; document.relevance &lt;&lt; &quot;, &quot;s
         &lt;&lt; &quot;rating = &quot;s &lt;&lt; document.rating
         &lt;&lt; &quot; }&quot;s &lt;&lt; endl;
}

int main() {
    SearchServer search_server;
    search_server.SetStopWords(&quot;и в на&quot;s);

    search_server.AddDocument(0, &quot;белый кот и модный ошейник&quot;s,        DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, &quot;пушистый кот пушистый хвост&quot;s,       DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, &quot;ухоженный пёс выразительные глаза&quot;s, DocumentStatus::ACTUAL, {5, -12, 2, 1});

    for (const Document&amp; document : search_server.FindTopDocuments(&quot;ухоженный кот&quot;s)) {
        PrintDocument(document);
    }
}
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>{ document_id = 2, relevance = 0.274653, rating = -1 }
{ document_id = 1, relevance = 0.101366, rating = 5 }
{ document_id = 0, relevance = 0.101366, rating = 2 }
</code></pre>

<h3>Подсказка</h3>

<p>Сравните по двум параметрам без пар и кортежей, но вместо <code>==</code> примените <code>abs</code> и сравнение с эпсилоном.</p>

</body>
</details>  

<details>  
<summary>Задание 2:</summary>

<p>Для работы над текущими задачами тимлид Матвей нанял двух разработчиков: Илью и Ивана. Матвей пристально следит за их успехами. Реализуйте класс <code>TeamTasks</code>, позволяющий хранить статистику по статусам задач Ивана и Ильи. Метод <code>PerformPersonTasks</code> должен реализовывать следующий алгоритм:</p>

<ol>
<li>Рассмотреть все невыполненные задачи разработчика <code>person</code>.</li>
<li>Упорядочить их по статусам: сначала все задачи в статусе NEW, затем все задачи в статусе IN_PROGRESS и, наконец, задачи в статусе TESTING.</li>
<li>Рассмотреть первые <code>task_count</code> задач и перевести каждую из них в следующий статус в соответствии с естественным порядком: NEW → IN_PROGRESS → TESTING → DONE.</li>
<li>Вернуть кортеж из двух элементов: информацию о статусах обновившихся задач — включая те, которые оказались в статусе DONE, — и информацию о статусах остальных невыполненных задач.</li>
</ol>

<p>Если разработчика с именем <code>person</code> нет, метод <code>PerformPersonTasks</code> должен вернуть кортеж из двух пустых <code>TasksInfo</code>.</p>

<p>Гарантируется, что <code>task_count</code> — положительное число. Если <code>task_count</code> превышает текущее количество невыполненных задач разработчика, достаточно считать, что <code>task_count</code> равен количеству невыполненных задач. Дважды обновлять статус какой-либо задачи в этом случае не нужно.</p>

<p>Гарантируется, что метод <code>GetPersonTasksInfo</code> не будет вызван для разработчика, не имеющего задач.</p>

<h3>Пример работы метода PerformPersonTasks</h3>

<p>Допустим, у конкретного разработчика десять задач со следующими статусами:</p>

<ul>
<li>NEW — 3</li>
<li>IN_PROGRESS — 2</li>
<li>TESTING — 4</li>
<li>DONE — 1</li>
</ul>

<p>Поступает команда <code>PerformPersonTasks</code> с параметром <code>task_count = 4</code>, что означает обновление статуса c NEW до IN_PROGRESS для трёх задач и с IN_PROGRESS до TESTING для одной задачи. Новые статусы задач:</p>

<ul>
<li>IN_PROGRESS — 3 обновлённых, 1 старая</li>
<li>TESTING — 1 обновлённая, 4 старых</li>
<li>DONE — 1 старая</li>
</ul>

<p>В этом случае нужно вернуть кортеж из двух словарей:</p>

<ul>
<li>Обновлённые задачи: IN_PROGRESS — 3, TESTING — 1.</li>
<li>Необновлённые задачи, исключая выполненные: IN_PROGRESS — 1, TESTING — 4.</li>
</ul>

<p>Ни один из словарей не должен содержать лишних элементов, то есть статусов, которым соответствует ноль задач.</p>

<p>DONE задачи не нужно возвращать в необновлённых задачах (<code>untouched_tasks</code>).</p>

<h3>Пример ввода:</h3>

<pre><code class="language-cpp">// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout &lt;&lt; tasks_info[TaskStatus::NEW] &lt;&lt; &quot; new tasks&quot; &lt;&lt;
        &quot;, &quot; &lt;&lt; tasks_info[TaskStatus::IN_PROGRESS] &lt;&lt; &quot; tasks in progress&quot; &lt;&lt;
        &quot;, &quot; &lt;&lt; tasks_info[TaskStatus::TESTING] &lt;&lt; &quot; tasks are being tested&quot; &lt;&lt;
        &quot;, &quot; &lt;&lt; tasks_info[TaskStatus::DONE] &lt;&lt; &quot; tasks are done&quot; &lt;&lt; endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask(&quot;Ilia&quot;);
    for (int i = 0; i &lt; 3; ++i) {
        tasks.AddNewTask(&quot;Ivan&quot;);
    }
    cout &lt;&lt; &quot;Ilia's tasks: &quot;;
    PrintTasksInfo(tasks.GetPersonTasksInfo(&quot;Ilia&quot;));
    cout &lt;&lt; &quot;Ivan's tasks: &quot;;
    PrintTasksInfo(tasks.GetPersonTasksInfo(&quot;Ivan&quot;));
  
    TasksInfo updated_tasks, untouched_tasks;
  
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks(&quot;Ivan&quot;, 2);
    cout &lt;&lt; &quot;Updated Ivan's tasks: &quot;;
    PrintTasksInfo(updated_tasks);
    cout &lt;&lt; &quot;Untouched Ivan's tasks: &quot;;
    PrintTasksInfo(untouched_tasks);
  
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks(&quot;Ivan&quot;, 2);
    cout &lt;&lt; &quot;Updated Ivan's tasks: &quot;;
    PrintTasksInfo(updated_tasks);
    cout &lt;&lt; &quot;Untouched Ivan's tasks: &quot;;
    PrintTasksInfo(untouched_tasks);
}
</code></pre>

<h3>Пример вывода:</h3>

<pre><code class="language-cpp">Ilia's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Ivan's tasks: 3 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 2 tasks in progress, 0 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 1 tasks in progress, 1 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 0 new tasks, 1 tasks in progress, 0 tasks are being tested, 0 tasks are done
</code></pre>

<h3>Подсказка</h3>

<p>Обновление словаря одновременно с итерированием по нему может привести к непредсказуемым последствиям. Сначала соберите информацию об обновлениях в отдельном временном словаре, а потом примените их к основному словарю.</p>


</details>  
