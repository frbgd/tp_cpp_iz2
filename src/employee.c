#include <stdio.h>
#include <stdlib.h>
#include "../include/employee.h"

int read_employees_number_from_file(char *file_name) {
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    int employees_number = 0;
    if (fread(&employees_number, sizeof(unsigned), 1, f) != 1) {
        fprintf(stderr, "Failed to read employees number\n");
        return -1;
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return -1;
    }
    return employees_number;
}

int read_employees_from_file(char *file_name, int employees_number, employee *employee_list) {
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    fseek(f, FILE_HEADER_SIZE, SEEK_SET);
    for(int i = 0; i < employees_number; i++) {
        if (fread(&employee_list[i], sizeof(employee), 1, f) != 1) {
            fprintf(stderr, "Failed to read employees\n");
            return -1;
        }
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return -1;
    }
    return 0;
}
