<details>  
<summary>Введение:</summary>

# Перечислимые типы

На целочисленных типах основаны перечислимые, или перечисляемые, — тип данных с конечным числом упорядоченных именованных значений.

Перечислимый тип объявляют ключевым словом  `enum`. После ключевого слова пишут идентификатор перечисления, затем перечислители. В зависимости от позиции в наборе перечислителям присваиваются целочисленные значения:

```cpp
enum CatBreed {
  RUSSIAN_BLUE,        //присваивается значение 0
  MAINE_COON,         //присваивается значение 1
  BRITISH_SHORTHAIR, //присваивается значение 2
  SIBERIAN          //присваивается значение 3
};

enum DogBreed {
  SIBERIAN_HUSKY,
  GOLDEN_RETRIEVER
  POMERANIAN,
  SAMOYED
};

```

Переменные этого типа принимают значения из заранее известного ограниченного набора. Перечислители и перечисление расположены в едином пространстве имён. Поэтому перечислители в разных наборах не должны повторяться. Например, сибирскую кошку и сибирскую хаски нельзя назвать  `SIBERIAN`  одновременно. Но  `SIBERIAN`  и  `SIBERIAN_HUSKY`  — разные перечислители, и проблем не возникает.

В перечислениях бывают иные проблемы и связаны они с преобразованием типов. При логических и арифметических операциях перечислимые типы неявно преобразуются к целочисленным с автоматически присвоенным значением. То есть значение русской голубой кошки и сибирской хаски будет 0, а значение мейн-куна и золотистого ретривера — 1.

Чтобы избежать неприятностей с  `enum`, в C++11 появился более удобный способ перечисления:  `enum class`. Каждый  `enum class`  считается уникальным типом. Поэтому компилятор не будет проводить операции с перечислителями из разных наборов и выдаст ошибку.

![1.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint1/%D0%9F%D0%B5%D1%80%D0%B5%D1%87%D0%B8%D1%81%D0%BB%D0%B8%D0%BC%D1%8B%D0%B5_%D1%82%D0%B8%D0%BF%D1%8B/image/1.png?raw=true)

Посмотрим, как это работает на примере. Напишем функцию, которая обрабатывает запросы ко множеству:

```cpp
void ProcessRequest(
    set<int>& numbers,
    /* ??? */ request_type,
    int request_data) {
  if (/* запрос на добавление */) {
    numbers.insert(request_data);
  } else if (/* запрос на удаление */) {
    numbers.erase(request_data);
  } else if (/* запрос на умножение на -1 */) {
    if (numbers.count(request_data) == 1) {
      numbers.erase(request_data);
      numbers.insert(-request_data);
    }
  }
}

ProcessRequest(numbers, /* добавить */, 8);
ProcessRequest(numbers, /* умножить на -1 */, 8);
ProcessRequest(numbers, /* удалить */, -8);

```

Выберем оптимальный тип параметра  `request_type`. Рассмотрим несколько вариантов.

### request_type — строка

```cpp
void ProcessRequest(
		    set<int>& numbers,
		    const string& request_type,
		    int request_data) {
	  if (request_type == "ADD"s) {
		    numbers.insert(request_data);
	  } else if (request_type == "REMOVE"s) {
		    numbers.erase(request_data);
	  } else if (request_type == "NEGATE"s) {
		    if (numbers.count(request_data) == 1) {
			      numbers.erase(request_data);
			      numbers.insert(-request_data);
		    }
	  }
}

ProcessRequest(numbers, "ADD"s, 8);
ProcessRequest(numbers, "NEGATE"s, 8);
ProcessRequest(numbers, "REMOVE"s, -8);

```

Плюсы:

-   Код легко читать.

Минусы:

-   Тип  `string`  предназначен для произвольных строк, а мы храним в нём заранее известные. То есть тратим лишнюю память на хранение наборов символов и лишнее время на сравнение строк в функции  `ProcessRequest`.
-   При вызове функции легко сделать опечатку в типе запроса. Тогда вместо ошибки компиляции запрос проигнорируется.

### request_type — число

