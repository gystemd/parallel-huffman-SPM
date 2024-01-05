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

std::unordered_map<char, std::string> huffman_base::build_codes() {
  std::unordered_map<char, std::string> codes;
  std::function<void(huffman_node *, std::string)> encode =
      [&](huffman_node *node, std::string code) {
        if (node == nullptr) return;
        if (node->left == nullptr && node->right == nullptr) {
          codes[node->data] = code;
        }
        encode(node->left, code + "0");
        encode(node->right, code + "1");
      };

  encode(root, "");
  return codes;
}


void huffman_base::run() {
  {
    long elapsed;
    utimer timer("read", &elapsed);
    this->text = read_file();
  }

  {
    long elapsed;
    utimer timer("count frequency", &elapsed);
    this->freq = count_frequency();
  }

  this->root = build_tree();
  this->codes = build_codes();

  {
    long elapsed;
    utimer timer("encode", &elapsed);
    this->encoded = encode_string();
  }

  write_file();
}

void huffman_base::write_file() {
  std::ofstream file(output_file, std::ios::out | std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open the output file." << std::endl;
    return;
  }

  for (size_t i = 0; i < encoded.size(); i += 8) {
    std::bitset<8> bits(encoded.substr(i, 8));
    unsigned char byte = bits.to_ulong();
    file.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
  }

  file.close();
}

std::string huffman_base::decode() {
  std::string decoded = "";
  huffman_node *current = root;
  for (char c : encoded) {
    current = c == '0' ? current->left : current->right;
    if (current->left == nullptr && current->right == nullptr) {
      decoded += current->data;
      current = root;
    }
  }
  return decoded;
}