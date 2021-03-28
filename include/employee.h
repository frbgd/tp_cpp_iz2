#ifndef TP_CPP_IZ2_EMPLOYEE_H
#define TP_CPP_IZ2_EMPLOYEE_H

#define POSITION_STR_LEN 16
#define SECOND_NAME_STR_LEN 23
#define FIRST_NAME_STR_LEN 16

struct employee {
    char position[POSITION_STR_LEN];        // 16 байт
    unsigned short age;                     // 2 байта
    unsigned short work_experience;         // 2 байта
    unsigned salary;                        // 4 байта
    char second_name[SECOND_NAME_STR_LEN];  // 23 байта
    _Bool is_male;                          // 1 байт
    char first_name[FIRST_NAME_STR_LEN];    // 16 байт
};                                          // 64 байта
typedef struct employee employee;

employee *employee_list;

unsigned read_employees_number_from_file(char *file_name);
unsigned read_employees_from_file(char *file_name, employee *employee_list);

#endif //TP_CPP_IZ2_EMPLOYEE_H
