# MiniSumo 2022 Board (`sumo_6`)

> [!WARNING]
> This is not an official Zephyr board module, nor is it managed by PBS. Use at your own risk.

## Overview

The `sumo_6` board configuration provides Zephyr RTOS support for the custom ESP32-S2 based board designed for the [PBS MiniSumo 2022 Robotics Project](https://roboty.pbs.edu.pl/pl/).

This out-of-tree board module defines the hardware abstraction layer, allowing developers to utilize standard Zephyr APIs to interact with the board's specific routing, including standard LEDs, user buttons, external sensor ports, WS2812 addressable LEDs, and the custom H-Bridge motor driver.

## Devicetree Aliases

For ease of use in Zephyr applications, the following standard aliases are provided out-of-the-box:

* `sw0`: Start Button
* `led0`: Standard LED 1
* `led1`: Standard LED 2
* `led-strip`: WS2812 Addressable LED String
* `motor1`: Left/Primary Motor
* `motor2`: Right/Secondary Motor
* *Standard buses*: `uart-0`, `i2c-0`, `i2c-1`, `watchdog0`

## Hardware Mappings & Peripherals

The board utilizes the ESP32-S2's dual GPIO ports. *Note: In Zephyr's ESP32 DTS, `gpio1` is offset by 32 (e.g., `gpio1 9` corresponds to physical GPIO 41).*

### User IO (LEDs & Buttons)

* **LED 1 (`led0`):** GPIO 41 (`gpio1 9`), Active High.
* **LED 2 (`led1`):** GPIO 42 (`gpio1 10`), Active High.
* **START Button (`sw0`):** GPIO 45 (`gpio1 13`), Active Low with internal Pull-Up resistor.

### WS2812 Addressable LED Strip

The WS2812 (NeoPixel) communication is driven by the ESP32-S2's SPI peripheral (`spim2`) to ensure strict timing.

* **Data Pin (SPI MOSI):** GPIO 21

### External Sensor Ports

The board breaks out four 5.5V-tolerant sensor ports (VCC, Signal, GND). To make reading these sensors trivial in Zephyr, they are mapped using the standard `gpio-keys` compatible. This allows you to easily attach interrupts or poll their states without writing custom GPIO setup code.

```devicetree
sensors {
    compatible = "gpio-keys";
    grd1: grd_1 { gpios = <&gpio0 2 GPIO_ACTIVE_HIGH>; label = "GRD1"; };
    grd2: grd_2 { gpios = <&gpio0 3 GPIO_ACTIVE_HIGH>; label = "GRD2"; };
    grd3: grd_3 { gpios = <&gpio0 4 GPIO_ACTIVE_HIGH>; label = "GRD3"; };
    grd4: grd_4 { gpios = <&gpio0 5 GPIO_ACTIVE_HIGH>; label = "GRD4"; };
};

```

### DC Motor Drivers

The board uses a custom driver binding (`sims,hbridge-motor`) to control two DC motors. Each motor utilizes two hardware PWM channels (via `ledc0`) and two standard GPIOs for direction/bridge control.

**Motor 1 (`motor1`):**

* `PWM l1`: LEDC Channel 0
* `PWM r1`: LEDC Channel 1
* `GPIO l2`: GPIO 34 (`gpio1 2`)
* `GPIO r2`: GPIO 36 (`gpio1 4`)

**Motor 2 (`motor2`):**

* `PWM l1`: LEDC Channel 2
* `PWM r1`: LEDC Channel 3
* `GPIO l2`: GPIO 38 (`gpio1 6`)
* `GPIO r2`: GPIO 40 (`gpio1 8`)

## Building and Flashing

To build an application targeting this specific hardware routing, simply pass the board name to West:

```bash
# Build your application
west build -b sumo_6 <path_to_your_app>

# Flash over USB
west flash

```

## Resources and Documentation

* **Board Schematics & Files:** [Politechnika Bydgoska - Płytka z 2022r](https://roboty.pbs.edu.pl/pl/pliki-do-pobrania/plytka-z-2022r)
* **Zephyr ESP32-S2 official board Documentation:** [ESP32-S2-Saola](https://docs.zephyrproject.org/latest/boards/espressif/esp32s2_saola/doc/index.html)
* **Espressif WROVER Datasheet:** [ESP32-S2-WROVER Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-s2-wrover_esp32-s2-wrover-i_datasheet_en.pdf)