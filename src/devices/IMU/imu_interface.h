#include <stdint.h>

typedef struct
{
    int (*init)(void *self);
    int (*close)(void *self);
    // add more function pointers as needed
} IMUInterface;