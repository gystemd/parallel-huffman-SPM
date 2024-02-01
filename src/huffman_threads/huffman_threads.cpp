#include "huffman_threads.h"

#include <thread>
#include <unordered_map>

/**
 * Counts the frequency of each character in a string using multithreading.
 * @param text The string to count the character frequencies in.
 * @return A map where each key-value pair represents a character and its
 * frequency.
 */
std::unordered_map<char, unsigned int> huffman_thread::count_frequency(
    std::string &text) {
  std::unordered_map<char, unsigned int> partial_freqs[num_threads];
  std::unordered_map<char, unsigned int> result;
  std::vector<std::thread> threads(num_threads);

  // The map function counts the frequency of each character in a chunk of the
  // string.
  auto map = [&](size_t tid) {
    auto start = tid * (text.length() / num_threads);
    auto end = (tid + 1) * (text.length() / num_threads);
    if (tid == num_threads - 1) end = text.length();

    for (size_t i = start; i < end; i++) partial_freqs[tid][text[i]]++;
  };

  for (size_t i = 0; i < num_threads; i++) threads[i] = std::thread(map, i);

  for (auto &t : threads) t.join();

  for (auto &partial_freq : partial_freqs)
    for (auto &it : partial_freq) result[it.first] += it.second;

  return result;
}

encoded_t *huffman_thread::encode_string(
    std::unordered_map<char, std::vector<bool> *> &codes, std::string &text) {
  std::vector<std::thread> threads(num_threads);
  auto size = text.length();
  auto encoded_chunks = new encoded_t(num_threads);

  auto encode_executor = [&](size_t tid) {
    auto chunk_size = size / num_threads;
    auto start = tid * chunk_size;
    auto end = tid == num_threads - 1 ? size : (tid + 1) * chunk_size;

    encoded_chunks->at(tid) = new std::vector<std::vector<bool> *>();
    encoded_chunks->at(tid)->reserve(end - start);
    for (size_t i = start; i < end; i++)
      encoded_chunks->at(tid)->push_back(codes[text[i]]);
  };

  for (size_t i = 0; i < num_threads; i++)
    threads[i] = std::thread(encode_executor, i);

  for (auto &t : threads) t.join();
  return encoded_chunks;
}