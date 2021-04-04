#ifndef INCLUDE_EMPLOYEE_H_
#define INCLUDE_EMPLOYEE_H_

#include <stdbool.h>

#define POSITION_STR_LEN 16
#define SECOND_NAME_STR_LEN 23
#define FIRST_NAME_STR_LEN 16

#define ARRAY_INIT_SIZE 10

#define FILE_HEADER_SIZE 16

#define INIT_MIN_AGE 1000;
#define INIT_MAX_AGE 0;

typedef struct {
    char position[POSITION_STR_LEN];        // 16 байт
    unsigned short age;                     // 2 байта
    unsigned short work_experience;         // 2 байта
    unsigned salary;                        // 4 байта
    char second_name[SECOND_NAME_STR_LEN];  // 23 байта
    bool is_male;                           // 1 байт
    char first_name[FIRST_NAME_STR_LEN];    // 16 байт
} employee;                                 // 64 байта
typedef struct {
    employee *array;
    size_t used;
    size_t size;
} employee_array;

void init_array(employee_array *a, size_t initial_size);
void insert_array(employee_array *a, employee element);
void slice_array(employee_array *input, employee_array *output, size_t start_index, size_t size);
void free_array(employee_array *a);

int read_employees_number_from_file(char *file_name);
int read_employees_from_file(char *file_name, int employees_number, employee_array *arr);

int match_position(employee_array *arr, char *target_position);
void sort_by_second_name(employee_array *arr);
int search(employee_array *empl_list, employee_array *result);

employee find_min_age_employee(employee_array *empl_list, char *position);
employee find_max_age_employee(employee_array *empl_list, char *position);

#endif  // INCLUDE_EMPLOYEE_H_
