CC=g++

all: bin bin/histogram bin/bank_mapper bin/row_bxor_mapper bin/bxor_manipulation_experiment bin/pagemap bin/hammering
clean:
	rm -rf bin/

bin/histogram: src/histogram/histogram.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/histogram/histogram.cc src/util.hh src/shared.cc

bin/histogram2: src/histogram/histogram2.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/histogram/histogram2.cc src/util.hh src/shared.cc


bin/bank_mapper: src/hammering/bank_mapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/bank_mapper.cc src/util.hh src/shared.cc

bin/row_bxor_mapper: src/hammering/row_bxor_mapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/row_bxor_mapper.cc src/util.hh src/shared.cc

bin/bxor_manipulation_experiment: src/hammering/bxor_manipulation_experiment.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/bxor_manipulation_experiment.cc src/util.hh src/shared.cc

bin/rowmapper: src/reverse_engineering/rowmapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/reverse_engineering/rowmapper.cc src/util.hh src/shared.cc

bin/hammering: src/hammering/hammering.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) -std=c++11 -g -O0 -o $@ src/hammering/hammering.cc src/util.hh src/shared.cc

bin/pagemap: src/pagemap.cc
	$(CC) -std=c++11 -g -O0 -o $@ src/pagemap.cc

bin:
	mkdir bin