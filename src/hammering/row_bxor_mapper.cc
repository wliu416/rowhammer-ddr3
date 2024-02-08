#include "../shared.hh"
#include "../util.hh"
#include "../params.hh"
#include "stdlib.h"
#include <random>

std::map<uint64_t, uint64_t> physaddr_bankno_map;
std::map<uint64_t, std::vector<uint64_t>> bank_to_physaddr_map;

// void test_bitxor_row(uint64_t rowaddr, uint64_t * offset) {
// 
// }


int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    uint64_t mem_size = (uint64_t) (0.25 * BUFFER_SIZE_MB * (1024 * 1024));
    allocated_mem = allocate_pages(mem_size);
    setup_PPN_VPN_map(allocated_mem, mem_size);


    fprintf(stdout, "Running Row Bit XOR Testing.\n");

    const long int num_iterations = mem_size / ROW_SIZE;
    char *base = (char *)allocated_mem;
    for (int i = 1; i < num_iterations-8; i++) {

        //if (i >= 15000) {
            // Run only 15000 rows for sanity
            // break;
        //}

        uint64_t curr_row = (uint64_t ) (base + i * ROW_SIZE);
        uint64_t curr_row_paddr = virt_to_phys(curr_row);
        fprintf(stdout, "**************************************************\n");
        fprintf(stdout, "<Current Row: %lx>: Physical Address: [%lx]\n", curr_row, curr_row_paddr);
        fprintf(stdout, "**************************************************\n");

        for (int k = 1; k <= 8; k++) {

            uint64_t bxor_col_bits = (uint64_t) (curr_row_paddr & 0xFFFF);
            uint64_t conflict_row_no = (uint64_t) ((curr_row_paddr >> 16) + k);
            uint64_t conflict_row_paddr = (conflict_row_no << 16) + bxor_col_bits; 
            uint64_t conflict_row = phys_to_virt(conflict_row_paddr);


            fprintf(stdout, "=========================================================\n");

            fprintf(stdout, "Combining Col Bits: [%lx], Row No: [%lx], Reassembled PAddr: {%lx}\n", bxor_col_bits, conflict_row_no, conflict_row_paddr);

            if (conflict_row == 0) {
                fprintf(stdout, "Created address Does Not exist and is 0. {%lx}\n\n", conflict_row_paddr);
                fprintf(stdout, "=========================================================\n");
                continue;
            }

            uint64_t time = 0;

            for (int j = 0; j < SAMPLES; j++) {
                time += measure_bank_latency((uint64_t)curr_row, (uint64_t)(conflict_row));
            }

            double avg_time = (double) (time / (float) SAMPLES);

            if (avg_time >= ROW_BUFFER_CONFLICT_LATENCY) {
                fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
                fprintf(stdout, "Row Bit +{%d}: \n", k);
                fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
                fprintf(stdout, "Timing >= 390 Cycles: CONFLICT: [%f] Cycles \n", avg_time);

            } else {
                fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
                fprintf(stdout, "Row Bit +{%d}: \n", k);
                fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
                fprintf(stdout, "Timing < 390 Cycles: NO CONFLICT: [%f] Cycles \n", avg_time);
            }
            fprintf(stdout, "=========================================================\n");
            //sleep(1); // Delay a bit
        }
        
        
    }


    fprintf(stdout, "Keeping Row, Col Standard. Flipping BXOR Bits from bank 0-7...\n");
    for (int i = 1; i < num_iterations-8; i++) {

        uint64_t curr_row = (uint64_t) (base + i * ROW_SIZE);
        uint64_t curr_row_paddr = virt_to_phys(curr_row);
        fprintf(stdout, "**************************************************\n");
        fprintf(stdout, "<Current Row: %ld>: Physical Address: [%ld]\n", curr_row, curr_row_paddr);
        fprintf(stdout, "**************************************************\n");

        for (int k = 1; k <= 8; k++) {

            uint64_t row_bits = curr_row_paddr >> 16; // Bit 16+ is for row bits
            uint64_t column_bits = curr_row_paddr & 0x1fff; // 13 col bits

            int bank_xor_bits = (curr_row_paddr >> 13) & 0x7; // Get Bank XOR Bits
            int bank = bank_xor_bits ^ (curr_row_paddr & 0x7);

            uint64_t bxor_bits = (uint64_t) ((bank_xor_bits + k)%8); // Test all 8 flips so use mod 8
            uint64_t conflict_row_paddr = (row_bits << 16) + (bxor_bits << 13) + column_bits; 
            uint64_t conflict_row = phys_to_virt(conflict_row_paddr);

            fprintf(stdout, "=========================================================\n");

            fprintf(stdout, "Combining Col Bits: [%lx], Row Bits: [%lx], BXOR Bits: [%lx], Reassembled PAddr: {%lx}\n", column_bits, row_bits, bxor_bits, conflict_row_paddr);


            if (conflict_row == 0) {
                fprintf(stdout, "Created address Does Not exist and is 0. {%lx}\n\n", conflict_row_paddr);
                fprintf(stdout, "=========================================================\n");
                continue;
            }

            uint64_t time = 0;

            for (int j = 0; j < SAMPLES; j++) {
                time += measure_bank_latency((uint64_t)curr_row, (uint64_t)(conflict_row));
            }

            double avg_time = (double) (time / (float) SAMPLES);

            if (avg_time >= ROW_BUFFER_CONFLICT_LATENCY) {
                fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
                fprintf(stdout, "BXOR Bit +{%d}. BXOR bits now [%ld] \n", k, bxor_bits);
                fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
                fprintf(stdout, "Timing >= 390 Cycles: CONFLICT: [%f] Cycles \n", avg_time);

            } else {
                fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
                fprintf(stdout, "BXOR Bit +{%d}. BXOR bits now [%ld] \n", k, bxor_bits);
                fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
                fprintf(stdout, "Timing < 390 Cycles: NO CONFLICT: [%f] Cycles \n", avg_time);
            }
            fprintf(stdout, "=========================================================\n");
            //sleep(1); // Delay a bit
        }
        
        
    }

}