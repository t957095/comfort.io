# Design

The program is in `src/main.c`. It reads a line from the terminal, checks that it is a number, and compares it to the low and high limits. The same comparison decides what to print, what goes in the files, and whether the LED is on.

The average uses a running total and a count. Both start over each time the program runs. The CSV keeps the older readings and their original status. A new range applies to new readings.

The LED uses BCM GPIO 17 on the Pi 4B. The program starts with the LED off, updates it after each valid reading, and turns it off when you type `q`. The status file keeps the last saved reading's status. The starter value is NORMAL, and the CSV starts with just its header.
