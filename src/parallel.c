#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/employee.h"

typedef struct {
    size_t proc_positions_size;
    size_t proc_idx;
    size_t processes_number;
    size_t positions_number;
} position_info;

size_t calculate_proc_positions_len(position_info info) {
    size_t start_idx = info.proc_positions_size * info.proc_idx;
    size_t proc_result_len;

    // в последнем процессе обрабатываются оставшиеся профессии
    if (info.proc_idx == info.processes_number - 1) {
        size_t stop_idx = info.positions_number;
        proc_result_len = (stop_idx - start_idx);
    } else {
        proc_result_len = info.proc_positions_size;
    }

    return proc_result_len;
}

int search(employee_array *empl_list, employee_array *result) {
    // собираем список профессий
    employee_array positions;
    if (init_array(&positions, ARRAY_INIT_SIZE) == -1) {
        return -1;
    }
    for (size_t i = 0; i < empl_list->used; i++) {
        int match = match_position(&positions, empl_list->array[i].position);
        if (match == 0) {
            if (insert_array(&positions, empl_list->array[i]) == -1) {
                return -1;
            }
        } else if (match == -1) {
            return -1;
        }
    }

    // параллелим алгоритм по количеству процессов в системе,
    // каждый процесс будет искать самого взрослого и молодого
    // сотрудника в определенных профессиях
    int processes_number = get_nprocs();
    // количество профессий в каждом процессе
    size_t proc_positions_size = positions.used / processes_number;
    // готовим массив pipe'ов
    int pipe_arr[processes_number][2];

    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        if (pipe(pipe_arr[proc_idx]) == -1) {
            fprintf(stderr, "Failed to create pipe\n");
            return -1;
        }

        int pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Failed to fork child\n");
            return -1;
        } else if (pid > 0) {
            close(pipe_arr[proc_idx][1]);
        } else if (pid == 0) {
            close(pipe_arr[proc_idx][0]);

            // профессии, среди которых ищет текущий процесс
            employee_array proc_positions;
            if (init_array(&proc_positions, proc_positions_size) == -1) {
                exit(-1);
            }
            position_info info = {
                    proc_positions_size,
                    proc_idx,
                    processes_number,
                    positions.used
            };
            if (slice_array(&positions,
                            &proc_positions,
                            proc_positions_size * proc_idx,
                            calculate_proc_positions_len(info)) == -1 ) {
                return -1;
            }
            // массив найденных сотрудников
            employee_array proc_result;
            if (init_array(&proc_result, proc_positions.used * 2) == -1) {
                exit(-1);
            }

            for (
                    size_t proc_positions_idx = 0;
                    proc_positions_idx < proc_positions.used;
                    proc_positions_idx++
                ) {
                // ищем самого молодого сотрудника для текущей профессии
                employee min_age_employee =
                        find_min_age_employee(empl_list, proc_positions.array[proc_positions_idx].position);
                if (insert_array(&proc_result, min_age_employee) == -1) {
                    exit(-1);
                }

                // ищем самого взрослого сотрудника для текущей профессии
                employee max_age_employee =
                        find_max_age_employee(empl_list, proc_positions.array[proc_positions_idx].position);
                if (insert_array(&proc_result, max_age_employee) == -1) {
                    exit(-1);
                }
            }

            write(pipe_arr[proc_idx][1], (employee *)proc_result.array, sizeof(employee) * proc_result.used);

            if (free_array(&proc_result) == -1) {
                exit(-1);
            }
            if (free_array(&proc_positions) == -1) {
                exit(-1);
            }
            exit(0);
        }
    }

    // вычитываем результаты из pipe'ов
    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        // каждый процесс отдает число сотрудников,
        // равное удвоенному число обрабатываемых процессом должностей

        position_info info = {
                proc_positions_size,
                proc_idx,
                processes_number,
                positions.used
        };
        size_t proc_result_len = calculate_proc_positions_len(info) * 2;
        employee proc_result[proc_result_len];
        read(pipe_arr[proc_idx][0], &proc_result, sizeof(employee) * proc_result_len);
        for (size_t i = 0; i < proc_result_len; i++) {
            if (insert_array(result, proc_result[i]) == -1) {
                return -1;
            }
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
