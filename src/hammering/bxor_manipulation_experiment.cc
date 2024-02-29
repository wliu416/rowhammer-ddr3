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

    fprintf(stdout, "**************************************************\n");
    fprintf(stdout, "Keeping BXOR Bits. Keeping 16-18 Constant. Move 19-21.\n");
    fprintf(stdout, "**************************************************\n");

    const long int num_iterations = mem_size / ROW_SIZE;
    char *base = (char *)allocated_mem;
    // for (int i = 1; i < num_iterations -8; i++) {

    //     uint64_t curr_row = (uint64_t) base + i * ROW_SIZE;
    //     uint64_t curr_row_paddr = virt_to_phys(curr_row);

    //     for (int k = 1; k <= 8; k++) {

    //         fprintf(stdout, "=========================================================\n");
            
    //         // Mask first 0-13 COL, 13-15 BXOR, 16-18 Row BXOR bits.
    //         uint64_t bxor_col_bits = (uint64_t) (curr_row_paddr & 0x7FFFF);
    //         uint64_t conflict_row_no = (uint64_t) ((curr_row_paddr >> 19) + k);
    //         uint64_t conflict_row_paddr = (conflict_row_no << 19) + bxor_col_bits; 
    //         uint64_t conflict_row = phys_to_virt(conflict_row_paddr);

    //         fprintf(stdout, "Combining Row Bits: [%lx], BXOR + Col Bits: [%lx], Reassembled PAddr: {%lx}\n", conflict_row_no, bxor_col_bits, conflict_row_paddr);

    //         if (conflict_row == 0) {
    //             fprintf(stdout, "Address Does Not exist and is 0. {%lx}\n\n", curr_row_paddr);
    //             fprintf(stdout, "=========================================================\n");
    //             continue;
    //         }

    //         uint64_t time = 0;

    //         for (int j = 0; j < SAMPLES; j++) {
    //             time += measure_bank_latency((uint64_t)curr_row, (uint64_t)(conflict_row));
    //         }

    //         double avg_time = (double) (time / (float) SAMPLES);

            
    //         if (avg_time >= ROW_BUFFER_CONFLICT_LATENCY) {
    //             fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
    //             fprintf(stdout, "Row Bit 19-21 +{%d}: \n", k);
    //             fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
    //             fprintf(stdout, "Timing >= 390 Cycles: CONFLICT: [%f] Cycles \n", avg_time);

    //         } else {
    //             fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
    //             fprintf(stdout, "Row Bit 19-21 +{%d}: \n", k);
    //             fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
    //             fprintf(stdout, "Timing < 390 Cycles: NO CONFLICT: [%f] Cycles \n", avg_time);
    //         }
    //         fprintf(stdout, "=========================================================\n");
    //     }
        
        
    // }

    fprintf(stdout, "**************************************************\n");
    fprintf(stdout, "Reverse Engineering. Change bit 16-18. Reverse engineer BXOR bits.\n");
    fprintf(stdout, "**************************************************\n");
    for (int i = 1; i < num_iterations - 8; i++) {

        uint64_t curr_row = (uint64_t) base + i * ROW_SIZE;
        uint64_t curr_row_paddr = virt_to_phys(curr_row);

        for (int k = 1; k <= 8; k++) {

            fprintf(stdout, "=========================================================\n");

            uint64_t row_bits = curr_row_paddr >> 16; // Bit 16+ is for row bits
            uint64_t column_bits = curr_row_paddr & 0x1fff; // 13 col bits

            int bank_xor_bits = (curr_row_paddr >> 13) & 0x7; // Get Bank XOR Bits
            int bank = bank_xor_bits ^ (curr_row_paddr & 0x7);

            
            uint64_t new_row_bits = (uint64_t) (row_bits + k);
            int bits_row_bank_xor = (new_row_bits & 0x7);

            int new_bank_xor_bits = bank ^ (new_row_bits & 0x7);

            uint64_t conflict_row_paddr = (new_row_bits << 16) + (new_bank_xor_bits << 13) + column_bits; 
            uint64_t conflict_row = phys_to_virt(conflict_row_paddr);

            fprintf(stdout, "Combining Col Bits: [%lx], Row Bits: [%lx], BXOR Bits: [%x], Reassembled PAddr: {%lx}\n", column_bits, new_row_bits, new_bank_xor_bits, conflict_row_paddr);

            if (conflict_row == 0) {
                fprintf(stdout, "Address Does Not exist and is 0. {%lx}\n\n", curr_row_paddr);
                fprintf(stdout, "=========================================================\n");
                continue;
            }

            uint64_t time = 0;

            for (int j = 0; j < SAMPLES; j++) {
                time += measure_bank_latency((uint64_t)curr_row, (uint64_t)(conflict_row));
            }

            double avg_time = (double) (time / (float) SAMPLES);

            fprintf(stdout, "=========================================================\n");
            if (avg_time >= ROW_BUFFER_CONFLICT_LATENCY) {
                fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
                fprintf(stdout, "Row Bit +{%d}. New BXOR bits now [%d] \n", k, new_bank_xor_bits);
                fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
                fprintf(stdout, "Timing >= 390 Cycles: CONFLICT: [%f] Cycles \n", avg_time);

            } else {
                fprintf(stdout, "Addr Original: {%lx}\n", curr_row_paddr);
                fprintf(stdout, "Row Bit +{%d}. New BXOR bits now [%d] \n", k, new_bank_xor_bits);
                fprintf(stdout, "Addr Changed: {%lx}\n", conflict_row_paddr);
                fprintf(stdout, "Timing < 390 Cycles: NO CONFLICT: [%f] Cycles \n", avg_time);
            }
            fprintf(stdout, "=========================================================\n");
        }
        
        
    }

}