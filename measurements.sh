file_name=$1
if [ ! -f "$file_name" ]
then
    echo "File $file not found!"
    exit 1
fi

rm measurements/seq.csv
rm measurements/threads.csv
rm measurements/ff.csv

echo "Running sequential \n"
./HuffmanProject $file_name binary seq

for i in 2 4 8 16 32 64 128
do
    echo "Running threads version with $i threads \n"
     ./HuffmanProject $file_name binary t $i
done

for i in 2 4 8 16 32 64 128
do
    echo "Running fastflow with $i threads \n"
     ./HuffmanProject $file_name binary ff $i
done
mv measurements/seq.csv measurements/seq-nojemalloc.csv
mv measurements/threads.csv measurements/threads-nojemalloc.csv
mv measurements/ff.csv measurements/ff-nojemalloc.csv

if [ "$2" = "jemalloc" ]
then
    export LD_PRELOAD=/usr/local/lib/libjemalloc.so

    echo "Running sequential (jemalloc) \n"
    ./HuffmanProject $file_name binary seq

    for i in 2 4 8 16 32 64 128
    do
        echo "Running $i threads version (jemalloc) \n"
        LD_PRELOAD=$LD_PRELOAD ./HuffmanProject $file_name binary t $i
    done

    for i in 2 4 8 16 32 64 128
    do
        echo "Running fastflow version ($i threads) (jemalloc) \n"
        LD_PRELOAD=$LD_PRELOAD ./HuffmanProject $file_name binary ff $i
    done
fi

mv measurements/seq.csv measurements/seq-jemalloc.csv
mv measurements/threads.csv measurements/threads-jemalloc.csv
mv measurements/ff.csv measurements/ff-jemalloc.csv

