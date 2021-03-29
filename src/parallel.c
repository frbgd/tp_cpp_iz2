#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <wait.h>
#include <stdio.h>
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

    // параллелим алгоритм по количеству найденных профессий
    int processes_number = positions.used;
    int pipe_arr[processes_number][2];

    int* pid_list = malloc(processes_number * sizeof(int));
    int pid = -1, status, pid_list_iter = 0;
    for (int i = 0; i < processes_number; i++) {
        if (pipe(pipe_arr[i]) == -1){
            // TODO return with panic
        }

        pid = fork();
        if (pid > 0) {
            pid_list[pid_list_iter++] = pid;
        } else if (pid == 0) {
            // выделяем локальный массив результатов
            employee proc_result[2];
            char target_position[POSITION_STR_LEN];
            strcpy(target_position, positions.array[i].position);

            // ищем минимального сотрудника для текущей профессии (под индексом i)
            unsigned short min_age = INIT_MIN_AGE;
            employee min_age_employee;
            for (size_t j = 0; j < empl_list->used; j++) {
                if (strcmp(empl_list->array[j].position, target_position) == 0
                    && empl_list->array[j].age < min_age) {
                    min_age_employee = empl_list->array[j];
                    min_age = min_age_employee.age;
                }
            }
            proc_result[0] = min_age_employee;

            // ищем максимального сотрудника для текущей профессии (под индексом i)
            unsigned short max_age = INIT_MAX_AGE;
            employee max_age_employee;
            for (size_t j = 0; j < empl_list->used; j++) {
                if (strcmp(empl_list->array[j].position, target_position) == 0
                    && empl_list->array[j].age > max_age) {
                    max_age_employee = empl_list->array[j];
                    max_age = max_age_employee.age;
                }
            }
            proc_result[1] = max_age_employee;

            write(pipe_arr[i][1], proc_result, sizeof(employee) * 2);

            exit(0);
        }
    }

    // вычитываем результаты из pipe'ов
        for (int i = 0; i < processes_number; i++) {
        employee proc_result[2];
        read(pipe_arr[i][0], proc_result, sizeof(employee) * 2);
        insert_array(result, proc_result[0]);
        insert_array(result, proc_result[1]);
    }

    // ждём завершения процессов
    pid_list_iter = 0;
    do {
        pid_t waited_pid = waitpid(pid_list[pid_list_iter], &status, WNOHANG);
        if (waited_pid < 0) {
            // TODO return with panic
        }

        if (waited_pid) {
            pid_list_iter++;
        }
    } while (pid_list_iter != processes_number);

    free(pid_list);

    sort_by_second_name(result);
    free_array(&positions);
}