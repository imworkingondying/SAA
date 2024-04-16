#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace ShannonFano
{
    struct Node
    {
        char data;
        int freq;
        Node* left;
        Node* right;

        Node() : data('\0'), freq(-1), left(nullptr), right(nullptr) {}
        Node(char data, int freq, Node* left = nullptr, Node* right = nullptr) : data(data), freq(freq), left(left), right(right) {}
    };

    Node* buildTree(std::vector<Node*>& nodes);
    void generateCodes(Node* root, std::string code, std::unordered_map<char, std::string>& codes);
    std::string encode(const std::string& data);
    std::string decode(Node* root, const std::string& encodedData);
    void printEncoded(const std::string& data);
    void printDecoded(Node* root, const std::string& encodedData);
}