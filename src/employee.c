#include <stdio.h>
#include <stdlib.h>
#include "../include/employee.h"

unsigned read_employees_number_from_file(char *file_name) {
    FILE *f = fopen(file_name, "rb");

    if (!f) {
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    unsigned employees_number = 0;
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

unsigned read_employee_from_file(char *file_name, employee *employee_list) {
    FILE *f = fopen(file_name, "rb");

    if (!f) {
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    unsigned employees_number = 0;
    if (fread(&employees_number, sizeof(unsigned), 1, f) != 1) {
        fprintf(stderr, "Failed to read employees number\n");
        return -1;
    }
    char ee[12];
    if (fread(&ee, sizeof(char) * 12, 1, f) != 1) {
        fprintf(stderr, "Failed to read ee\n");
        return -1;
    }

    for(unsigned i = 0; i < employees_number; i++) {
        if (fread(&employee_list[i], sizeof(employee), 1, f) != 1) {
            fprintf(stderr, "Failed to read employees\n");
            return -1;
        }
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return -1;
    }

    return employees_number;
}
