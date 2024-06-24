#include "Huffman.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <chrono>
#include <bitset>
#include <iomanip>

void Huffman::compress(const std::vector<std::string>& inputFiles, const std::string& outputFile) {
    std::string data;
    for (const auto& file : inputFiles) {
        std::ifstream inFile(file);
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        data += buffer.str();
    }

    auto start = std::chrono::high_resolution_clock::now();

    calculateFrequencies(data);
    buildTree(data);
    generateCodes();
    writeCompressedFile(data, outputFile);

    auto end = std::chrono::high_resolution_clock::now();
    compressionTime = std::chrono::duration<double>(end - start).count();

    std::ifstream originalFile(inputFiles[0], std::ios::binary | std::ios::ate);
    std::ifstream compressedFile(outputFile, std::ios::binary | std::ios::ate);
    double originalSize = originalFile.tellg();
    double compressedSize = compressedFile.tellg();

    compressionRatio = originalSize / compressedSize;
}

void Huffman::calculateFrequencies(const std::string& data) {
    for (char ch : data) {
        frequencies[ch]++;
    }
}

void Huffman::buildTree(const std::string& data) {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for (const auto& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        Node* node = new Node('\0', sum);
        node->left = left;
        node->right = right;
        pq.push(node);
    }

    root = pq.top();
}

void Huffman::generateCodes() {
    encode(root, "");
}

void Huffman::encode(Node* root, const std::string& str) {
    if (!root) return;

    if (root->left == nullptr && root->right == nullptr) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0");
    encode(root->right, str + "1");
}

void Huffman::writeCompressedFile(const std::string& data, const std::string& outputFile) {
    std::ofstream outFile(outputFile, std::ios::binary);
    
    std::string encodedString;
    for (char ch : data) {
        encodedString += huffmanCode[ch];
    }

    int extraBits = 8 - (encodedString.size() % 8);
    for (int i = 0; i < extraBits; ++i) {
        encodedString += "0";
    }

    outFile << std::bitset<8>(extraBits);

    for (size_t i = 0; i < encodedString.size(); i += 8) {
        std::bitset<8> byte(encodedString.substr(i, 8));
        outFile << static_cast<char>(byte.to_ulong());
    }

    outFile.close();
}

void Huffman::printStatistics() const {
    std::cout << "Compression time: " << compressionTime << " seconds" << std::endl;
    std::cout << "Compression ratio: " << compressionRatio << std::endl;
}
