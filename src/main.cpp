#include "huffman_ff/huffman_ff.h"
#include "huffman_sequential/huffman_seq.h"
#include "huffman_threads/huffman_threads.h"

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Usage: ./huffman_seq <input_file> <output_file> <seq|t|ff> "
                 "<n_threads>"
              << std::endl;
    return 1;
  }
  std::string input_file = argv[1];
  std::string output_file = argv[2];
  std::string type = argv[3];

  huffman_base *huffman;
  int num_threads =
      argv[4] ? atoi(argv[4]) : std::thread::hardware_concurrency();

  if (type == "seq")
    huffman = new huffman_seq(input_file, output_file);
  else if (type == "t")
    huffman = new huffman_thread(input_file, output_file, num_threads);
  else if (type == "ff")
    huffman = new huffman_ff(input_file, output_file, num_threads);
  else {
    std::cout << "Usage: ./huffman_seq <input_file> <output_file> <seq|t|ff> "
                 "<n_threads>"
              << std::endl;
    return 1;
  }

  huffman->run();

#ifdef DECODE
  std::string decoded = huffman->decode_file(output_file, huffman->get_root());
  if (decoded == huffman->get_text())
    std::cout << "Decoded successfully." << std::endl;
  else
    std::cout << "Decoded unsuccessfully." << std::endl;
  return 0;
#endif

  delete huffman;
  return 0;
}