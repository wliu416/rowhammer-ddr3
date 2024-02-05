CC=g++

all: bin bin/histogram bin/histogram2 bin/bank_mapper
clean:
	rm -rf bin/

bin/histogram: src/histogram/histogram.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/histogram/histogram.cc src/util.hh src/shared.cc

bin/histogram2: src/histogram/histogram2.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/histogram/histogram2.cc src/util.hh src/shared.cc

bin/histogram_alt: src/histogram/histogram.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -c src/histogram/histogram.cc -o bin/histogram.o
	$(CC) -std=c++11 -g -O0 -c src/shared.cc -o bin/shared.o
	$(CC) -o bin/histogram bin/histogram.o bin/shared.o

bin/bank_mapper: src/hammering/bank_mapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/bank_mapper.cc src/util.hh src/shared.cc

bin/rowmapper: src/reverse_engineering/rowmapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/reverse_engineering/rowmapper.cc src/util.hh src/shared.cc

bin/hammering: src/hammering/hammering.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/hammering.cc src/util.hh src/shared.cc

bin:
	mkdir bin