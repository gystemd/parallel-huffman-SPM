#include <unordered_map>
#include "huffman_seq.h"
// #include "utimer.cpp"

std::unordered_map<char, int> huffman_seq::count_frequency() {
  std::unordered_map<char, int> freq;
  for (char c : text ) {freq[c]++;}
  return freq;
}

std::string huffman_seq::encode_string() {
  std::string encoded = "";
  for (char c : text) encoded += codes[c];
  return encoded;
}