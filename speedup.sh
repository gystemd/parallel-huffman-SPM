# echo "Running sequential version \n"
# ./HuffmanProject samples/1000000000.txt binary seq

i=128
echo "Running parallel version with $i threads \n"
./HuffmanProject samples/1000000000.txt binary ff $i