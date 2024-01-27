#ifndef PARAMS_GUARD
#define PARAMS_GUARD

// Size of allocated buffer
#define BUFFER_SIZE_MB 2048

// Size of hugepages in system
#define HUGE_PAGE_SIZE (1 << 21)

// Number of offset bits
#define PAGE_OFFSET_BITS 12

// Size of DRAM row (1 bank)
#define ROW_SIZE (8192)

// Number of hammers to perform per iteration
#define HAMMERS_PER_ITER 5000000

// TODO: RUN HISTOGRAM!
// Latency Threshold for Row Buffer Conflict
#define ROW_BUFFER_CONFLICT_LATENCY (390)

// TODO: RUN HISTOGRAM!
// Latency Threshold for Row Buffer Hit
#define ROW_BUFFER_HIT_LATENCY (290)

#endif
