#!/bin/bash
SOURCE=$(dirname -- "${BASH_SOURCE[0]}")

if [ ! -d $SOURCE/../data ]; then
    mkdir $SOURCE/../data
fi

echo "Running Histogram Generation Command."
echo "$SOURCE/../bin/histogram2 > $SOURCE/../data/histogram2.out"
for i in 1 2 3 4 5 6 7 8 9 10; do cpupower frequency-info >> /dev/null; done && $SOURCE/../bin/histogram2 > $SOURCE/../data/histogram2.out

echo "Using Python, Generate Histogram IMAGE."
echo "python3 $SOURCE/../src/histogram/histogram2.py"
python3 $SOURCE/../src/histogram/histogram2.py

echo "cat $SOURCE/../data/histogram2.out"
cat $SOURCE/../data/histogram2.out