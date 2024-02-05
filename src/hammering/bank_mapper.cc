#include "../shared.hh"
#include "../util.hh"
#include "../params.hh"
#include "stdlib.h"
#include <random>

std::map<uint64_t, uint64_t> physaddr_bankno_map;
std::map<uint64_t, std::vector<uint64_t>> bank_to_physaddr_map;

void get_bank_mapping(void * allocated_mem, uint64_t buffer_size_bytes) {

    const long int num_iterations = buffer_size_bytes / ROW_SIZE;
    uint64_t * base = (uint64_t *)allocated_mem;

    // Insert memory addresses into the map
    for (int i = 0; i < num_iterations; i++) { 
        
        // Set all to bank 0 initially
        uint64_t virt_addr = (uint64_t) base + i*ROW_SIZE;
        uint64_t phys_addr = virt_to_phys(virt_addr);
        physaddr_bankno_map[phys_addr] = 0;
        fprintf(stdout, "=========================================================\n");
        fprintf(stdout, "Phys Addr: {%lx}, Bank Preset: {%lu}\n", phys_addr, physaddr_bankno_map[phys_addr]);
        fprintf(stdout, "=========================================================\n");
    }


    // Iterate through the map and see if we can sort the rows
    for (auto addr_1 = physaddr_bankno_map.begin(); addr_1 != physaddr_bankno_map.end(); ++addr_1) {
        for (auto addr_2 = std::next(addr_1); addr_2 != physaddr_bankno_map.end(); ++addr_2) {
            uint64_t paddr1 = addr_1->first;
            uint64_t paddr2 = addr_2->first;
            uint64_t bank1 = addr_1->second;
            uint64_t bank2 = addr_2->second;
            fprintf(stdout, "=========================================================\n");
            fprintf(stdout, "Addr 1: Bank [%lu]: Phys Address: {%lx}\n", bank1, paddr1);
            fprintf(stdout, "Addr 2: Bank [%lu]: Phys Address: {%lx}\n", bank2, paddr2);
            fprintf(stdout, "=========================================================\n");

            // Extract virtual addresses i and j
            uint64_t vaddr1 = phys_to_virt(paddr1);
            uint64_t vaddr2 = phys_to_virt(paddr2);

            fprintf(stdout, "Vaddr1 [%lu]: Vaddr2: {%lx}", vaddr1, vaddr2);
            fprintf(stdout, "=========================================================\n");
            fprintf(stdout, "\n\n");

            if (vaddr1 == 0 || vaddr2 == 0) {
                continue;
            }

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
        uint64_t address = pair.first;
        uint64_t bank = pair.second;
        fprintf(stdout, "=========================================================\n");
        fprintf(stdout, "Bank [%lu]: Address: {%lx}\n", bank, address);
        fprintf(stdout, "=========================================================\n");
        fprintf(stdout, "\n\n");
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
            fprintf(stdout, "A: {%lx}, B: {%lx}, Latency: {%lu}. NOT IN SAME BANK DESPITE BEING SORTED AS SO", paddr_1, paddr_2, time);
        }
        if (time >= ROW_BUFFER_CONFLICT_LATENCY) {
            fprintf(stdout, "A: {%lx}, B: {%lx}, Latency: {%lu}. IN SAME BANK AND BEING SORTED AS SO", paddr_1, paddr_2, time);
        }

    }



}



int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    uint64_t mem_size = (uint64_t) (1.8 * BUFFER_SIZE_MB * (1024 * 1024));
    allocated_mem = allocate_pages(mem_size);
    setup_PPN_VPN_map(allocated_mem, mem_size);

    fprintf(stdout, "Setting Up Everything Complete\n");
    // Get the address to bank mapping completely filled up
    fprintf(stdout, "Getting Bank Mapping\n");
    get_bank_mapping(allocated_mem, mem_size);

    fprintf(stdout, "Reversing Bank to Address Mapping\n");
    // Reverse the a2b map to b2a.
    create_banktoaddr_map();


    fprintf(stdout, "TBD: Verify Bank to Address Mapping\n");
    for (uint64_t i= 0; i<NUM_BANKS; i++) {
        // Verify Bank mappings by taking samples
        //verify_same_bank(SAMPLES, i);
    }


}