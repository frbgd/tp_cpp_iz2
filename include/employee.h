#ifndef TP_CPP_IZ2_EMPLOYEE_H
#define TP_CPP_IZ2_EMPLOYEE_H

#define POSITION_STR_LEN 16
#define SECOND_NAME_STR_LEN 23
#define FIRST_NAME_STR_LEN 16

#define ARRAY_INIT_SIZE 10

#define FILE_HEADER_SIZE 16

#define INIT_MIN_AGE 1000;
#define INIT_MAX_AGE 0;

struct employee {
    char position[POSITION_STR_LEN];        // 16 байт
    unsigned short age;                     // 2 байта
    unsigned short work_experience;         // 2 байта
    unsigned salary;                        // 4 байта
    char second_name[SECOND_NAME_STR_LEN];  // 23 байта
    _Bool is_male;                          // 1 байт
    char first_name[FIRST_NAME_STR_LEN];    // 16 байт
};                                          // 64 байта
struct employee_array {
    struct employee *array;
    size_t used;
    size_t size;
};
typedef struct employee employee;
typedef struct employee_array employee_array;

void init_array(employee_array *a, size_t initialSize);
void insert_array(employee_array *a, employee element);
void free_array(employee_array *a);

int read_employees_number_from_file(char *file_name);
int read_employees_from_file(char *file_name, int employees_number, employee_array *employee_list);

int match_position(employee_array *arr, char *target_position);
void sort_by_second_name(employee_array *arr);
void search(employee_array *empl_list, employee_array *result);

#endif //TP_CPP_IZ2_EMPLOYEE_H
