#include <unordered_map>
#include "huffman_seq.h"
// #include "utimer.cpp"

std::unordered_map<char, int> huffman_seq::count_frequency() {
  std::unordered_map<char, int> freq;
  for (char c : text ) {freq[c]++;}
  return freq;
}

encoded_t huffman_seq::encode_string() {
  encoded_t encoded;
  for (char c : text) {encoded.push_back(codes[c]);}
  return encoded;
}