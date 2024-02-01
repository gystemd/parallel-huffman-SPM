#include "huffman_base.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <thread>
#include <vector>

#include "../utimer.cpp"
#include "bitset"
#include "unordered_map"

/**
 * Reads the content of the file to compress as a string.
 *
 * @param input_file The name of the file to read.
 * @return The content of the file as a string.
 * @throws std::runtime_error If the file could not be opened.
 */
std::string huffman_base::read_file(std::string input_file) {
  std::ifstream in(input_file);
  std::string seq;

  if (!in.is_open())
    throw std::runtime_error("Could not open file: " + input_file);
  getline(in, seq);
  in.close();
  return seq;
}

/**
 * Builds a Huffman tree from a frequency map.
 *
 * @param freq A map where each key-value pair represents a character and its
 * frequency.
 * @return A pointer to the root node of the Huffman tree.
 */
huffman_base::huffman_node *huffman_base::build_tree(
    std::unordered_map<char, unsigned int> &freq) {
  struct Compare {
    bool operator()(huffman_node *left, huffman_node *right) {
      // Nodes with higher frequency have lower priority.
      return left->frequency > right->frequency;
    }
  };

  std::priority_queue<huffman_node *, std::vector<huffman_node *>, Compare> pq;

  // Create a node for each character and add it to the priority queue.
  for (auto entry : freq) pq.push(new huffman_node(entry.first, entry.second));

  while (pq.size() > 1) {
    // Remove the two nodes with the highest priority (lowest frequency).
    huffman_node *left = pq.top();
    pq.pop();

    huffman_node *right = pq.top();
    pq.pop();

    // Create a new node that is the parent of the two nodes.
    huffman_node *parent =
        new huffman_node('\0', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }

  return pq.top();
}

/**
 * Builds a map of Huffman codes from a Huffman tree.
 * @param root A pointer to the root node of the Huffman tree.
 * @return A map where each key-value pair represents a character and its
 * Huffman code.
 */
std::unordered_map<char, std::vector<bool> *> huffman_base::build_codes(
    huffman_base::huffman_node *root) {
  std::unordered_map<char, std::vector<bool> *> codes;

  // Stack to keep track of nodes and their corresponding codes
  std::stack<std::pair<huffman_node *, std::vector<bool> *>> stack;

  // Start with the root node
  std::vector<bool> *code = new std::vector<bool>();
  stack.push({root, code});

  while (!stack.empty()) {
    auto node = stack.top().first;
    auto code = stack.top().second;
    stack.pop();

    if (node->left == nullptr && node->right == nullptr) {
      // If the node is a leaf, add its code to the map
      codes[node->data] = code;
    } else {
      // If the node has a left child, add a '0' to the code and push it to the
      // stack
      if (node->left) {
        std::vector<bool> *left_code = new std::vector<bool>(*code);
        left_code->push_back(false);
        stack.push({node->left, left_code});
      }

      // If the node has a right child, add a '1' to the code and push it to the
      // stack
      if (node->right) {
        std::vector<bool> *right_code = new std::vector<bool>(*code);
        right_code->push_back(true);
        stack.push({node->right, right_code});
      }
    }
  }

  return codes;
}

/**
 * Writes the encoded data to a binary file.
 *
 * This function takes the encoded data and an output file name as parameters.
 * It writes the encoded data to the binary file. The binary file starts with a
 * header that indicates how many bits to discard from the last byte.
 *
 * @param encoded The encoded data to write to the file. This is a vector of
 * vectors of boolean values, where each boolean represents a bit.
 * @param output_file The name of the file to write the encoded data to.
 */
void huffman_base::write_file(encoded_t &encoded, std::string output_file) {
  std::ofstream out(output_file, std::ios::binary);
  if (!out.is_open())
    throw std::runtime_error("Could not open file: " + output_file);

  std::vector<unsigned char> bytes;
  int current_bits = 0;
  unsigned char byte = 0;

  for (const auto &chunk : encoded) {
    for (const auto &code : *chunk) {
      for (auto bit : *code) {
        // a byte may contain bits from different codes to fully exploit the
        // compression. Therefore we need to keep track of how many bits we have
        // already written up to now.
        byte |= (bit << current_bits);
        current_bits++;
        if (current_bits == 8) {
          bytes.push_back(byte);
          byte = 0;
          current_bits = 0;
        }
      }
    }
  }

  if (current_bits > 0) bytes.push_back(byte);
  // the header indicates how many bits to discard from the last byte.
  unsigned char header = 8 - current_bits;
  if (header == 8) header = 0;

  out << char(header);
  out.write(reinterpret_cast<const char *>(bytes.data()), (long)bytes.size());
  out.close();
}

/**
 * Decodes a binary file using a Huffman tree.
 *
 * This function reads a binary file that contains Huffman-encoded data. It uses
 * a Huffman tree to decode the data. The binary file starts with a header that
 * indicates how many bits to discard from the last byte.
 *
 * @param input_file The name of the file to read the encoded data from.
 * @param root A pointer to the root node of the Huffman tree.
 * @return The decoded data as a string.
 */
std::string huffman_base::decode_file(std::string input_file,
                                      const huffman_node *root) {
  std::ifstream in(input_file, std::ios::binary);
  auto encoded = new std::vector<bool>();

  if (!in.is_open())
    throw std::runtime_error("Could not open file: " + input_file);

  // header indicates how many bits to discard from the last byte.
  char header;
  in.get(header);

  char c;
  while (in.get(c)) {
    std::bitset<8> bits(c);
    for (int i = 0; i < 8; i++) encoded->push_back(bits[i]);
  }

  // erase the last 'header' bits, which are not part of the encoded data.
  encoded->erase(encoded->end() - int(header), encoded->end());
  in.close();
  return decode(*encoded, root);
}

std::string huffman_base::decode(const std::vector<bool> &encoded,
                                 const huffman_base::huffman_node *root) {
  std::string decoded = "";
  auto node = root;
  for (auto b : encoded) {
    if (b)
      node = node->right;
    else
      node = node->left;

    if (node->data != '\0') {
      decoded.push_back(node->data);
      node = root;
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
  // file << num_threads << "," << read_time << "," << frequency_time << "," <<
  // tree_time << ","
  //      << code_time << "," << encode_time << "," << write_time << std::endl;
  file << num_threads << "," << total_time << "," << total_time_nio
       << std::endl;
  file.close();
}

void huffman_base::run() {
  {
    utimer timer("read", &read_time);
    this->text = read_file(this->input_file);
  }

  {
    utimer timer("count frequency", &frequency_time);
    this->freq = count_frequency(this->text);
  }

  {
    utimer timer("build tree", &tree_time);
    this->root = build_tree(this->freq);
  }

  {
    utimer timer("build codes", &code_time);
    this->codes = build_codes(this->root);
  }

  {
    utimer timer("encode", &encode_time);
    this->encoded = encode_string(this->codes, this->text);
  }

  {
    utimer timer("write", &write_time);
    write_file(*(this->encoded), this->output_file);
  }

  total_time = read_time + frequency_time + tree_time + code_time +
               encode_time + write_time;
  total_time_nio = frequency_time + tree_time + code_time + encode_time;
  write_benchmark();
}

huffman_base::~huffman_base() {
  // delete the tree
  std::function<void(huffman_node *)> delete_tree = [&](huffman_node *node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
  };
  delete_tree(root);

  // delete the codes
  for (auto &entry : codes) delete entry.second;

  // delete the encoding
  for (auto &v : *encoded) delete v;
  delete encoded;
}