#include "huffman_seq.h"

#include <unordered_map>
// #include "utimer.cpp"

std::unordered_map<char, unsigned int> huffman_seq::count_frequency(
    std::string& text) {
  std::unordered_map<char, unsigned int> freq;
  for (char c : text) {
    freq[c]++;
  }
  return freq;
}

encoded_t* huffman_seq::encode_string(
    std::unordered_map<char, std::vector<bool>*>& codes, std::string &text) {
  encoded_t* encoded = new encoded_t();
  std::vector<std::vector<bool>*>* encoded_chunk =
      new std::vector<std::vector<bool>*>();
  for (char c : text) encoded_chunk->push_back(codes[c]);
  encoded->push_back(encoded_chunk);
  return encoded;
}