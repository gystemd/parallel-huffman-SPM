# echo "Running sequential\n"
# ./HuffmanProject samples/1000000000.txt binary seq
file_name=$1
# check if file exists
if [ ! -f "$file_name" ]
then
    echo "File $file not found!"
    exit 1
fi
# check if the second argument is equal to y
if [ "$2" = "y" ]
then
    echo "Running with jemalloc\n"
    export LD_PRELOAD=/usr/local/lib/libjemalloc.so
fi
rm measurements/seq.csv
echo "Running sequential\n"
./HuffmanProject $file_name binary seq

rm measurements/threads.csv
for i in 2 4 8 16 32 64 128
do
    echo "Running with $i threads\n"
    LD_PRELOAD=$LD_PRELOAD ./HuffmanProject $file_name binary t $i
done

rm measurements/ff.csv
for i in 2 4 8 16 32 64 128
do
    echo "Running with $i threads\n"
    LD_PRELOAD=$LD_PRELOAD ./HuffmanProject $file_name binary ff $i
done

