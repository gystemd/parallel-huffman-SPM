
#include "../huffman_base/huffman_base.h"

class huffman_ff : public huffman_base {
 public:
  std::unordered_map<char, unsigned int> count_frequency(
      std::string &text) override;
  virtual encoded_data *encode_string(
      std::unordered_map<char, std::vector<bool> *> &codes,
      std::string &text) override;

  huffman_ff(std::string input_file, std::string output_file, int n_threads)
      : huffman_base(input_file, output_file) {
    num_threads = n_threads;
    benchmark_file = "measurements/ff.csv";
  }
};