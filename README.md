This project is an implementation of the [Huffman](https://en.wikipedia.org/wiki/Huffman_coding) algorithm in C++. Two versions are provided: one that uses
the pthread library from the C++ standard library and another that uses the fastflow framework. You can find architectural details and experiments sin the `report.pdf` file.

## Install with docker
Installing with docker is the recommended way to run the project.
You can run the following commands to build the docker image and run the project:
```bash
docker build -t huffman .
docker run -it huffman
```

Then, you can encode a file with the following command:
```bash
./HuffmanProject <file_name> <output_file> <seq|t|ff> <num_threads>
```

You have to choose an option for the third argument which can be `seq` for the sequential version, `t` for the version using the pthread library and `ff` for the version using the fastflow framework. The last argument is the number of threads to use in the parallel versions.

An example of encoding a file with the pthread version:
```bash
./HuffmanProject test.txt test_encoded t 64
```