```cpp
void ProcessRequest(
		    set<int>& numbers,
		    int request_type,
		    int request_data) {
	  if (request_type == 0) {
		    numbers.insert(request_data);
	  } else if (request_type == 1) {
		    numbers.erase(request_data);
	  } else if (request_type == 2) {
		    if (numbers.count(request_data) == 1) {
			      numbers.erase(request_data);
			      numbers.insert(-request_data);
		    }
	  }
}

ProcessRequest(numbers, 0, 8);
ProcessRequest(numbers, 2, 8);
ProcessRequest(numbers, 1, -8);

```

Плюсы:

-   Эффективнее, чем вариант со строкой.

Минусы:

-   Без комментариев непонятно, что такое  `0`,  `1`  и  `2`  во втором аргументе функции  `ProcessRequest`.

Код будет понятнее, если объявить именованные константы для различных типов запросов:

```cpp
const int REQUEST_ADD = 0;
const int REQUEST_REMOVE = 1;
const int REQUEST_NEGATE = 2;

void ProcessRequest(
		    set<int>& numbers,
		    int request_type,
		    int request_data) {
	  if (request_type == REQUEST_ADD) {
		    numbers.insert(request_data);
	  } else if (request_type == REQUEST_REMOVE) {
		    numbers.erase(request_data);
	  } else if (request_type == REQUEST_NEGATE) {
		    if (numbers.count(request_data) == 1) {
			      numbers.erase(request_data);
			      numbers.insert(-request_data);
		    }
	  }
}

ProcessRequest(numbers, REQUEST_ADD, 8);
ProcessRequest(numbers, REQUEST_NEGATE, 8);
ProcessRequest(numbers, REQUEST_REMOVE, -8);

```

В константы мы добавили префикс REQUEST_, чтобы не занимать популярные названия ADD, REMOVE и NEGATE. Так, ADD и NEGATE могут быть названиями арифметических операций.

Плюсы:

-   Код с константами понятнее.

Минусы:

-   При вызове функции  `ProcessRequest`  легко сделать опечатку и перепутать второй и третий аргументы: передать тип запроса в качестве значения и наоборот. В этом случае вместо ошибки компиляции получим обработку другого запроса, возможно, формально корректного. Ошибки такого рода тяжело искать.
-   Компилятор не станет препятствовать использованию чисел в явном виде вместо констант. Вызов  `ProcessRequest(numbers, 0, 8)`  он будет считать корректным.
-   Константу — REQUEST_REMOVE например, — можно умножить на два как обычное число.

Чтобы избежать этих проблем, попробуем сделать параметр  `request_type`  отдельным перечислимым типом данных.

### request_type — enum class

Объявим перечислимый тип  `RequestType`  с тремя возможными значениями: ADD, REMOVE, NEGATE:

```cpp
enum class RequestType {
	  ADD,
	  REMOVE,
	  NEGATE
};

void ProcessRequest(
		    set<int>& numbers,
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


```

-   Тип  `RequestType`  объявлен ключевыми словами  `enum class`. В фигурных скобках — идентификаторы возможных значений типа.
-   `RequestType`  может быть типом переменной или типом параметра функции.
-   Перед значением типа стоит префикс  `RequestType::`. Вызов функции  `ProcessRequest(numbers, ADD, 8)`  не скомпилируется, имя ADD останется свободным.

`RequestType`  и целочисленные типы неявно преобразовываться друг к другу не будут. Следовательно, вызов функции  `ProcessRequest(numbers, 8, RequestType::ADD)`  с перепутанными аргументами не скомпилируется.

Значения одного перечислимого типа — например  `RequestType`, — сравнивают друг с другом операторами  `==`,  `!=`,  `<`  и  `>`. Значения перечислимых типов могут быть элементами множеств или ключами словарей. Порядок между значениями соответствует порядку их определения при объявлении типа. В нашем примере  `RequestType::ADD < RequestType::REMOVE`  и  `RequestType::REMOVE < RequestType::NEGATE`.
</details>  



