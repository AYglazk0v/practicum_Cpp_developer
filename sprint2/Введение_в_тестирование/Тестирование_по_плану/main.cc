#include <iostream>
#include <string>

using namespace std;

bool IsPalindrome(const string& text) {
    if (text.empty()) {
        return false;
    }
    for (int l = 0, r = text.size()-1 ; l < r; ++l, --r) {
        while( l < r && !isalnum(text[l]))
            l++;
        while (l < r && !isalnum(text[r]))
            r--;
        if ((!isalnum(text[l]) && !isalnum(text[r])) || tolower(text[l]) != tolower(text[r]))
            return false;   
    }
    return true;
}

int main() {
    string text;
    getline(cin, text);

    /*
        racecar -- true
        a man a plan a canal panama -- true
        Able was I ere I saw Elba -- true

        @#$%@#$% -- false
        not a palindrome -- false
        "" -- false
        "       " -- false
    */

    if (IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    } else {
        cout << "not a palindrome"s << endl;
    }
}