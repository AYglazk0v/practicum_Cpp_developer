#pragma once

#include <istream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class Node {
public:
    explicit Node(std::vector<Node> array);
    explicit Node(std::map<std::string, Node> map);
    explicit Node(int value);
    explicit Node(std::string value);

    const std::vector<Node>& AsArray() const;
    const std::map<std::string, Node>& AsMap() const;
    int AsInt() const;
    const std::string& AsString() const;

private:
    std::vector<Node> as_array_;
    std::map<std::string, Node> as_map_;
    int as_int_;
    std::string as_string_;
};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

Document Load(std::istream& input);