#include "ini.h"

#include <iostream>

using namespace std;

namespace {
    std::string trimWhiteSpace(std::string& s, size_t pos_start, size_t pos_end) {
        auto pos_start_trim = s.find_first_not_of(' ', pos_start);
        auto result_str = s.substr(pos_start_trim, pos_end - pos_start_trim);
        auto trim_last_space = result_str.find_last_not_of(' ');
        if (trim_last_space != s.npos) {
            return result_str.substr(0, trim_last_space + 1);
        }
        return result_str;
    }
}

namespace ini {
    Section& Document::AddSection(std::string name) {
        return sections_[name];
    }

    const Section& Document::GetSection(const std::string& name) const {
        static Section empty = {};
        if (sections_.contains(name)) {
            return sections_.at(name);
        }
        return empty;
    }

    std::size_t Document::GetSectionCount() const {
        return sections_.size();
    }

    Document Load(std::istream& input) {
        Document    result_doc;
        Section*    current_section = nullptr;
        for (std::string str; getline(input, str);) {
            auto pos_first_not_space = str.find_first_not_of(' ');
            if (pos_first_not_space != str.npos
                && str[pos_first_not_space] == '[') {
                current_section = &result_doc.AddSection(str.substr(str.find('[') + 1, str.rfind(']') - 1));
            } else if (pos_first_not_space != str.npos) {
                auto pos_assign = str.find('=');
                auto key = trimWhiteSpace(str, pos_first_not_space, pos_assign); 
                auto value = trimWhiteSpace(str, pos_assign + 1, str.size());
                if (current_section != nullptr) {
                    current_section->insert({std::move(key), std::move(value)});
                }
            }
        }
        return result_doc;
    }
}