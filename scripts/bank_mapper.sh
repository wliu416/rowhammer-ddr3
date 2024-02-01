#!/bin/bash
SOURCE=$(dirname -- "${BASH_SOURCE[0]}")

if [ ! -d $SOURCE/../data ]; then
    mkdir $SOURCE/../data
fi

echo "Running Row to Bank Mapper and testing hypotheses"
echo "$SOURCE/../bin/bank_mapper > $SOURCE/../data/bank_mapper.out"
for i in 1 2 3 4 5 6 7 8 9 10; do cpupower frequency-info >> /dev/null; done && $SOURCE/../bin/bank_mapper > $SOURCE/../data/bank_mapper.out
