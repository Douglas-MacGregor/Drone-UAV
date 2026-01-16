
typedef struct
{
    int (*init_gpio)();
    int (*close_gpio)();
    int (*set_pin_mode)(int pin, int mode);
    int (*write_pin)(int pin, int value);
    int (*read_pin)(int pin);
} HAL_GPIO;