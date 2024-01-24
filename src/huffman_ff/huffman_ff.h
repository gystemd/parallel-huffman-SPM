
#include "../huffman_base/huffman_base.h"

class huffman_ff : public huffman_base {
  int num_threads;

 public:
  std::unordered_map<char, unsigned int> count_frequency() override;
  encoded_t* encode_string() override;
  huffman_ff(std::string input_file, std::string output_file, int n_threads)
      : huffman_base(input_file, output_file), num_threads(n_threads)  {}
};