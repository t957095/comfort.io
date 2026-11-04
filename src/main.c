#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pigpio.h>

#define LED_PIN 17
#define LOG_FILE "data/temperature_log.csv"
#define STATUS_FILE "status.txt"

static int read_number(const char *text, double *number)
{
    char *end;
    errno = 0;
    double value = strtod(text, &end);

    if (end == text || errno == ERANGE || !isfinite(value)) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        end++;
    }
    if (*end != '\0') {
        return 0;
    }

    *number = value;
    return 1;
}

static int save_status(const char *status)
{
    FILE *file = fopen(STATUS_FILE, "w");
    if (file == NULL) {
        return 0;
    }

    int written = fprintf(file, "%s\n", status);
    int closed = fclose(file);
    return written >= 0 && closed == 0;
}

int main(int argc, char *argv[])
{
    double low = 68.0;
    double high = 76.0;
    if (argc != 1 &&
        (argc != 3 || !read_number(argv[1], &low) ||
         !read_number(argv[2], &high) || low > high)) {
        fprintf(stderr, "Usage: %s [minimum_F maximum_F]\n", argv[0]);
        return 1;
    }

    if (gpioInitialise() < 0) {
        fputs("Could not start pigpio. Run this on the Pi with sudo.\n", stderr);
        return 1;
    }
    if (gpioSetMode(LED_PIN, PI_OUTPUT) < 0 || gpioWrite(LED_PIN, 0) < 0) {
        fputs("Could not set up the LED on GPIO 17.\n", stderr);
        gpioTerminate();
        return 1;
    }

    FILE *log = fopen(LOG_FILE, "a");
    if (log == NULL) {
        perror(LOG_FILE);
        gpioTerminate();
        return 1;
    }
    long log_size = fseek(log, 0, SEEK_END) == 0 ? ftell(log) : -1;
    if (log_size < 0 ||
        (log_size == 0 && fputs("temperature_f,status\n", log) == EOF)) {
        fputs("Could not prepare the temperature log.\n", stderr);
        fclose(log);
        gpioTerminate();
        return 1;
    }

    char line[128];
    long double total = 0.0L;
    unsigned long count = 0;
    int result = 0;

    puts("Classroom Comfort Monitor");
    printf("Normal range: %g to %g F\n", low, high);
    puts("Enter q to quit.");

    for (;;) {
        printf("Temperature (F): ");
        fflush(stdout);
        if (fgets(line, sizeof line, stdin) == NULL) {
            if (ferror(stdin)) {
                perror("Reading temperature");
                result = 1;
            }
            break;
        }
        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Finish reading the long line before the next prompt. */
            }
            puts("Enter a shorter number.");
            continue;
        }
        size_t length = strlen(line);
        while (length > 0 && isspace((unsigned char)line[length - 1])) {
            line[--length] = '\0';
        }
        if (strcmp(line, "q") == 0 || strcmp(line, "Q") == 0) {
            break;
        }

        double temperature;
        if (!read_number(line, &temperature)) {
            puts("Enter a number like 72 or 72.5.");
            continue;
        }

        int warning = temperature < low || temperature > high;
        const char *status = warning ? "WARNING" : "NORMAL";
        if (gpioWrite(LED_PIN, warning) < 0) {
            fputs("Could not update the LED.\n", stderr);
            result = 1;
            break;
        }
        if (fprintf(log, "%.17g,%s\n", temperature, status) < 0 ||
            fflush(log) == EOF) {
            fputs("Could not save the temperature reading.\n", stderr);
            result = 1;
            break;
        }
        if (!save_status(status)) {
            fputs("Reading was logged, but status.txt could not be updated.\n", stderr);
            result = 1;
            break;
        }

        total += temperature;
        count++;
        double average = (double)(total / count);
        printf("%s | LED %s | Session average: %.2f F\n",
               status, warning ? "ON" : "OFF", average);
    }

    if (fclose(log) == EOF) {
        fputs("Could not close the temperature log.\n", stderr);
        result = 1;
    }
    if (gpioWrite(LED_PIN, 0) < 0) {
        fputs("Could not turn off the LED.\n", stderr);
        result = 1;
    }
    gpioTerminate();
    return result;
}
