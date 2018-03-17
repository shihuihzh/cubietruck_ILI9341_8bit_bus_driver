#include <zconf.h>
#include "common.h"

// delay
void delay(int d)
{
    usleep(d * 1000);
}

int digitalWrite(unsigned int pin, unsigned int val)
{
    return sunxi_gpio_output(pin, val);
}

int digitalWriteBank(unsigned int val)
{
    return sunxi_gpio_output_bank(DATA, 0x0 | val);
}