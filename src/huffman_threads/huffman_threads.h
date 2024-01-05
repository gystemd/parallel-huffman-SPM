#include "../huffman_base/huffman_base.h"

class huffman_thread : public huffman_base {
 public:
  int num_threads;
  std::unordered_map<char, int> count_frequency() override;
  std::string encode_string() override;
  huffman_thread(std::string input_file, std::string output_file,
                 int par_degree)
      : huffman_base(input_file, output_file), num_threads(par_degree) {}
};
