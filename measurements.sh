file_name=$1
jemalloc_path=$2

if [ ! -f "$file_name" ]
then
    echo "File $file not found!"
    exit 1
fi


for i in 1 2 3 4 5
do
    echo "Running sequential"
    ./HuffmanProject $file_name binary seq
done

for i in 2 4 8 16 32 64 128
do
    for j in 1 2 3 4 5
    do
        echo "Running $i threads version"
        ./HuffmanProject $file_name binary t $i
    done
done

for i in 2 4 8 16 32 64 128
do
    for j in 1 2 3 4 5
    do
        echo "Running fastflow with $i threads"
        ./HuffmanProject $file_name binary ff $i
    done
done

mv measurements/seq.csv measurements/seq-nojemalloc.csv
mv measurements/threads.csv measurements/threads-nojemalloc.csv
mv measurements/ff.csv measurements/ff-nojemalloc.csv


echo "Running sequential (jemalloc)"
for i in 1 2 3 4 5
do
    LD_PRELOAD=$jemalloc_path ./HuffmanProject $file_name binary seq
done

for i in 2 4 8 16 32 64 128
do
    for j in 1 2 3 4 5
    do
        echo "Running $i threads version (jemalloc)"
        LD_PRELOAD=$jemalloc_path ./HuffmanProject $file_name binary t $i
    done
done

for i in 2 4 8 16 32 64 128
do
    for j in 1 2 3 4 5
    do
        echo "Running fastflow version ($i threads) (jemalloc) "
        LD_PRELOAD=$jemalloc_path ./HuffmanProject $file_name binary ff $i
    done
done

mv measurements/seq.csv measurements/seq-jemalloc.csv
mv measurements/threads.csv measurements/threads-jemalloc.csv
mv measurements/ff.csv measurements/ff-jemalloc.csv


