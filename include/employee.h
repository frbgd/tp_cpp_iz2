#ifndef TP_CPP_IZ2_EMPLOYEE_H
#define TP_CPP_IZ2_EMPLOYEE_H

#define POSITION_STR_LEN 16
#define SECOND_NAME_STR_LEN 23
#define FIRST_NAME_STR_LEN 16

#define FILE_HEADER_SIZE 16

struct employee {
    char position[POSITION_STR_LEN];        // 16 байт
    unsigned short age;                     // 2 байта
    unsigned short work_experience;         // 2 байта
    unsigned salary;                        // 4 байта
    char second_name[SECOND_NAME_STR_LEN];  // 23 байта
    _Bool is_male;                          // 1 байт
    char first_name[FIRST_NAME_STR_LEN];    // 16 байт
};                                          // 64 байта
struct array {
    struct employee *array;
    size_t used;
    size_t size;
};
typedef struct employee employee;
typedef struct array array;

void init_array(array *a, size_t initialSize);
void insert_array(array *a, employee element);
void free_array(array *a);

int read_employees_number_from_file(char *file_name);
int read_employees_from_file(char *file_name, int employees_number, array *employee_list);


#endif //TP_CPP_IZ2_EMPLOYEE_H
