#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 8
#define COLS 8

// Memory structure
typedef struct {
    int memory[ROWS][COLS];
    int row_buffer[COLS];
} MemoryArray;

// Function to initialize memory with random binary values
void init_memory(MemoryArray *mem) {
    srand(time(NULL));
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            mem->memory[i][j] = rand() % 2;
        }
    }
}

// Function to display memory contents
void display_memory(MemoryArray *mem) {
    printf("\nMemory Array:\n");
    for(int i = 0; i < ROWS; i++) {
        printf("Row %d: ", i);
        for(int j = 0; j < COLS; j++) {
            printf("%d ", mem->memory[i][j]);
        }
        printf("\n");
    }
    
    printf("\nRow Buffer: ");
    for(int j = 0; j < COLS; j++) {
        printf("%d ", mem->row_buffer[j]);
    }
    printf("\n");
}

// Step 1: Activate row and transfer to buffer
void activate_and_transfer_to_buffer(MemoryArray *mem, int row_a) {
    if(row_a >= 0 && row_a < ROWS) {
        printf("\nStep 1: Activating Row %d and transferring to buffer\n", row_a);
        for(int j = 0; j < COLS; j++) {
            mem->row_buffer[j] = mem->memory[row_a][j];
        }
        printf("Data transferred to buffer\n");
    } else {
        printf("Invalid row number\n");
    }
}

// Step 2: Transfer from buffer to target row
void transfer_from_buffer(MemoryArray *mem, int row_b) {
    if(row_b >= 0 && row_b < ROWS) {
        printf("\nStep 2: Activating Row %d and transferring from buffer\n", row_b);
        for(int j = 0; j < COLS; j++) {
            mem->memory[row_b][j] = mem->row_buffer[j];
        }
        printf("Data transferred from buffer to Row %d\n", row_b);
    } else {
        printf("Invalid row number\n");
    }
}

// Complete row clone operation
void simulate_row_clone(MemoryArray *mem, int source_row, int target_row) {
    printf("\nSimulating Row Clone from Row %d to Row %d\n", source_row, target_row);
    printf("Initial state:");
    display_memory(mem);
    
    // Step 1: Source to buffer
    activate_and_transfer_to_buffer(mem, source_row);
    
    // Step 2: Buffer to target
    transfer_from_buffer(mem, target_row);
    
    printf("\nFinal state:");
    display_memory(mem);
}

int main() {
    MemoryArray mem = {0}; // Initialize memory array to zero
    
    // Initialize memory with random binary values
    init_memory(&mem);
    
    // Perform row clone operation (e.g., from row 0 to row 3)
    simulate_row_clone(&mem, 0, 3);
    
    return 0;
}