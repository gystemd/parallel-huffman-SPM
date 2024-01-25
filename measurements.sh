# echo "Running sequential\n"
# ./HuffmanProject samples/1000000000.txt binary seq
rm measurements/threads.csv
for i in 2 4 8 16 32 64 128
do
    echo "Running with $i threads\n"
    ./HuffmanProject samples/1000000000.txt binary t $i
done

rm measurements/ff.csv
for i in 2 4 8 16 32 64 128
do
    echo "Running with $i threads\n"
    ./HuffmanProject samples/1000000000.txt binary ff $i
done

