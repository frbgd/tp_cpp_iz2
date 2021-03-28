#ifndef TP_CPP_IZ2_ARRAY_H
#define TP_CPP_IZ2_ARRAY_H

#include "employee.h"

typedef struct {
    employee *array;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, employee element);
void freeArray(Array *a);

#endif //TP_CPP_IZ2_ARRAY_H
