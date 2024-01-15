#include "huffman_sequential/huffman_seq.h"
#include "huffman_threads/huffman_threads.h"
#include "huffman_ff/huffman_ff.h"
int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Usage: ./huffman_seq <input_file> <output_file> <seq|t|ff> <n_threads>" << std::endl;
    return 1;
  }
  std::string input_file = argv[1];
  std::string output_file = argv[2];
  std::string type = argv[3];

  huffman_base *huffman;
  if(type == "seq")
    huffman = new huffman_seq(input_file, output_file);
  else if(type == "t"){
    int num_threads = argv[4] ? atoi(argv[4]) : std::thread::hardware_concurrency();
    huffman = new huffman_thread(input_file, output_file, num_threads);
  }
  else if (type == "ff"){
    int num_threads = argv[4] ? atoi(argv[4]) : std::thread::hardware_concurrency();
    huffman = new huffman_ff(input_file, output_file, num_threads);
  }
  else{
    std::cout << "Usage: ./huffman_seq <input_file> <output_file> <seq|t|ff> <n_threads>" << std::endl;
    return 1;
  }
  huffman->run();
  return 0;
}