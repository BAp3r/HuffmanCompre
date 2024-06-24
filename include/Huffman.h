#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>
#include <unordered_map>

class Huffman {
public:
    void compress(const std::vector<std::string>& inputFiles, const std::string& outputFile);
    void printStatistics() const;

private:
    void buildTree(const std::string& data);
    void generateCodes();
    void writeCompressedFile(const std::string& data, const std::string& outputFile);
    void calculateFrequencies(const std::string& data);
    void encode(Node* root, const std::string& str);

    struct Node {
        char ch;
        int freq;
        Node* left;
        Node* right;
        Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    };

    struct Compare {
        bool operator()(Node* l, Node* r) {
            return l->freq > r->freq;
        }
    };

    Node* root;
    std::unordered_map<char, std::string> huffmanCode;
    std::unordered_map<char, int> frequencies;
    double compressionRatio;
    double compressionTime;
};

#endif // HUFFMAN_H
