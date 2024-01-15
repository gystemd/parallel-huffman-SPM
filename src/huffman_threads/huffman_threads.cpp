#include "huffman_threads.h"

#include <thread>
#include <unordered_map>

std::unordered_map<char, int> huffman_thread::count_frequency() {
  std::unordered_map<char, int> partial_freqs[num_threads];
  std::unordered_map<char, int> result;
  std::vector<std::thread> thread_mappers(num_threads);

  auto map_executor = [&](size_t tid) {
    auto start = tid * (text.length() / num_threads);
    auto end = (tid + 1) * (text.length() / num_threads);
    if (tid == num_threads - 1) end = text.length();

    for (size_t i = start; i < end; i++) partial_freqs[tid][text[i]]++;
  };

  // start the threads
  for (size_t i = 0; i < num_threads; i++)
    thread_mappers[i] = std::thread(map_executor, i);

  for (auto &t : thread_mappers) t.join();

  for (auto &partial_freq : partial_freqs)
    for (auto &it : partial_freq) result[it.first] += it.second;

  return result;
}

encoded_t *huffman_thread::encode_string() {
  std::vector<std::thread> threads(num_threads);
  auto size = text.length();
  auto results = new encoded_t(num_threads);

  auto encode_executor = [&](size_t tid) {
    auto chunk_size = size / num_threads;
    // split the sequence in chunks
    auto start = tid * chunk_size;
    auto end = tid == num_threads - 1 ? size : (tid + 1) * chunk_size;

    results->at(tid) = new std::vector<std::vector<bool> *>();
    results->at(tid)->reserve(end - start);
    for (size_t i = start; i < end; i++) {
      results->at(tid)->push_back(codes[text[i]]);
    }
  };

  // start and join the threads
  for (size_t i = 0; i < num_threads; i++)
    threads[i] = std::thread(encode_executor, i);
  for (auto &t : threads) t.join();
  return results;
}