#include <stdio.h>
#include <stdlib.h>
#include "../include/employee.h"

int main() {
    char *file_name = "test_data.bin";
    int employees_number = read_employees_number_from_file(file_name);
    if (employees_number < 0) {
        return -1;
    }
    employee_array employee_list;
    init_array(&employee_list, employees_number);
    if (read_employees_from_file(file_name, employees_number, &employee_list) != 0) {
        return -1;
    }

    struct employee_array search_result;
    init_array(&search_result, ARRAY_INIT_SIZE);
    search(&employee_list, &search_result);

    for(int i = 0; i < search_result.used; i++){
        printf("%s %d %d %d %s %d %s\n",
               search_result.array[i].position, search_result.array[i].age,
               search_result.array[i].work_experience, search_result.array[i].salary,
               search_result.array[i].second_name, search_result.array[i].is_male,
               search_result.array[i].first_name);
    }

    free_array(&search_result);
    free_array(&employee_list);
    return 0;
}