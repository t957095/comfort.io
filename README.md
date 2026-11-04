# comfort.io

Option 1 for the CSE 1106 final project. Type in classroom temperatures in Fahrenheit. The program shows NORMAL from 68 to 76 F, including 68 and 76. Anything outside that range shows WARNING and turns on the LED.

## Wire the LED

Shut down the Pi 4B and unplug it before connecting the wires. Put the LED legs in separate breadboard rows.

```text
Physical pin 11 (GPIO 17) -> 1 kΩ resistor -> LED long leg
Physical pin 9 (GND)      <-                 LED short leg
```

## Build and run on the Pi

Power the Pi back on and open Terminal. If you already cloned the repo into your home folder, update it.

```sh
cd ~/comfort.io
git pull --ff-only
```

For a first clone, run `cd ~`, then `git clone https://github.com/t957095/comfort.io.git` and `cd comfort.io`.

Run the following from the repo folder. You need GCC and the [pigpio C library](https://github.com/joan2937/pigpio).

```sh
gcc -std=c11 -Wall -Wextra -o comfort src/main.c -lpigpio -lrt -pthread
sudo ./comfort
```

Try `72`, `79`, then `65`, pressing Enter after each one. The LED should be off, on, and on. The average after those three readings is 72.00 F. Type `q` and press Enter to quit and turn off the LED.

To run with a normal range of 70 through 74 F, run `sudo ./comfort 70 74`. Both endpoints count as normal. With no numbers after `./comfort`, it uses 68 and 76 again.

## Files

- `src/main.c` is the monitor program.
- `data/temperature_log.csv` adds one row per reading and keeps rows between runs. The average starts fresh each run.
- `status.txt` holds the most recently saved status. Its starter value is NORMAL.
- `docs/` has the requirements, design, and change log. The [test plan](tests/test_plan.md) has the demo inputs and expected results.

After a run, use `tail data/temperature_log.csv` and `cat status.txt` to see the saved results. These files change locally as you enter readings.

## Part B LED test

`led.c` is the separate wiring test from the lab. To run it from the repo folder:

```sh
gcc -Wall -Wextra -o led led.c -lpigpio -lrt -pthread
sudo ./led
```

The LED starts on. Press Enter to turn it off and Enter again to exit.
