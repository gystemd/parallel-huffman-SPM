#include "huffman_threads.h"
#include <thread>
#include <unordered_map>

std::unordered_map<char, int> huffman_thread::count_frequency() {
  std::unordered_map<char, int>
      partial_freqs[num_threads];  
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

std::string huffman_thread::encode_string() {
  std::string encoded;
  {
    long time_elapsed;
    std::vector<std::thread> threads(num_threads);
    std::vector<std::string> encoded_parts(num_threads);

    int chunk_size = text.size() / num_threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
      int start = i * chunk_size;
      int end = (i == num_threads - 1) ? text.size() : start + chunk_size;
      threads[i] = std::thread([&, i, start, end]() {
        for (int j = start; j < end; ++j) encoded_parts[i] += codes[text[j]];
      });
    }

    for (auto &t : threads) t.join();

    // Combine all the encoded strings
    for (const auto &part : encoded_parts) encoded += part;
  }

  std::cout<<encoded.size()<<std::endl;
  return encoded;
}