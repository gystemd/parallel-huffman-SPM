#include "../huffman_base/huffman_base.h"

class huffman_seq : public huffman_base {
 public:
  std::unordered_map<char, int> count_frequency() override;
  encoded_t* encode_string() override;
  huffman_seq(std::string input_file, std::string output_file)
      : huffman_base(input_file, output_file) {
    benchmark_file = "measurements/seq.csv";
  }
};