<details>  
<summary>Задание 1:</summary>
<p>Расширим функционал вашей поисковой системы: добавим перечислимые типы и статусы документов. Каждый документ при добавлении будет помечаться как актуальный (ACTUAL), устаревший (IRRELEVANT), отклонённый (BANNED) или удалённый (REMOVED).</p>

<p>Начиная с этой задачи вам больше не придётся отправлять на проверку функцию <code>main</code>, реализовывать ввод и вывод данных. Важно, чтобы класс <code>SearchServer</code> и вспомогательные структуры и функции удовлетворяли всем условиям.</p>

<ul>
<li>Объявите перечислимый тип <code>DocumentStatus</code> со значениями ACTUAL, IRRELEVANT, BANNED, REMOVED.</li>
<li>Добавьте статус документа третьим параметром метода <code>AddDocument</code>. Итого параметры будут следующими: <code>int document_id, const string&amp; document, DocumentStatus status, const vector&lt;int&gt;&amp; ratings</code>. Этот статус сохраните для документа по его <code>document_id</code>.</li>
<li>Добавьте статус вторым параметром метода <code>FindTopDocuments</code>. Параметры теперь будут такими: <code>const string&amp; raw_query, DocumentStatus status</code>. Метод должен возвращать пять лучших документов, отсортированных по релевантности, и выбирать только среди документов с указанным статусом. Фильтрация по статусу пусть происходит до отсечения лучших документов. Возвращать статусы найденных документов не нужно.</li>
</ul>

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
    search_server.AddDocument(3, &quot;ухоженный скворец евгений&quot;s,         DocumentStatus::BANNED, {9});

    cout &lt;&lt; &quot;ACTUAL:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s, DocumentStatus::ACTUAL)) {
        PrintDocument(document);
    }

    cout &lt;&lt; &quot;BANNED:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }

    return 0;
}
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>ACTUAL:
{ document_id = 1, relevance = 0.866434, rating = 5 }
{ document_id = 0, relevance = 0.173287, rating = 2 }
{ document_id = 2, relevance = 0.173287, rating = -1 }
BANNED:
{ document_id = 3, relevance = 0.231049, rating = 9 }
</code></pre>

<h3>Подсказка</h3>

<p>Создайте отдельную структуру для хранения внутренних данных о документе и сложите в неё рейтинг и статус. Объявить её можно в приватной части класса поисковой системы.</p>

</details> 

<details>  
<summary>Задание 2:</summary> 
<p>Большинство людей интересуется только актуальной информацией. Поэтому неясно, зачем просить пользователя поисковой системы каждый раз передавать статус. Научите метод <code>FindTopDocuments</code> обходиться одним аргументом — поисковым запросом. То есть поиск должен производиться только по актуальным документам.</p>

<p>Этого можно добиться, указав для параметра-статуса значение по умолчанию:</p>

<pre><code class="language-cpp">vector&lt;Document&gt; FindTopDocuments(
				const string&amp; raw_query, DocumentStatus status = DocumentStatus::ACTUAL
) const {
		// реализация метода
}
</code></pre>

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
    search_server.AddDocument(3, &quot;ухоженный скворец евгений&quot;s,         DocumentStatus::BANNED, {9});

    cout &lt;&lt; &quot;ACTUAL:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s)) {
        PrintDocument(document);
    }

    cout &lt;&lt; &quot;BANNED:&quot;s &lt;&lt; endl;
    for (const Document&amp; document : search_server.FindTopDocuments(&quot;пушистый ухоженный кот&quot;s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }
}
</code></pre>

<p><strong>Вывод</strong></p>

<pre><code>ACTUAL:
{ document_id = 1, relevance = 0.866434, rating = 5 }
{ document_id = 0, relevance = 0.173287, rating = 2 }
{ document_id = 2, relevance = 0.173287, rating = -1 }
BANNED:
{ document_id = 3, relevance = 0.231049, rating = 9 }
</code></pre>

<h3>Подсказка</h3>

<p>Достаточно добавить <code>= DocumentStatus::ACTUAL</code> сразу после второго параметра метода <code>FindTopDocuments</code>.</p>

</details>  
