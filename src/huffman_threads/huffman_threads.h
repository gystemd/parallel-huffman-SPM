#include "../huffman_base/huffman_base.h"

class huffman_thread : public huffman_base {
 public:
  std::unordered_map<char, unsigned int> count_frequency() override;
  encoded_t* encode_string() override;
  huffman_thread(std::string input_file, std::string output_file,
                 int par_degree)
      : huffman_base(input_file, output_file) {
    num_threads = par_degree;
    benchmark_file = "measurements/threads.csv";
  }
};
