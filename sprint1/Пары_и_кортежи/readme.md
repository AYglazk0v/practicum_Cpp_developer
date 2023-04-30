<details>  
<summary>Введение</summary>
<p>Вы уже знакомы с парами в C++. Они позволяют объединить две переменных в одну и сравнить полученные объекты стандартными средствами, без компаратора. Но всё же удобнее написать структуру.</p>

<p>В этой теме вы научитесь применять пары в сочетании со структурами. Также вы познакомитесь с кортежами. Они расширяют пары и могут хранить более двух объектов без создания отдельной структуры.</p>

</details>  

<details>  
<summary>Улучшаем сравнение:</summary>
<p>Вы уже умеете сортировать объекты по одному параметру. Допустим, информация очевидцев о потерянном животном хранится в структуре:</p>

<pre><code class="language-cpp">struct AnimalObservation {
	string name;   // кличка
	int days_ago;  // сколько дней назад видели
};
</code></pre>

<p>Животных, которых видели недавно, проще найти. Поэтому они интересуют нас в первую очередь. Напишем вектор структур и сортируем компаратором по <code>days_ago</code>:</p>

<pre><code class="language-cpp">vector&lt;AnimalObservation&gt; observations = {{&quot;Мурка&quot;s, 3}, {&quot;Рюрик&quot;s, 1}, {&quot;Веня&quot;s, 2}};
sort(observations.begin(), observations.end(),
	[](const AnimalObservation&amp; lhs, const AnimalObservation&amp; rhs) {
		return lhs.days_ago &lt; rhs.days_ago;
	});
// получим: {&quot;Рюрик&quot;s, 1}, {&quot;Веня&quot;s, 2}, {&quot;Мурка&quot;s, 3}
</code></pre>

<p>Если очевидцев станет больше, они будут каждый день обнаруживать десятки животных. Определим, кого искать в первую очередь. Попросим свидетелей оценивать состояние здоровья животного по шкале от нуля (нужна срочная помощь) до десяти (выглядит абсолютно счастливым):</p>

<pre><code class="language-cpp">struct AnimalObservation {
	string name;
	int days_ago;
	int health_level;  // состояние здоровья
};
</code></pre>

<p>Теперь отсортируем животных не только по давности свидетельства. Пусть среди свидетельств с одинаковым <code>days_ago</code> раньше идут те, которые имеют наименьший <code>health_level</code>. Применим условный оператор:</p>

<pre><code class="language-cpp">sort(observations.begin(), observations.end(),
	[](const AnimalObservation&amp; lhs, const AnimalObservation&amp; rhs) {
		if (lhs.days_ago == rhs.days_ago) {
			return lhs.health_level &lt; rhs.health_level;
		} else {
			return lhs.days_ago &lt; rhs.days_ago;
		}
	});
</code></pre>

<p>Этот компаратор можно записать и короче — в виде сложного логического выражения:</p>

<pre><code class="language-cpp">sort(observations.begin(), observations.end(),
	[](const AnimalObservation&amp; lhs, const AnimalObservation&amp; rhs) {
		// благодаря приоритету операций скобки в выражении необязательны
		return lhs.days_ago &lt; rhs.days_ago
			|| (lhs.days_ago == rhs.days_ago
				&amp;&amp; lhs.health_level &lt; rhs.health_level);
	});
</code></pre>

<p>Если <code>lhs.days_ago &lt; rhs.days_ago</code>, первый параметр будет считаться меньше второго, так как первый аргумент операции <code>||</code> — истинный. А если <code>lhs.days_ago == rhs.days_ago</code>, первый параметр будет меньше только при <code>lhs.health_level &lt; rhs.health_level</code>.</p>

</details>

<details>  
<summary>Задание:</summary>
<p>Вы сортировали найденные поисковой системой документы по убыванию релевантности. Не меняя данной функции, исправьте компаратор так, чтобы он сортировал по убыванию рейтинга. А при одинаковом рейтинге — по убыванию релевантности.</p>

<p>Код из тренажёра должен вывести следующее:</p>

<pre><code>102 0.3 5
101 1.2 4
100 0.5 4
</code></pre>

<h3>Подсказка</h3>

<p>Когда рейтинг первого параметра больше рейтинга второго — результат компаратора точно <code>true</code>. Иначе такой результат можно получить лишь при равном рейтинге и большей релевантности первого документа.</p>

</details>  
