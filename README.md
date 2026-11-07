# Embedded Piscine

Exercises completed as part of the embedded piscine organized by the **42 Chips** association.

This is low-level programming on an **AVR ATmega328P** microcontroller (16 MHz), in C without the Arduino library: everything is done by directly manipulating the registers.

## Modules

| Module | Topic | Concepts covered |
|--------|-------|------------------|
| `module00` | GPIO | Turning on an LED, reading a button (`DDRx`, `PORTx`, `PINx`) |
| `module01` | Timers | Timer1, hardware PWM, duty cycle control |
| `module02` | UART | Serial communication, interrupts (RX/TX, timer) |
| `module03` | RGB LED | PWM on Timer0 / Timer2, colors sent over UART |
| `module04` | Interrupts | External interrupts (`INT0`) and timer interrupts |
| `module05` | ADC | Analog reading (potentiometer, LDR, NTC, internal temperature sensor) |
| `module06` | I2C | TWI bus, reading the AHT20 temperature/humidity sensor |

Each module contains exercises `ex00`, `ex01`, … each with its own `main.c` and `Makefile`.

## Usage

In an exercise folder:

```sh
make        # build and flash the board
make hex    # build only (.elf, .hex, .bin)
make flash  # flash the board
make clean  # remove generated files
```

The serial port and programmer can be changed at the top of the `Makefile` (`PORT`, `PROGRAMMER`).

To view the UART output (modules 02, 03, 05, 06):

```sh
screen /dev/ttyUSB0 115200
```

## Resources

- [ATmega328P datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf)
- [AHT20 datasheet](https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf)
