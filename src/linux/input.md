# Linux input

Some notes on using `/dev/input/*` device driver files.

## mouseX / mice

These device files are created by the [mousedev] driver.

- `/dev/input/mouseX` represents the input stream for a *SINGLE* mouse device.
- `/dev/input/mice` represents the merged input stream for *ALL* mouse devices.

The data stream consists of `3 bytes` per `event`. An event is encoded as `(BTN, X, Y)`.
- `BTN` button pressed
- `X` movement in x-direction `-1 -> left` and `1 -> right`
- `Y` movement in y-direction `-1 -> down` and `1 -> up`

The raw data stream can be inspected as follows.
```bash
sudo cat /dev/input/mice | od -tx1 -w3 -v
```

## eventX

These device files are created by the [evdev] driver.

- `/dev/input/eventX` represents the generic input event interface a *SINGLE* input deivece.

Input events are encoded as given by the `input_event` struct below. Reading
from the `eventX` device file will always yield whole number of input events.
```c
struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};
```

On most 64bit machines the raw data stream can be inspected as follows.
```bash
sudo cat /dev/input/event4 | od -tx1 -w24 -v
```

## Identifyin device files.

To find out which device file is assigned to which input device the following
file `/proc/bus/input/devices` in the proc filesystem can be consulted.

This yields entries as follows and shows which `Handlers` are assigned to which
`Name`.
```
I: Bus=0018 Vendor=04f3 Product=0033 Version=0000
N: Name="Elan Touchpad"
...
H: Handlers=event15 mouse0
...
```

## Example: Toying with `/dev/input/eventX`

Once compiled, the example should be run as `sudo ./event /dev/input/eventX`.
```c
{{#include src/event.c:7: }}
```

[linux-input]: https://www.kernel.org/doc/Documentation/input/input.txt
[mousedev]: TODO /home/johannst/dev/linux/drivers/input/mousedev.c
[evdev]: TODO /home/johannst/dev/linux/drivers/input/evdev.c
