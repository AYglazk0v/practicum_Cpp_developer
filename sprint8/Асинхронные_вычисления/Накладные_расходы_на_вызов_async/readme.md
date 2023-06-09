<details>
<summary>Теория:</summary>

# Накладные расходы на вызов async

Вы уже могли заметить, что использовать параллельные версии стандартных алгоритмов обычно проще, чем искусственно разбивать задачу на подзадачи и вызывать пачку  `async`. Параллельные версии действительно предпочтительнее. Авторы стандартной библиотеки позаботились о разработчиках и реализовали параллельные версии так, чтобы ядра процессора использовались максимально эффективно.

С  `async`  в этом плане дела обстоят хуже. Каждый его вызов говорит: «У меня есть вот такая задача, постарайся запустить её асинхронно». Если свободные ядра есть, так и будет. Если нет — процессор потратит дополнительные ресурсы на то, чтобы выполнить задачу на существующих ядрах.

Поэтому лучше всего  `async`  помогает в распараллеливании, когда количество задач близко к количеству ядер. Если же бездумно порождать всё новые и новые задачи, программа может стать чудовищно неэффективной. Этот эффект вы прочувствуете, когда будете выполнять задание.

</details>

<details>
<summary>Задание:</summary>

## Задание

Вы уже реализовывали сортировку слиянием: простой алгоритм, когда массив разбивается на две половины, каждая из них сортируется, а затем алгоритм  `merge`  сливает их, сохраняя нужный для сортировки порядок.

Ускорьте её, использовав многопоточность. В этой задаче не требуется реализовывать две версии, которые активируются использованием разных ExecutionPolicy. Достаточно ускорить данный код.

### Ограничения

Гарантируется, что передаваемые в функцию итераторы — это итераторы произвольного доступа.

Ваш код не должен содержать слово  `sort`, но должен содержать  `async`.

### Что отправлять на проверку

cpp-файл, содержащий шаблонную функцию  `MergeSort`  с тем же интерфейсом, что в заготовке кода, а также нужные для её работы подключения библиотек, другие классы и функции. Функция  `main`  будет проигнорирована при тестировании.

### Как будет тестироваться ваш код

Правильность работы функции будет проверена юнит-тестами.

Также будет измерено время её работы. Для достаточно больших массивов оно должно быть по крайней мере на 60% меньше, чем у функции из заготовки кода.

Мы проверим это на массиве из 4 000 000 целых чисел.

### Подсказка

Примените  `async`  для одновременного запуска сортировки половин диапазона. Но помните, что большое количество запусков  `async`  негативно сказывается на производительности.

</details>
