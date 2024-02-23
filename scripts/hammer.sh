#!/bin/bash
SOURCE=$(dirname -- "${BASH_SOURCE[0]}")

if [ ! -d $SOURCE/../data ]; then
    mkdir $SOURCE/../data
fi

echo "Running Rowhammer Experiment"
echo "$SOURCE/../bin/hammering > $SOURCE/../data/hammering.out"
for i in 1 2 3 4 5 6 7 8 9 10; do cpupower frequency-info >> /dev/null; done && $SOURCE/../bin/hammering > $SOURCE/../data/hammering.out

