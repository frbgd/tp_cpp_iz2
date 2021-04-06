#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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
    int pid_arr[processes_number];
    // количество профессий в каждом процессе
    size_t proc_positions_size = positions.used / processes_number;
    // создаем shared memory, в который процессы запишут мин и макс. возрастных сотрудников для каждой профессии
    int shmid = shmget(IPC_PRIVATE, sizeof(employee) * positions.used * 2, 0644|IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "Failed to create shared memory\n");
        return -1;
    }

    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        int pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Failed to fork child\n");
            return -1;
        } else if (pid > 0) {
            // заполняем массив pid'ов, чтобы потом ждать их завершения
            pid_arr[proc_idx] = pid;
        } else if (pid == 0) {
            // профессии, среди которых ищет текущий процесс
            employee_array proc_positions;
            if (init_array(&proc_positions, proc_positions_size) == -1) {
                exit(-1);
            }
            // рассчитываем количество профессий, обрабатываемых текущим процессом
            position_info info = {
                    proc_positions_size,
                    proc_idx,
                    processes_number,
                    positions.used
            };
            size_t proc_position_len = calculate_proc_positions_len(info);
            // получаем список профессий, обрабатываемых текущим процессом
            if (slice_array(&positions,
                            &proc_positions,
                            proc_positions_size * proc_idx,
                            proc_position_len) == -1 ) {
                return -1;
            }
            // массив найденных сотрудников
            employee_array proc_result;
            if (init_array(&proc_result, proc_positions.used * 2) == -1) {
                exit(-1);
            }
            // итерируемся по профессиям
            for (
                    size_t proc_positions_idx = 0;
                    proc_positions_idx < proc_positions.used;
                    proc_positions_idx++
                ) {
                // ищем самого молодого сотрудника для текущей профессии
                employee *min_age_employee = malloc(sizeof(employee));
                if (find_min_age_employee(empl_list,
                                          proc_positions.array[proc_positions_idx].position, min_age_employee) != 0) {
                    exit(-1);
                }
                if (insert_array(&proc_result, *min_age_employee) == -1) {
                    exit(-1);
                }
                free(min_age_employee);

                // ищем самого взрослого сотрудника для текущей профессии
                employee *max_age_employee = malloc(sizeof(employee));
                if(find_max_age_employee(empl_list,
                                         proc_positions.array[proc_positions_idx].position, max_age_employee) != 0) {
                    exit(-1);
                }
                if (insert_array(&proc_result, *max_age_employee) == -1) {
                    exit(-1);
                }
                free(max_age_employee);
            }

            // записываем полученных сотрудников в shared memory
            employee *shm_empl = (employee*)shmat(shmid, NULL, 0);
            if (shm_empl == (void *) -1) {
                fprintf(stderr, "Failed to attach shared memory\n");
                exit(-1);
            }
            // позиция начала списка сотрудников текущего процесса в shared memory
            size_t shared_memory_start_idx = proc_positions_size * proc_idx * 2;
            for (size_t i = 0; i < proc_result.used; i++) {
                shm_empl[shared_memory_start_idx + i] = proc_result.array[i];
            }
            if (shmdt(shm_empl) == -1) {
                fprintf(stderr, "Failed to detach shared memory\n");
                return -1;
            }

            if (free_array(&proc_result) == -1) {
                exit(-1);
            }
            if (free_array(&proc_positions) == -1) {
                exit(-1);
            }
            exit(0);
        }
    }

    // дожидаемся завершения всех процессов
    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        int proc_status;
        waitpid(pid_arr[proc_idx], &proc_status, 0);
        if (proc_status != 0) {
            fprintf(stderr, "Error in child %d\n", pid_arr[proc_idx]);
            return -1;
        }
    }
    // вычитываем данные из shared memory
    employee *shm_empl = (employee*)shmat(shmid, NULL, 0);
    if (shm_empl == (void *) -1) {
        fprintf(stderr, "Failed to attach shared memory\n");
        return -1;
    }
    for (size_t i = 0; i < positions.used * 2; i++) {
        insert_array(result, shm_empl[i]);
    }
    if (shmdt(shm_empl) == -1) {
        fprintf(stderr, "Failed to detach shared memory\n");
        return -1;
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "Failed to destroy shared memory\n");
        return -1;
    }

    if (sort_by_second_name(result) == -1) {
        return -1;
    }
    if (free_array(&positions) == -1) {
        return -1;
    }

    return 0;
}
