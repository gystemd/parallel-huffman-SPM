
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

#include "unordered_map"

#ifndef HUFFMAN_BASE_H
#define HUFFMAN_BASE_H

typedef std::vector<std::vector<std::vector<bool>*>*> encoded_t;

class huffman_base {
  struct huffman_node {
    char data;
    int frequency;
    huffman_node *left, *right;

    huffman_node(char data, int frequency)
        : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
  };

 protected:
  std::string input_file, output_file, text, decoded;
  encoded_t* encoded;
  std::unordered_map<char, int> freq;
  huffman_node* root;
  std::unordered_map<char, std::vector<bool>*> codes;

 public:
  huffman_base(std::string input_file, std::string output_file)
      : input_file(input_file), output_file(output_file) {}

  huffman_node* build_tree();
  std::string read_file();
  std::unordered_map<char, std::vector<bool>*> build_codes();
  std::string decode();
  void run();
  void write_file();

  virtual std::unordered_map<char, int> count_frequency() = 0;
  virtual encoded_t* encode_string() = 0;

  virtual ~huffman_base() {
    if (root) {
      std::function<void(huffman_node*)> delete_tree = [&](huffman_node* node) {
        if (!node) return;
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
      };
      delete_tree(root);
    }
    if (encoded) {
      for (auto& v : *encoded) {
        for (auto& code : *v) delete code;
        delete v;
      }
      delete encoded;
    }
  }
};

#endif  // HUFFMAN_BASE_H