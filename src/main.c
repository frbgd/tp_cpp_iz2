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

    

    free_array(&employee_list);
    return 0;
}