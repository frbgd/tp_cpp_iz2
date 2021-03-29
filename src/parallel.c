#include <string.h>
#include "../include/employee.h"

void search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    init_array(&positions, ARRAY_INIT_SIZE);
    for (size_t i = 0; i < empl_list->used; i++) {
        if (match_position(&positions, empl_list->array[i].position) == 0) {
            insert_array(&positions, empl_list->array[i]);
        }
    }

    // параллельно ищем минимального и максимального сотрудника


    sort_by_second_name(result);
    free_array(&positions);
}