#include <stdio.h>

#include <pigpio.h>

#define LED_PIN 17

int main(void)
{
    if (gpioInitialise() < 0) {
        fputs("Could not initialize pigpio.\n", stderr);
        return 1;
    }

    if (gpioSetMode(LED_PIN, PI_OUTPUT) < 0 || gpioWrite(LED_PIN, 1) < 0) {
        fputs("Could not turn on the LED on GPIO 17.\n", stderr);
        gpioTerminate();
        return 1;
    }

    puts("LED is on. Press Enter to turn it off.");
    getchar();

    if (gpioWrite(LED_PIN, 0) < 0) {
        fputs("Could not turn off the LED on GPIO 17.\n", stderr);
        gpioTerminate();
        return 1;
    }

    puts("LED is off. Press Enter to exit.");
    getchar();

    gpioTerminate();
    return 0;
}
