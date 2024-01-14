
#include <unordered_map>
#include "huffman_ff.h"
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace ff;
std::unordered_map<char, int> huffman_ff::count_frequency() {
    auto res = std::unordered_map<char, int>();

    auto map_f = [&](const long i, std::unordered_map<char, int> &tempsum){
        tempsum[text[i]]++;
    };

    auto red_f = [&](std::unordered_map<char, int> &a, const std::unordered_map<char, int> &b){
        for (auto &it : b)
            a[it.first] += it.second;
    };

    auto pf = ParallelForReduce<std::unordered_map<char, int>>((long)num_threads);
    pf.parallel_reduce(res, std::unordered_map<char, int>(), 0, (long)text.size(), 1, map_f, red_f, num_threads);


    return res;
}

std::string huffman_ff::encode_string() {
  std::string encoded = "";
  for (char c : text) encoded += codes[c];
  return encoded;
}