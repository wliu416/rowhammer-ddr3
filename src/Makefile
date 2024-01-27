CC=g++

all: bin bin/histogram bin/rowmapper bin/hammering
clean:
	rm -rf bin/

bin/histogram: src/histogram/histogram.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/histogram/histogram.cc src/util.hh src/shared.cc

bin/rowmapper: src/reverse_engineering/rowmapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/reverse_engineering/rowmapper.cc src/util.hh src/shared.cc

bin/hammering: src/hammering/hammering.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/hammering.cc src/util.hh src/shared.cc

bin:
	mkdir bin