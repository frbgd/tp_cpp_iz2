#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include "../include/employee.h"

int main() {
    char *file_name = "test_data10k.bin";
    int employees_number = read_employees_number_from_file(file_name);
    if (employees_number < 0) {
        return 1;
    }
    employee_array employee_list;
    init_array(&employee_list, employees_number);
    if (read_employees_from_file(file_name, employees_number, &employee_list) != 0) {
        return 1;
    }

    employee_array search_result;
    init_array(&search_result, ARRAY_INIT_SIZE);

    // from dynamic library
    void *ext_library;
    int (*searchfunc)(employee_array*, employee_array*);
    ext_library = dlopen("libEmployeeDynamic.so", RTLD_NOW);
    if (!ext_library)
    {
        fprintf(stderr, "Cannot load dynamic library: %s\n", dlerror ());
        return 1;
    }
    searchfunc = dlsym(ext_library, "search");
    clock_t begin = clock();
    int result = (*searchfunc)(&employee_list, &search_result);
    clock_t end = clock();
    if (result != 0) {
        dlclose(ext_library);
        return 1;
    }
    dlclose(ext_library);
    double time_spent = (double)(end - begin);
    printf("dynamic lib - %f\n", time_spent);

    free_array(&search_result);
    init_array(&search_result, ARRAY_INIT_SIZE);

    // from static library
    begin = clock();
    result = search(&employee_list, &search_result);
    end = clock();
    if (result != 0)
        return 1;
    time_spent = (double)(end - begin);
    printf("static lib - %f\n", time_spent);

    free_array(&search_result);
    free_array(&employee_list);

    return 0;
}