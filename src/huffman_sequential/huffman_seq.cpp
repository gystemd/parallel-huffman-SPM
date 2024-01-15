#include "huffman_seq.h"

#include <unordered_map>
// #include "utimer.cpp"

std::unordered_map<char, int> huffman_seq::count_frequency() {
  std::unordered_map<char, int> freq;
  for (char c : text) {
    freq[c]++;
  }
  return freq;
}

encoded_t* huffman_seq::encode_string() {
  encoded_t* encoded = new encoded_t();
  std::vector<std::vector<bool>*>* encoded_chunk =
      new std::vector<std::vector<bool>*>();
  for (char c : text) encoded_chunk->push_back(codes[c]);
  encoded->push_back(encoded_chunk);
  return encoded;
}