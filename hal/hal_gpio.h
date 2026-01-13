
typedef struct
{
    int (*init_gpio)(void *self);
    int (*close_gpio)(void *self);
    int (*set_pin_mode)(void *self, int pin, int mode);
    int (*write_pin)(void *self, int pin, int value);
    int (*read_pin)(void *self, int pin);
} HAL_GPIO;