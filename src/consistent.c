#include <string.h>
#include "../include/employee.h"

int search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    init_array(&positions, ARRAY_INIT_SIZE);
    for (size_t i = 0; i < empl_list->used; i++) {
        if (match_position(&positions, empl_list->array[i].position) == 0) {
            insert_array(&positions, empl_list->array[i]);
        }
    }

    // для каждой профессии ищем минимальный возраст
    for (size_t i = 0; i < positions.used; i++) {
        unsigned short min_age = INIT_MIN_AGE;
        employee min_age_employee;
        for (size_t j = 0; j < empl_list->used; j++) {
            if (strcmp(empl_list->array[j].position, positions.array[i].position) == 0
                && empl_list->array[j].age < min_age) {
                min_age_employee = empl_list->array[j];
                min_age = min_age_employee.age;
            }
        }
        insert_array(result, min_age_employee);
    }
    // для каждой профессии ищем максимальный возраст
    for (size_t i = 0; i < positions.used; i++) {
        unsigned short max_age = INIT_MAX_AGE;
        employee max_age_employee;
        for (size_t j = 0; j < empl_list->used; j++) {
            if (strcmp(empl_list->array[j].position, positions.array[i].position) == 0
                && empl_list->array[j].age > max_age) {
                max_age_employee = empl_list->array[j];
                max_age = max_age_employee.age;
            }
        }
        insert_array(result, max_age_employee);
    }
    sort_by_second_name(result);

    free_array(&positions);

    return 0;
}
