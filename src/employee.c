#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/employee.h"

void init_array(employee_array *a, size_t initial_size) {
    a->array = malloc(initial_size * sizeof(employee));
    a->used = 0;
    a->size = initial_size;
}

void insert_array(employee_array *a, employee element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(employee));
    }
    a->array[a->used++] = element;
}

void slice_array(employee_array *input, employee_array *output, size_t start_index, size_t size) {
    for (size_t i = start_index; i < start_index + size; i++) {
        insert_array(output, input->array[i]);
    }
}

void free_array(employee_array *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

int read_employees_number_from_file(char *file_name) {
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    int employees_number = 0;
    if (fread(&employees_number, sizeof(unsigned), 1, f) != 1) {
        fprintf(stderr, "Failed to read employees number\n");
        fclose(f);
        return -1;
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return -1;
    }
    return employees_number;
}

int read_employees_from_file(char *file_name, int employees_number, employee_array *arr) {
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    fseek(f, FILE_HEADER_SIZE, SEEK_SET);
    for (int i = 0; i < employees_number; i++) {
        employee emp;
        if (fread(&emp, sizeof(employee), 1, f) != 1) {
            fprintf(stderr, "Failed to read employees\n");
            return -1;
        }
        insert_array(arr, emp);
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return -1;
    }
    return 0;
}

int match_position(employee_array *arr, char *target_position) {
    for (size_t i = 0; i < arr->used; i++) {
        if (strcmp(arr->array[i].position, target_position) == 0)
            return 1;
    }
    return 0;
}

void sort_by_second_name(employee_array *arr) {
    employee temp;

    for (size_t i = 0; i < arr->used; i++) {
        for (size_t j = i + 1; j < arr->used; j++) {
            if (strcmp(arr->array[i].second_name, arr->array[j].second_name) > 0) {
                temp = arr->array[i];
                arr->array[i] = arr->array[j];
                arr->array[j] = temp;
            }
        }
    }
}

employee find_min_age_employee(employee_array *empl_list, char *position) {
    unsigned short min_age = INIT_MIN_AGE;
    employee min_age_employee;

    for (size_t employees_idx = 0; employees_idx < empl_list->used; employees_idx++) {
        if (strcmp(empl_list->array[employees_idx].position, position) == 0
            && empl_list->array[employees_idx].age < min_age) {
            min_age_employee = empl_list->array[employees_idx];
            min_age = min_age_employee.age;
        }
    }
    return min_age_employee;
}

employee find_max_age_employee(employee_array *empl_list, char *position) {
    unsigned short max_age = INIT_MAX_AGE;
    employee max_age_employee;

    for (size_t employees_idx = 0; employees_idx < empl_list->used; employees_idx++) {
        if (strcmp(empl_list->array[employees_idx].position, position) == 0
            && empl_list->array[employees_idx].age > max_age) {
            max_age_employee = empl_list->array[employees_idx];
            max_age = max_age_employee.age;
        }
    }
    return max_age_employee;
}
