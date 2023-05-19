vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;
    const int64_t pos_end = str.npos;
    while (true) {
        int64_t space = str.find(' ');
        result.push_back(space == pos_end ? str.substr() : str.substr(0, space));
        if (space == pos_end) {
            break;
        } else {
            str = str.substr(space + 1);
        }
    }
    return result;
}