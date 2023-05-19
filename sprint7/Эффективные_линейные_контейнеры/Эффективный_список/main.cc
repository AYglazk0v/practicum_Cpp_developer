#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

class Editor {
public:
    using Iterator = std::list<char>::iterator;
    
    Editor() = default;

    void Left() {
        it_ = Shift(it_, -1);
    }
    void Right(){
        it_ = Shift(it_, 1);
    }
    
    void Insert(char token) {
        text_.insert(it_, token);
    }
    void Cut(size_t tokens = 1){
        auto it_end_cut_pos = Shift(it_, tokens);
        buff_.assign(it_ ,it_end_cut_pos);
        it_ = text_.erase(it_,it_end_cut_pos);
    }
    
    void Copy(size_t tokens = 1) {
        auto it_end_cut_pos = Shift(it_, tokens);
        buff_.assign(it_ ,it_end_cut_pos);
    }
    
    void Paste() {
        text_.insert(it_, buff_.begin(), buff_.end());
    }
    
    string GetText() const {
        return {text_.begin(), text_.end()};
    }

private:
    Iterator Shift(Iterator it, int steps) {
        while (steps > 0 && it != text_.end()) {
            ++it;
            --steps;
        }
        while (steps < 0 && it != text_.begin()) {
            --it;
            ++steps;
        }
        return it;
    }

private:
    std::list<char>              text_;
    std::list<char>              buff_;
    Iterator                     it_ = text_.end();
};

int main() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // // Текущее состояние редактора: `|world`
    // // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}
