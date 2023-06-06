#include <iostream>
#include <string>
#include <unordered_map>

// поместите все имена библиотеки INI в пространство имён ini

namespace ini {

using Section = std::unordered_map<std::string, std::string>;

class Document {
public:
    // реализация методов должна быть в файле ini.cpp
    Section& AddSection(std::string name);
    const Section& GetSection(const std::string& name) const;
    std::size_t GetSectionCount() const;

private:
    std::unordered_map<std::string, Section> sections_;
};

// определение этой функции должно быть в файле ini.cpp
Document Load(std::istream& input);

} //namespace ini