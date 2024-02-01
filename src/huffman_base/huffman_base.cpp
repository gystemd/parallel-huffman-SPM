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

std::string huffman_base::read_file(std::string input_file) {
  std::ifstream in(input_file);
  std::string seq;

  if (!in.is_open())
    throw std::runtime_error("Could not open file: " + input_file);
  getline(in, seq);
  in.close();
  return seq;
}

huffman_base::huffman_node *huffman_base::build_tree(
    std::unordered_map<char, unsigned int>& freq) {
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

std::unordered_map<char, std::vector<bool> *> huffman_base::build_codes(
    huffman_base::huffman_node *root) {
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
    // write_file(*(this->encoded), this->output_file);
  }

  total_time = read_time + frequency_time + tree_time + code_time +
               encode_time + write_time;
  total_time_nio = frequency_time + tree_time + code_time + encode_time;
  write_benchmark();
}

void huffman_base::write_file(encoded_t &encoded, std::string output_file) {
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open())
        throw std::runtime_error("Could not open file: " + output_file);

    std::vector<unsigned char> bytes;
    int bitsWritten = 0;
    int totalWritten = 0;
    unsigned char byte = 0;

    for (const auto &chunk : encoded)
    {
        for (const auto &vec : *chunk)
        {
            for (auto bit : *vec)
            {
                byte |= (bit << bitsWritten);
                bitsWritten++;
                totalWritten++;
                if (bitsWritten == 8)
                {
                    bytes.push_back(byte);
                    byte = 0;
                    bitsWritten = 0;
                }
            }
        }
    }

    // Handle padding
    if (bitsWritten > 0)
        bytes.push_back(byte);
    unsigned char header = 8 - (totalWritten % 8);
    if (header == 8)
        header = 0;

    // writing a header which contains the number of bits to discard from the last byte.
    out << char(header);
    out.write(reinterpret_cast<const char *>(bytes.data()), (long)bytes.size());
    out.close();
}


std::string huffman_base::decode_file(std::string input_file,const huffman_node *root) {
    std::ifstream in(input_file, std::ios::binary);
    auto encoded = new std::vector<bool>();

    if (!in.is_open())
        throw std::runtime_error("Could not open file: " + input_file);

    // first byte is the header, which contains the number of bits to discard from the last byte.
    char header;
    in.get(header);

    char c;
    while (in.get(c))
    {
        std::bitset<8> bits(c);
        for (int i = 0; i < 8; i++)
            encoded->push_back(bits[i]);
    }

    // discard the last n bits, where n is the header.
    encoded->erase(encoded->end() - int(header), encoded->end());
    in.close();
    return decode(*encoded, root) ;
}

std::string huffman_base::decode(const std::vector<bool> &encoded, const huffman_base::huffman_node *root)
{
    std::string decoded = "";
    auto node = root;
    for (auto b : encoded)
    {
        if (b)
            node = node->right;
        else
            node = node->left;

        if (node->data != '\0')
        {
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
  for (auto &v : *encoded) {
    for (auto &code : *v) delete code;
    delete v;
  }
  delete encoded;

}