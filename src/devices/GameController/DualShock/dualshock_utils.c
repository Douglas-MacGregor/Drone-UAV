#include "dualshock_utils.h"
#include <stdio.h>
#include <unistd.h>

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

int print_dualshock_report(const DualShockReport *report_buffer)
{
    printf("DualShock Report ID: %u\n", report_buffer->report_id);
    printf("Left Stick: X=%u, Y=%u\n", report_buffer->left_stick_x, report_buffer->left_stick_y);
    printf("Right Stick: X=%u, Y=%u\n", report_buffer->right_stick_x, report_buffer->right_stick_y);
    printf("Buttons (Shapes): 0x%02X\n", report_buffer->buttons_shapes);
    printf("Buttons (Misc): 0x%02X\n", report_buffer->buttons_misc);
    printf("Counter: %u\n", report_buffer->counter);
    printf("Left Trigger: %u\n", report_buffer->left_trigger);
    printf("Right Trigger: %u\n", report_buffer->right_trigger);
    return 0;
}

int process_dualshock_report(FILE *report, int dead_zone)
{
    DualShockReport report_buffer;
    if (get_dualshock_report(report, &report_buffer) != 0)
    {
        return -1;
    }
    print_dualshock_report(&report_buffer);
    return 0;
}

int main()
{
    FILE *report_file = fopen("/dev/hidraw0", "rb");
    if (!report_file)
    {
        fprintf(stderr, "Failed to open report file\n");
        return -1;
    }
    for (int i = 0; i < 10; i++)
    {                       // Read 10 reports for demonstration
        int dead_zone = 10; // Example dead zone value
        if (process_dualshock_report(report_file, dead_zone) != 0)
        {
            fclose(report_file);
            return -1;
        }
    }
    usleep(10000000); // Sleep for 100ms before next read

    fclose(report_file);
    return 0;
}