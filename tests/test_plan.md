# Test plan

Build and run the monitor on the Pi using the README. Start with the default limits. Enter the first five readings in order and watch the LED.

| Input | Expected status | LED | Session average |
| --- | --- | --- | --- |
| 72 | NORMAL | Off | 72.00 F |
| 79 | WARNING | On | 75.50 F |
| 65 | WARNING | On | 72.00 F |
| 68 | NORMAL | Off | 71.00 F |
| 76 | NORMAL | Off | 72.00 F |
| `abc` or `72abc` | Ask for a number again | Off | Still 72.00 F |
| 76.5 | WARNING | On | 72.75 F |
| `q` | Exit | Off | |

Run `tail -n 6 data/temperature_log.csv` and check for the six numeric readings with their statuses. Run `cat status.txt` and check that it says WARNING.

Start a new run with `sudo ./comfort 70 74`. Check that 68 gives WARNING, then 74 gives NORMAL and the average is 71.00 F. Quit and check that `status.txt` now says NORMAL and the CSV kept both runs.

Try `sudo ./comfort 76 68`. It should print usage and exit without recording anything.
