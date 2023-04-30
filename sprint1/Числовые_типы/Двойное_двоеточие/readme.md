<details>  
<summary>Оператор switch:</summary>
<p>В коде с перечислением мы несколько раз используем <code>if</code>. Выглядит это громоздко:</p>

<pre><code class="language-cpp">enum class RequestType {
	  ADD,
	  REMOVE,
	  NEGATE
};

void ProcessRequest(
		    set&lt;int&gt;&amp; numbers,
		    RequestType request_type,
		    int request_data) {
	  if (request_type == RequestType::ADD) {
		    numbers.insert(request_data);
	  } else if (request_type == RequestType::REMOVE) {
		    numbers.erase(request_data);
	  } else if (request_type == RequestType::NEGATE) {
		    if (numbers.count(request_data) == 1) {
			      numbers.erase(request_data);
			      numbers.insert(-request_data);
		    }
	  }
}

ProcessRequest(numbers, RequestType::ADD, 8);
ProcessRequest(numbers, RequestType::NEGATE, 8);
ProcessRequest(numbers, RequestType::REMOVE, -8);

</code></pre>

<p>Но нам повезло: в C++ есть компактная альтернатива цепочке условных операторов — оператор <code>switch</code>. В отличие от <code>if</code>, <code>switch</code> не может проверять произвольные логические выражения. Он позволяет сравнить заданную переменную или результат выражения с конкретными значениями и выполнить действия в зависимости от того, с каким значением произошло совпадение. Используем оператор <code>switch</code> и перепишем функцию <code>ProcessRequest</code>:</p>

<pre><code class="language-cpp">void ProcessRequest(
		    set&lt;int&gt;&amp; numbers,
		    RequestType request_type,
		    int request_data) {
	  switch (request_type) {
	  case RequestType::ADD:
		    numbers.insert(request_data);
		    break;
	  case RequestType::REMOVE:
		    numbers.erase(request_data);
		    break;
	  case RequestType::NEGATE:
		    if (numbers.count(request_data) == 1) {
			      numbers.erase(request_data);
			      numbers.insert(-request_data);
		    }
		    break;
	  }
}
</code></pre>

<p>Каждая ветка кончается оператором <code>break</code>. Фигурные скобки ей не нужны. Оператор <code>break</code> означает выход из оператора <code>switch</code> и не повлечёт за собой выход из объемлющего цикла <code>for</code> при его наличии.</p>

<p>Компактный аналог <code>else</code> для оператора <code>switch</code> — ветка <code>default</code>. Допустим, мы хотим перестраховаться от добавления новых типов запросов и добавить вывод предупреждающего сообщения для неизвестного запроса. <code>default</code>-ветка выполнится всегда, если не подошла ни одна <code>case</code>-ветка:</p>

<pre><code class="language-cpp">void ProcessRequest(
		    set&lt;int&gt;&amp; numbers,
		    RequestType request_type,
		    int request_data) {
	  switch (request_type) {
	  case RequestType::ADD:
		    numbers.insert(request_data);
		    break;
	  case RequestType::REMOVE:
		    numbers.erase(request_data);
		    break;
	  case RequestType::NEGATE:
		    if (numbers.count(request_data) == 1) {
			      numbers.erase(request_data);
			      numbers.insert(-request_data);
		    }
		    break;
	  default:
		    cout &lt;&lt; &quot;Unknown request&quot;s &lt;&lt; endl;
	  }
}
</code></pre>

<p>Если объявляем переменную в одной из веток оператора <code>switch</code>, всю ветку заключаем в блок из фигурных скобок:</p>

<pre><code class="language-cpp">void ProcessRequest(
		    set&lt;int&gt;&amp; numbers,
		    RequestType request_type,
		    int request_data) {
	  switch (request_type) {
	  case RequestType::ADD:
		    numbers.insert(request_data);
		    break;
	  case RequestType::REMOVE:
		    numbers.erase(request_data);
		    break;
	  case RequestType::NEGATE: {  // фигурные скобки обязательны
		    bool contains = numbers.count(request_data) == 1;
		    if (contains) {
		      numbers.erase(request_data);
		      numbers.insert(-request_data);
		    }
		    break;
	  }
	  default:
		    cout &lt;&lt; &quot;Unknown request&quot; &lt;&lt; endl;
	  }
}
</code></pre>

<p>В нашем случае нумерация перечислителей в наборе не имела значения. Но когда числовые значения элементов <code>enum</code> важны, их можно указать явно:</p>

