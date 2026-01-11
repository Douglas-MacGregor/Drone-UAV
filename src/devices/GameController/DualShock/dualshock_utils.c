#include "dualshock_utils.h"
#include <stdio.h>

int get_dualshock_report(FILE *report, DualShockReport *report_buffer)
{
    // Read the DualShock report from the file
    size_t read_bytes = fread(report_buffer, 1, sizeof(DualShockReport), report);
    if (read_bytes != sizeof(DualShockReport))
    {
        fprintf(stderr, "Error reading DualShock report: expected %zu bytes, got %zu bytes\n", sizeof(DualShockReport), read_bytes);
        return -1;
    }
    return 0;
}
