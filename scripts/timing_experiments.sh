#!/bin/bash
SOURCE=$(dirname -- "${BASH_SOURCE[0]}")

if [ ! -d $SOURCE/../data ]; then
    mkdir $SOURCE/../data
fi

echo "Part 1. Running Row bit xor timing experiments."
echo "$SOURCE/../bin/row_bxor_mapper > $SOURCE/../data/row_bxor_mapper.out"
for i in 1 2 3 4 5 6 7 8 9 10; do cpupower frequency-info >> /dev/null; done && $SOURCE/../bin/row_bxor_mapper > $SOURCE/../data/row_bxor_mapper.out


echo "Part 2. Running Bank XOR bit flipping timing experiments."
echo "$SOURCE/../bin/bxor_manipulation_experiment > $SOURCE/../data/bxor_manipulation_experiment.out"
for i in 1 2 3 4 5 6 7 8 9 10; do cpupower frequency-info >> /dev/null; done && $SOURCE/../bin/bxor_manipulation_experiment > $SOURCE/../data/bxor_manipulation_experiment.out
