#include <algorithm>
#include <iostream>
#include <type_traits>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container, typename Iterator>
requires random_access_iterator<Iterator>
void EraseAndPrint(Container& container, Iterator it) {
    PrintRange(container.begin(), it);
    if (it != container.end()) {
        PrintRange(std::next(it), container.end());
        container.erase(it);
    } else {
        PrintRange(it, it);
    }
}

template <typename Container>
void EraseAndPrint(Container& container, size_t pos) {
    EraseAndPrint(container, container.begin() + pos);
}

template <typename Container>
void EraseAndPrint(Container& container, size_t pos, size_t start_pos, size_t stop_pos) {
    EraseAndPrint(container, pos);
    if (container.begin() + stop_pos < container.end()) {
        PrintRange(container.begin() + stop_pos, container.end());
        container.erase(container.begin() + start_pos, container.begin() + stop_pos);
    }
}


int main() {
    {
        std::cout << "___________________________________\n"s;
        vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
        std::cout << "- EraseAndPrint:\n"s;
        EraseAndPrint(langs, langs.begin());
    }
    {
        std::cout << "___________________________________\n"s;
        vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
        //Проверка на it = end();
        std::cout << "- Проверка, если it == container.end():\n"s;
        EraseAndPrint(langs, langs.end());

        //Вывод изначального контейнера
        std::cout << "- Изначальный контейнер:\n"s;
        PrintRange(langs.begin(), langs.end());

        //Удаление С#
        std::cout << "EraseAndPrint:\n"s;
        EraseAndPrint(langs, std::find(langs.begin(), langs.end(), "C#"s));

        //Проверка, что мы удалили C#
        std::cout << "- Контейнер после удаления элементов:\n"s;
        PrintRange(langs.begin(), langs.end());
    }
    {
        std::cout << "___________________________________\n"s;
        vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
        std::cout << "- EraseAndPrint:\n"s;
        EraseAndPrint(langs, 2);
        
        //Проверка, что мы удалили C#
        std::cout << "- Контейнер после удаления элементов:\n"s;
        PrintRange(langs.begin(), langs.end());
    }
    {
        std::cout << "___________________________________\n"s;
        vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
        std::cout << "- EraseAndPrint:\n"s;
        EraseAndPrint(langs, 0, 0, 2);
        
        std::cout << "- Контейнер после удаления элементов:\n"s;
        PrintRange(langs.begin(), langs.end());
    }
    return 0;
}