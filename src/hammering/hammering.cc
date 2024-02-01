#include "../shared.hh"
#include "../util.hh"
#include "../params.hh"
#include "stdlib.h"
#include <random>

std::map<uint64_t, uint64_t> physaddr_bankno_map;
std::map<uint64_t, std::vector<uint64_t>> bank_to_physaddr_map;


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

    const long int num_iterations = buffer_size_bytes / ROW_SIZE;
    uint64_t * base = (uint64_t *)allocated_mem;

    // Insert memory addresses into the map
    for (int i = 0; i < num_iterations; i++) { 
        
        // Set all to bank 0 initially
        uint64_t virt_addr = (uint64_t) base + i*ROW_SIZE;
        uint64_t phys_addr = virt_to_phys(virt_addr);
        physaddr_bankno_map[phys_addr] = 0;
    }


    // Iterate through the map and see if we can sort the rows
    for (auto addr_1 = physaddr_bankno_map.begin(); addr_1 != physaddr_bankno_map.end(); ++addr_1) {
        for (auto addr_2 = std::next(addr_1); addr_2 != physaddr_bankno_map.end(); ++addr_2) {
            uint64_t paddr1 = addr_1->first;
            uint64_t paddr2 = addr_2->first;
            uint64_t bank1 = addr_1->second;
            uint64_t bank2 = addr_2->second;

            // Extract virtual addresses i and j
            uint64_t vaddr1 = phys_to_virt(paddr1);
            uint64_t vaddr2 = phys_to_virt(paddr2);

            uint64_t time = measure_bank_latency(vaddr1 , vaddr2);
            
            // TODO: Shubh uses <600. Why?
            if (time >= ROW_BUFFER_CONFLICT_LATENCY ) {
                addr_2->second = bank1;
            } else {
                // Increment the bank number and ensure it stays within the range of 0 to 7
                addr_2->second = (bank2 + 1) % NUM_BANKS; // Modulus 8 ensures it stays within 0 to 7 range

            }
            
        }
        
    }
    
}

void create_banktoaddr_map() {

    // Initialize Arrays for bank to phys addr map
    for (uint64_t i= 0; i<NUM_BANKS; i++) {
        bank_to_physaddr_map[i] = std::vector<uint64_t>();
    }

    for (const auto& pair : physaddr_bankno_map) {
        int address = pair.first;
        int bank = pair.second;
        // Check if the bank already exists in the bank-to-addresses map
        
        // Add the address to the vector of addresses for the corresponding bank
        bank_to_physaddr_map[bank].push_back(address);
    }
}


void verify_same_bank(uint64_t samples, uint64_t bank_no) {
    std::vector<uint64_t> bank_addrs = bank_to_physaddr_map[bank_no];

    // Take 10 random elements
    for (int i = 0; i < samples; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, bank_addrs.size() - 1);

        int index1 = dis(gen); // Generate a random index
        uint64_t paddr_1 = bank_addrs[index1]; 
        int index2 = dis(gen); // Generate a random index
        uint64_t paddr_2 = bank_addrs[index2]; 

        // Extract virtual addresses i and j
        uint64_t vaddr1 = phys_to_virt(paddr_1);
        uint64_t vaddr2 = phys_to_virt(paddr_2);

        uint64_t time = measure_bank_latency(vaddr1 , vaddr2);

        if (time >= ROW_BUFFER_HIT_LATENCY && time < ROW_BUFFER_CONFLICT_LATENCY) {
            fprintf(stdout, "A: {%lu}, B: {%lu}, Latency: {%lu}. NOT IN SAME BANK DESPITE BEING SORTED AS SO", paddr_1, paddr_2, time);
        }
        if (time >= ROW_BUFFER_CONFLICT_LATENCY) {
            fprintf(stdout, "A: {%lu}, B: {%lu}, Latency: {%lu}. IN SAME BANK AND BEING SORTED AS SO", paddr_1, paddr_2, time);
        }

    }



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