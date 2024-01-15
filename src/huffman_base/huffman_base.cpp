#include "huffman_base.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

#include "../utimer.cpp"
#include "bitset"
#include "unordered_map"

std::string huffman_base::read_file() {
  std::ifstream file(input_file);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

huffman_base::huffman_node *huffman_base::build_tree() {
  struct Compare {
    bool operator()(huffman_node *left, huffman_node *right) {
      return left->frequency > right->frequency;
    }
  };

  std::priority_queue<huffman_node *, std::vector<huffman_node *>, Compare> pq;
  for (auto entry : freq) pq.push(new huffman_node(entry.first, entry.second));

  while (pq.size() > 1) {
    huffman_node *left = pq.top();
    pq.pop();
    huffman_node *right = pq.top();
    pq.pop();
    huffman_node *parent =
        new huffman_node('\0', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }
  return pq.top();
}

std::unordered_map<char, std::vector<bool> *> huffman_base::build_codes() {
  std::unordered_map<char, std::vector<bool> *> codes;
  std::function<void(huffman_node *, std::vector<bool> *)> build_codes =
      [&](huffman_node *node, std::vector<bool> *code) {
        if (node->left == nullptr && node->right == nullptr) {
          codes[node->data] = code;
          return;
        }
        std::vector<bool> *left_code = new std::vector<bool>(*code);
        left_code->push_back(false);
        build_codes(node->left, left_code);
        std::vector<bool> *right_code = new std::vector<bool>(*code);
        right_code->push_back(true);
        build_codes(node->right, right_code);
      };
  std::vector<bool> *code = new std::vector<bool>();
  build_codes(root, code);
  return codes;
}

void huffman_base::run() {
  {
    utimer timer("read", &read_time);
    this->text = read_file();
  }

  {
    utimer timer("count frequency", &frequency_time);
    this->freq = count_frequency();
  }

  {
    utimer timer("build tree", &tree_time);
    this->root = build_tree();
  }

  {
    utimer timer("build codes", &code_time);
    this->codes = build_codes();
  }

  {
    utimer timer("encode", &encode_time);
    this->encoded = encode_string();
  }

  // std::string decoded = decode();
  // if (decoded == text) {
  //   std::cout << "Decoded text is  the same as the original text." <<
  //   std::endl;
  // } else {
  //   return;
  // }

  {
    utimer timer("write", &write_time);
    write_file();
  }

  std::cout << "read: " << read_time << std::endl;
  std::cout << "frequency: " << frequency_time << std::endl;
  std::cout << "tree: " << tree_time << std::endl;
  std::cout << "code: " << code_time << std::endl;
  std::cout << "encode: " << encode_time << std::endl;
  std::cout << "write: " << write_time << std::endl;
  write_benchmark();
}

void huffman_base::write_file() {
  std::ofstream file(output_file, std::ios::out | std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open the output file." << std::endl;
    return;
  }

  for (size_t i = 0; i < encoded->size(); i += 8) {
    for (size_t j = 0; j < encoded->at(i)->size(); j++) {
      std::vector<bool> bits = *encoded->at(i)->at(j);
      std::bitset<8> bitset;
      for (size_t k = 0; k < 8; k++) bitset[k] = bits[k];
      unsigned char byte = bitset.to_ulong();
      file.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
    }
  }

  file.close();
}

std::string huffman_base::decode() {
  std::string decoded = "";
  huffman_node *node = root;
  for (size_t i = 0; i < encoded->size(); i++) {
    for (size_t j = 0; j < encoded->at(i)->size(); j++) {
      std::vector<bool> *bits = encoded->at(i)->at(j);
      for (size_t k = 0; k < bits->size(); k++) {
        if (bits->at(k)) {
          node = node->right;
        } else {
          node = node->left;
        }
        if (node->left == nullptr && node->right == nullptr) {
          decoded += node->data;
          node = root;
        }
      }
    }
  }
  return decoded;
}

void huffman_base::write_benchmark() {
  std::ofstream file(benchmark_file, std::ios::out | std::ios::app);
  if (!file) {
    std::cerr << "Cannot open the benchmark file." << std::endl;
    return;
  }
  file << read_time << "," << frequency_time << "," << tree_time << ","
       << code_time << "," << encode_time << "," << write_time << std::endl;
  file.close();
}