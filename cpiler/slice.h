#pragma once

#include <stddef.h>

typedef struct {
    const void * pointer;
    size_t length;
} slice_t;

// slice to a single object/data thing (what do you call in in C?)
#define SLICE_TO(value) (slice_t){.pointer=&value, .length=sizeof(typeof(value))}

// slice to an array of thingies. array = pointer to array start. count = number of items in array
#define SLICE_ARRAY(array, count) (slice_t){.pointer=(array), .length=(count)*(sizeof(typeof(*(array))))}