rm measurements/*
echo "Running sequential\n"
./HuffmanProject samples/1000000000.txt binary seq

for i in 2 4 8 16 32 64 128 256
do
    echo "Running with $i threads\n"
    ./HuffmanProject samples/1000000000.txt binary t $i
done


