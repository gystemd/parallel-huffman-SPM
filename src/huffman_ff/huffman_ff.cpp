
#include "huffman_ff.h"

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <unordered_map>

#include "../utimer.cpp"

using namespace std;
using namespace ff;

unordered_map<char, unsigned int> huffman_ff::count_frequency(string& text) {
  auto frequency = unordered_map<char, unsigned int>();
  auto map = [&](const long i, unordered_map<char, unsigned>& partial_freq) {
    partial_freq[text[i]]++;
  };

  auto reduce = [&](unordered_map<char, unsigned>& a,
                    const unordered_map<char, unsigned>& b) {
    for (auto& it : b) a[it.first] += it.second;
  };

  auto pf = ParallelForReduce<unordered_map<char, unsigned>>((long)num_threads);
  pf.parallel_reduce(frequency, unordered_map<char, unsigned>(), 0,
                     (long)text.size(), 1, map, reduce, num_threads);
  return frequency;
}

encoded_data* huffman_ff::encode_string(unordered_map<char, vector<bool>*>& codes,
                                     string& text) {
  auto encoded_chunks = new encoded_data(num_threads);
  ParallelFor pf(num_threads);

  auto map = [&](const long start, const long stop, const int thid) {
    auto chunk = new std::vector<std::vector<bool>*>();
    chunk->reserve(stop - start);

    for (auto i = start; i < stop; i++) chunk->push_back(codes[text[i]]);
    encoded_chunks->at(thid) = chunk;
  };

  pf.parallel_for_idx(0, text.size(), 1, 0, map, num_threads);
  return encoded_chunks;
}
