#include "../shared.hh"
#include "../util.hh"
#include "../params.hh"

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    uint64_t buffer_size_bytes = (uint64_t) BUFFER_SIZE_MB * (1024*1024);
    allocated_mem = allocate_pages(buffer_size_bytes);
    uint64_t* bank_lat_histogram = (uint64_t*) calloc((100+1), sizeof(uint64_t));
    
    const long int num_iterations = buffer_size_bytes / ROW_SIZE;
    char *base = (char *)allocated_mem;
    for (int i = 1; i < num_iterations; i++) {
        uint64_t time = 0;
        for (int j = 0; j < SAMPLES; j++) {
            time += measure_bank_latency((uint64_t)base, (uint64_t)(base + i * ROW_SIZE));
        }
        double avg_time = (double) ( time / (float) SAMPLES);
        bank_lat_histogram[(int) (avg_time / 10)]++;
    }

    // Follow Shubh's output file format
    printf("Total Number of pairs: %ld\n", num_iterations);
  
    puts("-------------------------------------------------------");
    printf("Latency(cycles)\t\tNumber-of-Address-Pair \n");
    puts("-------------------------------------------------------");

    for (int i=0; i<100 ;i++){
        printf("[%d-%d)   \t %15lu \n",
	    i*10, i*10 + 10, bank_lat_histogram[i]);
    }
    printf("%d+   \t %15lu \n",100*10, bank_lat_histogram[100]);
}