#include <string.h>
#include "../include/employee.h"

int search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    init_array(&positions, ARRAY_INIT_SIZE);
    for (size_t empl_idx = 0; empl_idx < empl_list->used; empl_idx++) {
        if (match_position(&positions, empl_list->array[empl_idx].position) == 0) {
            insert_array(&positions, empl_list->array[empl_idx]);
        }
    }

    // для каждой профессии ищем минимальный возраст
    for (size_t positions_idx = 0; positions_idx < positions.used; positions_idx++) {
        employee min_age_employee = find_min_age_employee(empl_list, positions.array[positions_idx].position);
        insert_array(result, min_age_employee);
    }
    // для каждой профессии ищем максимальный возраст
    for (size_t positions_idx = 0; positions_idx < positions.used; positions_idx++) {
        employee max_age_employee = find_max_age_employee(empl_list, positions.array[positions_idx].position);
        insert_array(result, max_age_employee);
    }
    sort_by_second_name(result);

    free_array(&positions);

    return 0;
}
