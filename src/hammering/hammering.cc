#include "../shared.hh"
#include "../util.hh"
#include "../params.hh"


/**
 * Since we load an entire row into the cache, need to flush every 64 bits
*/
void clflush_row(uint8_t *row_ptr) {
    for (uint32_t index = 0; index < ROW_SIZE; index += 64) {
        clflush((row_ptr + index));
    }
}
/**
 * Another method to flush a lot of things in the cacheline.
*/
void clflush_area(uint8_t *start_ptr, uint64_t size) {
    for (uint32_t index = 0; index < size; index += 64) {
        clflush((start_ptr + index));
    }
}
/**
 * TODO: Cache line flushing via eviction
 * Evict the cache line not using clflush, but by overwriting it with irrelevant addresses
*/
void cache_flush_eviction_method(uint64_t cacheline_size, uint64_t start_address) {
    for (uint32_t index = 0; index < cacheline_size; index ++) {
        // TODO: for 64 bit cache lines, flush it all by loading a bunch of irrelevant, different bank addresses.
    }
}


void get_bank_mapping(void * allocated_mem, uint64_t buffer_size_bytes) {
}



int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    uint64_t mem_size = 1.8 * BUFFER_SIZE_MB * 1024 * 1024;
    allocated_mem = allocate_pages(mem_size);
    setup_PPN_VPN_map(allocated_mem, mem_size);

    uint64_t victim; 
    uint64_t* attacker_1 = (uint64_t*) calloc(1, sizeof(uint64_t));
    uint64_t* attacker_2 = (uint64_t*) calloc(1, sizeof(uint64_t)); 

}