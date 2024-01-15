#include <cstdlib>
#include <ctime>
#include <fstream>  // Include the fstream library
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <num_char> <output_file>"
              << std::endl;
    return 1;
  }

  int num_char = atoi(argv[1]);
  std::string output_file = argv[2];
  int srand(123);
  std::string text = "";
  char random_char;
  for (int i = 0; i < num_char; i++) {
    // skew the distribution of characters
    // if (rand() % 100 < 80)
    //   random_char = 'a';
    // else
    //   random_char = 'b' + rand() % 25;
    // text += random_char;
    text += 'a' + rand() % 26;
  }
  std::ofstream file(output_file);
  file << text << std::endl;
  file.close();
}