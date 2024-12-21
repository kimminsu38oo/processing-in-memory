#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 8
#define COLS 8

typedef struct {
    int memory[ROWS][COLS];
    int row_buffer[COLS];
    int R0[COLS];  // Reserved zero row
    int R1[COLS];  // Reserved one row
    int D1[COLS];  // Designated row 1
    int D2[COLS];  // Designated row 2
    int D3[COLS];  // Designated row 3
} BulkMemoryArray;

// Initialize memory with random binary values
void init_bulk_memory(BulkMemoryArray *mem) {
    srand(time(NULL));
    
    // Initialize main memory
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            mem->memory[i][j] = rand() % 2;
        }
    }
    
    // Initialize R0 (all zeros)
    for(int j = 0; j < COLS; j++) {
        mem->R0[j] = 0;
    }
    
    // Initialize R1 (all ones)
    for(int j = 0; j < COLS; j++) {
        mem->R1[j] = 1;
    }
}

void display_bulk_memory(BulkMemoryArray *mem) {
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
    
    printf("\nReserved Rows:\n");
    printf("R0 (Zero): ");
    for(int j = 0; j < COLS; j++) printf("%d ", mem->R0[j]);
    printf("\nR1 (One): ");
    for(int j = 0; j < COLS; j++) printf("%d ", mem->R1[j]);
    
    printf("\nDesignated Rows:\n");
    printf("D1: ");
    for(int j = 0; j < COLS; j++) printf("%d ", mem->D1[j]);
    printf("\nD2: ");
    for(int j = 0; j < COLS; j++) printf("%d ", mem->D2[j]);
    printf("\nD3: ");
    for(int j = 0; j < COLS; j++) printf("%d ", mem->D3[j]);
    printf("\n");
}

// Step 1: Activate row and transfer to buffer
void activate_and_transfer_to_buffer(BulkMemoryArray *mem, int row_a) {
    if(row_a >= 0 && row_a < ROWS) {
        //printf("\nStep 1: Activating Row %d and transferring to buffer\n", row_a);
        for(int j = 0; j < COLS; j++) {
            mem->row_buffer[j] = mem->memory[row_a][j];
        }
        //printf("Data transferred to buffer\n");
    } else {
        printf("Invalid row number\n");
    }
}

void row_clone(BulkMemoryArray *mem, int source_row, int *target) {
    printf("\nCloning Row %d\n", source_row);
    // First activate and transfer to buffer
    activate_and_transfer_to_buffer(mem, source_row);
    // Then copy from buffer to target
    for(int j = 0; j < COLS; j++) {
        target[j] = mem->row_buffer[j];
    }
}

// AND Operation (D3 = 0):
// D1  D2  D3(0) | Voltage Level  | Compare to 1/2Vdd | DRAM Result
// 0   0    0    | 0Vdd/3 = 0V    | 0 < 1/2Vdd       | 0
// 0   1    0    | 1Vdd/3         | 1/3 < 1/2Vdd     | 0
// 1   0    0    | 1Vdd/3         | 1/3 < 1/2Vdd     | 0
// 1   1    0    | 2Vdd/3         | 2/3 > 1/2Vdd     | 1
void bulk_and_operation(BulkMemoryArray *mem, int row_a, int row_b, int result_row) {
    printf("\nPerforming BULK AND between Row %d and Row %d\n", row_a, row_b);
    
    // Step 1: Clone source rows to designated rows
    row_clone(mem, row_a, mem->D1);
    row_clone(mem, row_b, mem->D2);
    for(int j = 0; j < COLS; j++) {
        mem->D3[j] = mem->R0[j];  // Clone R0 to D3 (all zeros)
    }
    
    // Step 2: Perform triple row activation - voltage-based comparison
    for(int j = 0; j < COLS; j++) {
        int ones_count = mem->D1[j] + mem->D2[j] + mem->D3[j];
        // Voltage comparison: (ones_count × Vdd/3) > 1/2Vdd
        mem->D3[j] = (ones_count * 2 > 3) ? 1 : 0;
    }

    // Step 3: Store result
    printf("\nStoring result in Row %d\n", result_row);
    for(int j = 0; j < COLS; j++) {
        mem->memory[result_row][j] = mem->D3[j];
    }
}

// OR Operation (D3 = 1):
// D1  D2  D3(1) | Voltage Level  | Compare to 1/2Vdd | DRAM Result
// 0   0    1    | 1Vdd/3         | 1/3 < 1/2Vdd     | 0
// 0   1    1    | 2Vdd/3         | 2/3 > 1/2Vdd     | 1
// 1   0    1    | 2Vdd/3         | 2/3 > 1/2Vdd     | 1
// 1   1    1    | 3Vdd/3 = Vdd   | 1 > 1/2Vdd       | 1
void bulk_or_operation(BulkMemoryArray *mem, int row_a, int row_b, int result_row) {
    printf("\nPerforming BULK OR between Row %d and Row %d\n", row_a, row_b);
    
    // Step 1: Clone source rows to designated rows
    row_clone(mem, row_a, mem->D1);
    row_clone(mem, row_b, mem->D2);
    for(int j = 0; j < COLS; j++) {
        mem->D3[j] = mem->R1[j];  // Clone R1 to D3 (all ones)
    }
    
    // Step 2: Perform triple row activation - voltage-based comparison
    for(int j = 0; j < COLS; j++) {
        int ones_count = mem->D1[j] + mem->D2[j] + mem->D3[j];
        // Voltage comparison: (ones_count × Vdd/3) > 1/2Vdd
        mem->D3[j] = (ones_count * 2 > 3) ? 1 : 0;
    }
    
    // Step 3: Store result
    printf("\nStoring result in Row %d\n", result_row);
    for(int j = 0; j < COLS; j++) {
        mem->memory[result_row][j] = mem->D3[j];
    }
}

int main() {
    BulkMemoryArray mem = {0};
    init_bulk_memory(&mem);
    
    printf("Initial state:");
    display_bulk_memory(&mem);
    
    // Test AND operation
    bulk_and_operation(&mem, 0, 1, 2);
    printf("\nAfter AND operation:");
    display_bulk_memory(&mem);
    
    // Test OR operation
    bulk_or_operation(&mem, 3, 4, 5);
    printf("\nAfter OR operation:");
    display_bulk_memory(&mem);
    
    return 0;
}