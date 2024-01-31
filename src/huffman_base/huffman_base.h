
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

 private:
  std::string input_file, output_file, text, decoded;

  encoded_t* encoded;
  std::unordered_map<char, unsigned int> freq;
  huffman_node* root;
  std::unordered_map<char, std::vector<bool>*> codes;

  long read_time, frequency_time, tree_time, code_time, encode_time, write_time;
  long total_time, total_time_nio;

  std::string read_file(std::string input_file);
  virtual std::unordered_map<char, unsigned int> count_frequency(
      std::string &text) = 0;
  huffman_node* build_tree(std::unordered_map<char, unsigned int>& freq);
  std::unordered_map<char, std::vector<bool>*> build_codes(huffman_node* root);
  virtual encoded_t* encode_string(
      std::unordered_map<char, std::vector<bool>*>& codes, std::string &text) = 0;
  void write_file(encoded_t &encoded, std::string output_file);
  std::string decode(const std::vector<bool> &encoded, const huffman_base::huffman_node *root);

 protected:
  int num_threads;
  std::string benchmark_file;

 public:
  huffman_base(std::string input_file, std::string output_file)
      : input_file(input_file), output_file(output_file) {}

  void run();

  std::string decode_file(std::string input_file, const huffman_node *root);
  virtual ~huffman_base();

  void write_benchmark();
  huffman_node* get_root() { return root; }
  std::string get_text() { return text; }
};

#endif  // HUFFMAN_BASE_H