# comfort.io

Smart classroom comfort monitor.

## Part B LED test

Open Terminal on the Raspberry Pi and clone the repository. You can clone it without signing in to GitHub.

```sh
cd ~
git clone https://github.com/t957095/comfort.io.git
cd comfort.io
```

If you already cloned it into your home folder, run `cd ~/comfort.io && git pull --ff-only` instead.

Power off the Pi before wiring the LED. Connect BCM GPIO 17 (physical pin 11) through a 1 kΩ resistor to the LED’s long leg. Connect the short leg to ground (physical pin 9).

Power the Pi back on, open Terminal, and run these commands. You’ll need GCC and the [pigpio C library](https://github.com/joan2937/pigpio). This lab uses pigpio, which requires a supported board such as a Raspberry Pi 4.

```sh
cd ~/comfort.io
gcc -Wall -Wextra -o led led.c -lpigpio -lrt -pthread
sudo ./led
```

The LED should turn on when the program starts. Press Enter once to turn it off, then press Enter again to exit.
