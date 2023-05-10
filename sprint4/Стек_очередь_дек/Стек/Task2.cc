#include <iostream>
#include <set>
#include <numeric>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
    public:
        void Push(const Type& element) {
            elements_.push_back(element);
        }
        void Pop() {
            elements_.pop_back();
        }
        const Type& Peek() const {
            return elements_.back();    
        }
        
        Type& Peek() {
            return elements_.back();
        }
        
        void Print() const {
            PrintRange(elements_.begin(), elements_.end());
        }
        
        uint64_t Size() const {
            return elements_.size();
        }

        bool IsEmpty() const {
            return elements_.empty();
        }

    private:
        vector<Type> elements_;
};


template <typename Type>
struct PairElemMin {
    Type element;
    Type minimum;
};

template <typename Type>
ostream& operator<<(ostream& out, const PairElemMin<Type>& pairElemMin) {
    out << pairElemMin.element;
    return out;
}

template <typename Type>
class StackMin {
    public:
        void Push(const Type& element) {
            PairElemMin<Type> new_pair = {element, element};
            if (!elements_.IsEmpty() && new_pair.minimum > elements_.Peek().minimum) {
                new_pair.minimum = elements_.Peek().minimum;
            }
            elements_.Push(new_pair);
        }

        void Pop() {
            elements_.Pop();
        }

        const Type& Peek() const {
            return elements_.Peek().element;
        }

        Type& Peek() {
            return elements_.Peek().element;
        }

        void Print() const {
            elements_.Print();
        }

        uint64_t Size() const {
            return elements_.size();
        }

        bool IsEmpty() const {
            return elements_.IsEmpty();
        }

        const Type& PeekMin() const {
            return elements_.Peek().minimum;
        }

        Type& PeekMin() {
            return elements_.Peek().minimum;
        }
        
    private:
        Stack<PairElemMin<Type>> elements_;
};


int main() {
    StackMin<int> stack;
    vector<int> values(5);
    iota(values.begin(), values.end(), 1);
    random_shuffle(values.begin(), values.end());

    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }

    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }
}