<pre><code class="language-cpp">enum class RequestType {
    ADD = 9,
    REMOVE = 8,
    NEGATE = 7
};
</code></pre>

<p>Это удобно, когда типы запросов поступают на вход программы в виде чисел 9, 8 и 7. Тогда получить объект типа <code>RequestType</code> по его числовому коду можно будет простым оператором <code>static_cast</code>:</p>

<pre><code class="language-cpp">int request_code;
cin &gt;&gt; request_code;
auto request_type = static_cast&lt;RequestType&gt;(request_code);
// Если ввести request_code = 7,
// в переменной request_type окажется RequestType::NEGATE
</code></pre>

<p>Если преобразуемому числовому коду не будет соответствовать ни один элемент перечислимого типа, ошибки компиляции не произойдёт. Но дальнейшее поведение программы грозит стать непредсказуемым.</p>

</details>  

<details>  
<summary>Двойное двоеточие</summary>
<p>В прошлом уроке вы столкнулись с такой записью для значений перечислимого типа: <code>DocumentStatus::ACTUAL</code>. Рассмотрим, зачем здесь двойное двоеточие и как его применяют в C++.</p>

<p>Двойное двоеточие — оператор разрешения области видимости. Он позволяет делать значения <code>enum</code>-типа неуникальными в рамках всей программы. Это одно из преимуществ <code>enum class</code>: все имена значений типа «спрятаны» внутрь его имени. Обращение к ним происходит через двойное двоеточие.</p>

<p>В записи <code>DocumentStatus::ACTUAL</code> первая часть — название группы имён, а вторая — конкретное имя из этой группы. Название группы имён совпадает с названием типа, что вполне согласуется со здравым смыслом. Это не единственная область применения двойного двоеточия.</p>

<h3>Обращение снаружи класса к сущностям внутри него</h3>

<p>В коде поисковой системы есть статический метод для вычисления среднего рейтинга:</p>

<pre><code class="language-cpp">class SearchServer {
public:
    // публичные методы
    
private:
    // приватные поля, структуры и методы
    
    static int ComputeAverageRating(const vector&lt;int&gt;&amp; ratings) {
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast&lt;int&gt;(ratings.size());
    }
};
</code></pre>

<p>Если захотим сделать метод публичным, поставим двойное двоеточие и обратимся к нему снаружи класса <code>SearchServer</code>:</p>

<pre><code class="language-cpp">const vector&lt;int&gt; ratings = {9, -2, 6, 1, 18};
cout &lt;&lt; SearchServer::ComputeAverageRating(ratings) &lt;&lt; endl;
</code></pre>

<p>Статический метод не требует для работы объект типа <code>SearchServer</code>. Поэтому метод вызывается не через точку для такого объекта, а как обычная функция. Её название состоит из названия класса, двух двоеточий и названия метода. Писать <code>ComputeAverageRating</code> без названия класса можно только внутри самого класса.</p>

<p>Двойное двоеточие позволяет обратиться снаружи класса по имени к любой сущности, объявленной внутри класса: к полю, методу или типу. Это пригодится не только при вызове статических методов. Далее в курсе узнаете, где именно.</p>

<h3>Пространство имён</h3>

<p>Применение двойного двоеточия часто связано со знакомым вам <code>using namespace std</code>. Здесь <code>std</code> — название пространства имён. Если не написать в начале программы <code>using namespace std</code>, все имена из этого пространства имён придётся употреблять с префиксом <code>std::</code>.</p>

<pre><code class="language-cpp">#include &lt;iostream&gt;
#include &lt;vector&gt;

int main() {
    const std::vector&lt;int&gt; ratings = {9, -2, 6, 1, 18};
    std::cout &lt;&lt; ratings.size() &lt;&lt; std::endl;
}
</code></pre>

<p>В реальных программах <code>using namespace std</code> пишут редко — у этой директивы есть недостатки. Позже вы узнаете о них, сделаете выводы и начнёте использовать <code>std::</code> в своём коде.</p>
</details>

<details>  
<summary>Задание:</summary>
<p>Научитесь работать с перечислимым типом, объявленным внутри класса. Для этого перенесите объявление <code>DocumentStatus</code> в публичную секцию класса <code>SearchServer</code>, а в функции <code>main</code> выведите численное значение статуса <code>BANNED</code>.</p>

<h3>Подсказка</h3>

<p>Используйте двойное двоеточие дважды: <code>SearchServer::DocumentStatus::BANNED</code>. Не забудьте о <code>static_cast</code>.</p>
</details>  
