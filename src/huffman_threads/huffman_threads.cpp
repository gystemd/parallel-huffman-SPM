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

encoded_t huffman_thread::encode_string() {
  encoded_t encoded;
  {
    long time_elapsed;
    std::vector<std::thread> threads(num_threads);
    auto encoded_parts =
        std::make_unique<std::vector<std::vector<std::vector<bool>*>>>(
            num_threads);

    int chunk_size = text.size() / num_threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
      int start = i * chunk_size;
      int end = (i == num_threads - 1) ? text.size() : start + chunk_size;

      auto &encoded_part = encoded_parts->at(i);
      encoded_part.reserve(end - start);

      threads[i] = std::thread([&, start, end]() {
        for (int j = start; j < end; ++j){
          encoded_part.emplace_back(codes[text[j]]);
          // std::cout<<"Thread "<<i<<std::endl;
        }
      });
    }

    for (auto &t : threads) t.join();

    // for (auto &part : encoded_parts)
    //   for (auto &code : part) encoded.push_back(code);
  }

  return encoded;
}