#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include "../include/employee.h"

int main() {
    char *file_name = "test_data.bin";
    int employees_number = read_employees_number_from_file(file_name);
    if (employees_number < 0) {
        return 1;
    }
    employee_array employee_list;
    init_array(&employee_list, employees_number);
    if (read_employees_from_file(file_name, employees_number, &employee_list) != 0) {
        return 1;
    }

    struct employee_array static_search_result;
    init_array(&static_search_result, ARRAY_INIT_SIZE);
    struct employee_array dynamic_search_result;
    init_array(&dynamic_search_result, ARRAY_INIT_SIZE);

    // from dynamic library
    void *ext_library;
    int (*searchfunc)(employee_array *empl_list, employee_array *result);

    ext_library = dlopen("libEmployeeDynamic.so", RTLD_NOW);
    if (!ext_library)
    {
        fprintf(stderr, "Cannot load dynamic library: %s\n", dlerror ());
        return 1;
    }
    clock_t begin = clock();
    searchfunc = dlsym(ext_library, "search");
    if ((*searchfunc)(&employee_list, &dynamic_search_result) != 0) {
        dlclose(ext_library);
        return 1;
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("dynamic library functime - %f\n", time_spent);
    dlclose(ext_library);

    // from static library
    begin = clock();
    if (search(&employee_list, &static_search_result) != 0)
        return 1;
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("static library functime - %f", time_spent);

//    for(size_t i = 0; i < static_search_result.used; i++){
//        printf("%s %d %d %d %s %d %s\n",
//               static_search_result.array[i].position, static_search_result.array[i].age,
//               static_search_result.array[i].work_experience, static_search_result.array[i].salary,
//               static_search_result.array[i].second_name, static_search_result.array[i].is_male,
//               static_search_result.array[i].first_name);
//    }
//    printf("\n\n\n\n");
//    for(size_t i = 0; i < dynamic_search_result.used; i++){
//        printf("%s %d %d %d %s %d %s\n",
//               dynamic_search_result.array[i].position, dynamic_search_result.array[i].age,
//               dynamic_search_result.array[i].work_experience, dynamic_search_result.array[i].salary,
//               dynamic_search_result.array[i].second_name, dynamic_search_result.array[i].is_male,
//               dynamic_search_result.array[i].first_name);
//    }

    free_array(&dynamic_search_result);
    free_array(&static_search_result);
    free_array(&employee_list);
    return 0;
}