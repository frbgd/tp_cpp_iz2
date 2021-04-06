#include <string.h>
#include <stdlib.h>
#include "../include/employee.h"

int search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    if (init_array(&positions, ARRAY_INIT_SIZE) == -1) {
        return -1;
    }
    for (size_t empl_idx = 0; empl_idx < empl_list->used; empl_idx++) {
        int match = match_position(&positions, empl_list->array[empl_idx].position);
        if (match == 0) {
            if (insert_array(&positions, empl_list->array[empl_idx]) == -1) {
                return -1;
            }
        } else if (match == -1) {
            return -1;
        }
    }

    // для каждой профессии ищем минимальный возраст
    for (size_t positions_idx = 0; positions_idx < positions.used; positions_idx++) {
        employee *min_age_employee = malloc(sizeof(employee));
        if (find_min_age_employee(empl_list, positions.array[positions_idx].position, min_age_employee) != 0) {
            return -1;
        }
        if (insert_array(result, *min_age_employee) == -1) {
            return -1;
        }
    }
    // для каждой профессии ищем максимальный возраст
    for (size_t positions_idx = 0; positions_idx < positions.used; positions_idx++) {
        employee *max_age_employee = malloc(sizeof(employee));
        if(find_max_age_employee(empl_list, positions.array[positions_idx].position, max_age_employee) != 0) {
            return -1;
        }
        if (insert_array(result, *max_age_employee) == -1) {
            return -1;
        }
    }
    if (sort_by_second_name(result) == -1) {
        return -1;
    }

    if (free_array(&positions) == -1) {
        return -1;
    }

    return 0;
}
