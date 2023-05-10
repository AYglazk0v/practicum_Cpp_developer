#include <stack>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

template <typename Type>
class Queue {
public:
    void Push(const Type& element) {
       stack1_.push(element);
    }
    void Pop() {
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
        } else {
            stack2_.pop();
        }
    }

    Type& Front() {
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
            return stack2_.top();
        }
        return stack2_.top();
    }

    const Type& Front() const {
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
            return stack2_.top();
        }
        return stack2_.top();
    }
    
    uint64_t Size() const {
        return stack1_.size() + stack2_.size();
    }
    bool IsEmpty() const {
        return stack1_.empty() && stack2_.empty();
    }

private:
    stack<Type> stack1_;
    stack<Type> stack2_;
};

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

int main() {
    Queue<int> queue;
    vector<int> values {5, 2, 4, 3, 1};

    // // заполняем вектор для тестирования очереди
    // iota(values.begin(), values.end(), 1);
    // // перемешиваем значения
    // random_shuffle(values.begin(), values.end());

    PrintRange(values.begin(), values.end());

    cout << "Заполняем очередь"s << endl;

    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        cout << "Вставленный элемент "s << values[i] << endl;
        cout << "Первый элемент очереди "s << queue.Front() << endl;
    }

    cout << "Вынимаем элементы из очереди"s << endl;

    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        cout << "Будем вынимать элемент "s << queue.Front() << endl;
        queue.Pop();
    }
    return 0;
}