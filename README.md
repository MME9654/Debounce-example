| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- |
# Debounce

This is a simple example that demonstrates the use of an interrupt service routine that triggers with each button press. It implements software switch debouncing to prevent "false" button presses resulting for mechanical contact bouncing from registering. By default, the button is expected to be attached between GPIO1 and ground; however the GPIO pin can be changed using menuconfig `idf.py menuconfig`.

## How to Use Example

Before project configuration and build, be sure to set the correct chip target using `idf.py set-target <chip_name>`.

### Hardware Required

* A development board with any Espressif SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* A USB cable for microcontroller power supply and programming
* A SPST-NO pushbutton
* Some jumper wires to connect pushbutton to GPIO

### Build and Flash

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://idf.espressif.com/) for full steps to configure and use ESP-IDF to build projects.


## Example Output

Run the example, you will see the following output log after the pushbutton has been pressed 6 times:

```
I (0) cpu_start: App cpu up.
I (292) main_task: Calling app_main()
I (292) debounce: Initialize button
I (302) gpio: GPIO[1]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:2 
I (4412) debounce: Button has been pressed 1 times
I (4712) debounce: Button has been pressed 2 times
I (4812) debounce: Button has been pressed 3 times
I (4912) debounce: Button has been pressed 4 times
I (6612) debounce: Button has been pressed 5 times
I (7012) debounce: Button has been pressed 6 times
```