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
    for(int i = 0; i < employees_number; i++) {
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
    for (int i = 0; i < arr->used; i++) {
        if (strcmp(arr->array[i].position, target_position) == 0)
            return 1;
    }
    return 0;
}

void sort_by_second_name(employee_array *arr) {
    employee temp;

    for (int i=0; i <= arr->used; i++) {
        for (int j = i + 1; j <= arr->used; j++) {
            if (strcmp(arr->array[i].second_name, arr->array[j].second_name) > 0) {
                temp = arr->array[i];
                arr->array[i] = arr->array[j];
                arr->array[j] = temp;
            }
        }
    }
}

void search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    init_array(&positions, ARRAY_INIT_SIZE);
    for (int i = 0; i < empl_list->used; i++) {
        if (match_position(&positions, empl_list->array[i].position) == 0) {
            insert_array(&positions, empl_list->array[i]);
        }
    }

    // для каждой профессии ищем минимальный возраст
    for (int i = 0; i < positions.used; i++) {
        unsigned short min_age = 1000;
        employee min_age_employee;
        for (int j = 0; j < empl_list->used; j++) {
            if (strcmp(empl_list->array[j].position, positions.array[i].position) == 0
            && empl_list->array[j].age < min_age) {
                min_age_employee = empl_list->array[j];
                min_age = min_age_employee.age;
            }
        }
        if (min_age < 1000)
            insert_array(result, min_age_employee);
    }
    // для каждой профессии ищем максимальный возраст
    for (int i = 0; i < positions.used; i++) {
        unsigned short max_age = 0;
        employee max_age_employee;
        for (int j = 0; j < empl_list->used; j++) {
            if (strcmp(empl_list->array[j].position, positions.array[i].position) == 0
            && empl_list->array[j].age > max_age) {
                max_age_employee = empl_list->array[j];
                max_age = max_age_employee.age;
            }
        }
        if (max_age > 0)
            insert_array(result, max_age_employee);
    }
    sort_by_second_name(result);

    free_array(&positions);
}
