
<details>  
<summary>Теория</summary>  
  <h1>Ранжирование по релевантности</h1>

  

<p>Эффективная поисковая система не только быстро ищет документы, но и показывает результаты упорядоченно — начиная с самых полезных. Полезность определяется таким параметром как релевантность документа.</p>

  

<p>Релевантность — то, насколько результат поиска соответствует ожиданиям пользователя, который ввёл поисковый запрос. Чем выше релевантность найденного документа, тем он полезнее.</p>
</details>  
  
<details>  
<summary>Задание 1:</summary>  
  <p>Пусть ваша поисковая система возвращает не любые документы, а документы с самой высокой релевантностью. Напишите функцию <code>FindTopDocuments</code>, которая должна находить только <code>MAX_RESULT_DOCUMENT_COUNT = 5</code> результатов. Объявите эту глобальную константу вверху своего решения. Функцию <code>FindTopDocuments</code> поместите там же, где для вывода итогового результат мы вызывали <code>FindDocuments</code>. Саму <code>FindDocuments</code> замените на функцию <code>FindAllDocuments</code>, которая будет возвращать все результаты поиска в виде пар из идентификатора документа и релевантности <code>{document_id, relevance}</code>. Отсортируйте результат <code>FindAllDocuments</code> по убыванию, чтобы использовать его в <code>FindTopDocuments</code>.</p>

  

<h3>Примеры</h3>

  

<p>Ввод</p>

  

<pre><code>a the on cat
6
a fat cat sat on a mat and ate a fat rat
fat rat
a fat cat rat
a fat cat sat
a fat cat
a fat dog
funny fat cat on a mat with rat
</code></pre>

  

<p>Вывод</p>

  

<pre><code>{ document_id = 0, relevance = 3 }
{ document_id = 2, relevance = 2 }
{ document_id = 1, relevance = 2 }
{ document_id = 5, relevance = 1 }
{ document_id = 4, relevance = 1 }

</code></pre>

  

<h3>Подсказка</h3>

  

<p>Вспомните, что должна возвращать функция <code>FindAllDocuments</code>, чтобы сортировка по убыванию поставила наверх документы с наибольшей релевантностью.</p>
  
</details>
