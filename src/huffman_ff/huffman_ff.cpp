
#include "huffman_ff.h"

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <unordered_map>

#include "../utimer.cpp"

using namespace std;
using namespace ff;

unordered_map<char, unsigned int> huffman_ff::count_frequency(string text) {
  auto res = unordered_map<char, unsigned int>();
  auto map_f = [&](const long i, unordered_map<char, unsigned>& tempsum) {
    tempsum[text[i]]++;
  };

  auto red_f = [&](unordered_map<char, unsigned>& a,
                   const unordered_map<char, unsigned>& b) {
    for (auto& it : b) a[it.first] += it.second;
  };
  auto pf = ParallelForReduce<unordered_map<char, unsigned>>((long)num_threads);
  pf.parallel_reduce(res, unordered_map<char, unsigned>(), 0, (long)text.size(),
                     1, map_f, red_f, num_threads);
  return res;
}

encoded_t* huffman_ff::encode_string(unordered_map<char, vector<bool>*> codes,
                                     string text) {
  auto results = new encoded_t(num_threads);
  ParallelFor pf(num_threads);

  auto Map = [&](const long start, const long stop, const int thid) {
    auto chunk = new std::vector<std::vector<bool>*>();
    chunk->reserve(stop - start);

    for (auto i = start; i < stop; i++) chunk->push_back(codes[text[i]]);
    results->at(thid) = chunk;
  };

  pf.parallel_for_idx(0, text.size(), 1, 0, Map, num_threads);
  return results;
}
