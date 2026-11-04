# comfort.io

Classroom temperature monitor for CSE 1106. The LED turns on outside the normal range of 68–76°F, inclusive.

## Wire the LED

Shut down and unplug the Pi 4B before wiring. Put the LED legs in separate breadboard rows.

```text
Physical pin 11 (GPIO 17) -> 1 kΩ resistor -> LED long leg
Physical pin 9 (GND)      <-                 LED short leg
```

## Run on the Pi

In Terminal, clone the repo with `git clone https://github.com/t957095/comfort.io.git` or update your copy with `git pull --ff-only` from the repo folder. With GCC and [pigpio](https://github.com/joan2937/pigpio) installed, run these from that folder.

```sh
gcc -std=c11 -Wall -Wextra -o comfort src/main.c -lpigpio -lrt -pthread && sudo ./comfort
```

Enter one temperature at a time. Try `72`, `79`, and `65`. The LED should go off, on, and on. Type `q` to quit. To set a different range, run `sudo ./comfort 70 74`.

The average is for the current run. Readings go in `data/temperature_log.csv` and the latest status goes in `status.txt`. See the [test plan](tests/test_plan.md).

## Part B LED test

```sh
gcc -Wall -Wextra -o led led.c -lpigpio -lrt -pthread && sudo ./led
```

The LED starts on. Press Enter to turn it off and Enter again to exit.
