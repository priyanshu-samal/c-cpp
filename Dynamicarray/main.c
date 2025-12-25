#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


typedef struct {
    int    *data;
    size_t size;
    size_t capacity;
} DynArray;


void da_init(DynArray *da) {
    da->data = NULL;
    da->size = 0;
    da->capacity = 0;
}


int da_reserve(DynArray *da, size_t new_capacity) {
    if (new_capacity <= da->capacity)
        return 1;

    int *new_data = (int *)realloc(da->data, new_capacity * sizeof(int));
    if (!new_data)
        return 0; 

    da->data = new_data;
    da->capacity = new_capacity;
    return 1;
}


int da_push(DynArray *da, int value) {
    if (da->size == da->capacity) {
        size_t new_capacity = (da->capacity == 0) ? 4 : da->capacity * 2;
        if (!da_reserve(da, new_capacity))
            return 0;
    }

    da->data[da->size++] = value;
    return 1;
}


int da_pop(DynArray *da, int *out) {
    if (da->size == 0)
        return 0;

    da->size--;
    if (out)
        *out = da->data[da->size];

    return 1;
}


int da_get(const DynArray *da, size_t index, int *out) {
    if (index >= da->size)
        return 0;

    *out = da->data[index];
    return 1;
}


int da_set(DynArray *da, size_t index, int value) {
    if (index >= da->size)
        return 0;

    da->data[index] = value;
    return 1;
}


int da_insert(DynArray *da, size_t index, int value) {
    if (index > da->size)
        return 0;

    if (da->size == da->capacity) {
        size_t new_capacity = (da->capacity == 0) ? 4 : da->capacity * 2;
        if (!da_reserve(da, new_capacity))
            return 0;
    }

    for (size_t i = da->size; i > index; i--) {
        da->data[i] = da->data[i - 1];
    }

    da->data[index] = value;
    da->size++;
    return 1;
}


int da_remove(DynArray *da, size_t index) {
    if (index >= da->size)
        return 0;

    for (size_t i = index; i + 1 < da->size; i++) {
        da->data[i] = da->data[i + 1];
    }

    da->size--;
    return 1;
}


void da_free(DynArray *da) {
    free(da->data);
    da->data = NULL;
    da->size = 0;
    da->capacity = 0;
}



int main(void) {
    DynArray arr;
    da_init(&arr);

    
    for (int i = 0; i < 10; i++) {
        if (!da_push(&arr, i * 10)) {
            printf("Push failed\n");
            da_free(&arr);
            return 1;
        }
    }

    
    da_insert(&arr, 5, 999);

    
    da_remove(&arr, 2);

    
    printf("Array contents:\n");
    for (size_t i = 0; i < arr.size; i++) {
        printf("[%zu] = %d\n", i, arr.data[i]);
    }

    printf("Size = %zu, Capacity = %zu\n", arr.size, arr.capacity);

    
    int popped;
    if (da_pop(&arr, &popped)) {
        printf("Popped: %d\n", popped);
    }

    da_free(&arr);
    return 0;
}
