#include "../shared.hh"
#include "../util.hh"
#include "../params.hh"


/**
 * Since we load an entire row into the cache, need to flush every 64 bits
*/
void clflush_row(uint8_t *row_ptr) {
    for (uint32_t index = 0; index < ROW_SIZE; index += 64) {
        clflush((uint64_t)(row_ptr + index));
    }
}
/**
 * Another method to flush a lot of things in the cacheline.
*/
void clflush_area(uint8_t *start_ptr, uint64_t size) {
    for (uint32_t index = 0; index < size; index += 64) {
        clflush((uint64_t)(start_ptr + index));
    }
}
/**
 * TODO: Cache line flushing via eviction
 * Evict the cache line not using clflush, but by overwriting it with irrelevant addresses
*/
void cache_flush_eviction_method(uint64_t cacheline_size, ) {
    for (uint32_t index = 0; index < cacheline_size; index ++) {
        // TODO: for 64 bit cache lines, flush it all by loading a bunch of irrelevant, different bank addresses.
    }
}



