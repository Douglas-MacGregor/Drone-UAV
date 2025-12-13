#include <stdint.h>

typedef struct
{
    int (*init)(void *self);
    int (*close)(void *self);
    int (*send)(void *self, const uint8_t *data, int len);
    int (*receive)(void *self, uint8_t *buffer, int max_len);
    int (*set_syncword)(void *self, uint8_t syncword);
    int (*set_frequency)(void *self, uint32_t freq);
    int (*set_power)(void *self, int level);
    int (*set_spreading_factor)(void *self, int sf);
    int (*sleep)(void *self);
    int (*standby)(void *self);
    int (*reset)(void *self);
} LoRaInterface;