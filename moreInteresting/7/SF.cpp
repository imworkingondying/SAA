#include "ShannonFano.h"
#include <algorithm>
using namespace std;

ShannonFano::Node* ShannonFano::buildTree(vector<Node*>& nodes)
{
    while (nodes.size() > size_t(1))
    {
        sort(nodes.begin(), nodes.end(),
            [](const Node* node1, const Node* node2)
            {
                return node1->freq > node2->freq;
            });
        Node* left = nodes.back();
        nodes.pop_back();
        Node* right = nodes.back();
        nodes.pop_back();
        nodes.push_back(new Node('\0', left->freq + right->freq, left, right));
    }
    return nodes.front();
}

void ShannonFano::generateCodes(Node* root, string code, unordered_map<char, string>& codes)
{
    if (root == nullptr) { return; }
    if (root->data != '\0') { codes[root->data] = code; }
    generateCodes(root->left, code + '0', codes);
    generateCodes(root->right, code + '1', codes);
}

string ShannonFano::encode(const string& data)
{
    unordered_map<char, int> freq;
    for (const char& ch : data) { freq[ch]++; }

    vector<Node*> nodes;
    for (const pair<char, int>& pair : freq) { nodes.push_back(new Node(pair.first, pair.second, nullptr, nullptr)); }
    Node* root = buildTree(nodes);

    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    string result = "";
    for (const char& ch : data) { result += codes[ch]; }
    return result;
}

string ShannonFano::decode(Node* root, const string& encodedData)
{
    string decodedData = "";
    Node* current = root;

    for (char bit : encodedData)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else if (bit == '1')
        {
            current = current->right;
        }

        if (current->data != '\0')
        {
            decodedData += current->data;
            current = root;
        }
    }

    return decodedData;
}

#include <iostream>

void ShannonFano::printEncoded(const string& data)
{
    cout << "Encoded: " << ShannonFano::encode(data) << endl;
}

void ShannonFano::printDecoded(Node* root, const string& encodedData)
{
    cout << "Decoded: " << ShannonFano::decode(root, encodedData) << endl;
}