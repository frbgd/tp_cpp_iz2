#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/employee.h"

size_t calculate_proc_positions_len(size_t proc_positions_size, size_t proc_idx, size_t processes_number, size_t positions_number) {
    size_t start_idx = proc_positions_size * proc_idx;
    size_t proc_result_len;

    // в последнем процессе обрабатываются оставшиеся профессии
    if (proc_idx == processes_number - 1) {
        size_t stop_idx = positions_number;
        proc_result_len = (stop_idx - start_idx);
    } else {
        proc_result_len = proc_positions_size;
    }

    return proc_result_len;
}

int search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    init_array(&positions, ARRAY_INIT_SIZE);
    for (size_t i = 0; i < empl_list->used; i++) {
        if (match_position(&positions, empl_list->array[i].position) == 0) {
            insert_array(&positions, empl_list->array[i]);
        }
    }

    // параллелим алгоритм по количеству процессов в системе, каждый процесс будет искать самого взрослого и молодого
    // сотрудника в определенных профессиях
    int processes_number = get_nprocs();
    // количество профессий в каждом процессе
    size_t proc_positions_size = positions.used / processes_number;
    // готовим массив pipe'ов
    int pipe_arr[processes_number][2];

    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        if (pipe(pipe_arr[proc_idx]) == -1){
            fprintf(stderr, "Failed to create pipe\n");
            return -1;
        }

        int pid = fork();
        if (pid < 0 ) {
            fprintf(stderr, "Failed to fork child\n");
            return -1;
        } else if (pid > 0) {
            close(pipe_arr[proc_idx][1]);
        } else if (pid == 0) {
            close(pipe_arr[proc_idx][0]);

            // профессии текущего процесса
            employee_array proc_positions;
            init_array(&proc_positions, proc_positions_size);
            slice_array(&positions, &proc_positions, proc_positions_size * proc_idx,
                        calculate_proc_positions_len(proc_positions_size, proc_idx, processes_number, positions.used));
            // итоговый массив сотрудников процесса
            employee_array proc_result;
            init_array(&proc_result, proc_positions.used * 2);

            for (size_t proc_positions_idx = 0; proc_positions_idx < proc_positions.used; proc_positions_idx++) {
                // ищем самого молодого сотрудника для текущей профессии
                employee min_age_employee = find_min_age_employee(empl_list, proc_positions.array[proc_positions_idx].position);
                insert_array(&proc_result, min_age_employee);

                // ищем самого взрослого сотрудника для текущей профессии
                employee max_age_employee = find_max_age_employee(empl_list, proc_positions.array[proc_positions_idx].position);
                insert_array(&proc_result, max_age_employee);
            }

            write(pipe_arr[proc_idx][1], (employee *)proc_result.array, sizeof(employee) * proc_result.used);

            free_array(&proc_result);
            free_array(&proc_positions);
            exit(0);
        }
    }

    // вычитываем результаты из pipe'ов
    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        size_t proc_result_len = calculate_proc_positions_len(proc_positions_size, proc_idx, processes_number, positions.used) * 2;
        employee proc_result[proc_result_len];
        read(pipe_arr[proc_idx][0], &proc_result, sizeof(employee) * proc_result_len);
        for (size_t i = 0; i < proc_result_len; i++) {
            insert_array(result, proc_result[i]);
        }
    }

    sort_by_second_name(result);
    free_array(&positions);

    return 0;
}