# NORVI GSM-AE02-V-L – Hardware Test Example

## Overview

This example demonstrates how to run a full hardware test on the NORVI GSM-AE02-V-L controller.

The purpose of this example is to show how to:

- Start the Wi-Fi access point and print the device MAC address
- Initialize the OLED display and the ADS1115 analog-to-digital converter
- Test the LTE modem using AT commands
- Read the front-panel button
- Read the status of the 8 digital inputs
- Read the four 0–10 V analog inputs in volts
- Switch the 2 transistor outputs ON and OFF alternately
- Send test data over RS-485

This example can be used as a starting point for developers building applications with NORVI controllers.

## Product Used

Product: NORVI GSM-AE02-V-L (ESP32-WROOM-32, LTE, 8 digital inputs, 4 × 0–10 V analog inputs, 2 transistor outputs, RS-485, OLED display)
Platform: Arduino IDE / PlatformIO (ESP32)

https://norvi.io

## Purpose of This Example

The example demonstrates how to:

- Start a Wi-Fi access point named after the device MAC address
- Initialize the SSD1306 OLED display over I2C (address 0x3C, SDA = GPIO 16, SCL = GPIO 17)
- Initialize the ADS1115 ADC (I2C address 0x48) and read the 0–10 V inputs
- Communicate with the LTE modem (EC25 / SIM7500) over UART (RXD = GPIO 33, TXD = GPIO 32, RESET = GPIO 21)
- Read the front-panel button analog value on GPIO 36
- Read the 8 digital inputs (GPIO HIGH = input OFF, GPIO LOW = input ON)
- Drive the transistor outputs (OUTPUT 1 = GPIO 12, OUTPUT 2 = GPIO 2)
- Transmit text over RS-485 (RX = GPIO 25, TX = GPIO 26, flow control = GPIO 22)

Naming used in the sketch and on the Serial Monitor:

- INPUT 1 to INPUT 8 are Digital Input 0 to Digital Input 7 in the datasheet
- OUTPUT 1 and OUTPUT 2 are Transistor Output 0 (T0) and Transistor Output 1 (T1) in the datasheet
- V 0 to V 3 are Analog Input 0 to Analog Input 3 (A0 to A3) in the datasheet

The program runs the following checks once at start-up, in this order:

- OLED display
- ADS1115 analog converter
- Wi-Fi access point
- LTE modem (`AT`, `AT+CPIN?`, `AT+GSN`, `AT+CSQ`)

The program then continuously:

- Passes anything typed in the Serial Monitor to the LTE modem and prints the modem reply
- Prints the front-panel button analog value
- Prints the state of INPUT 1 to INPUT 8 (0 or 1) on one line
- Prints the voltage of the four analog inputs in volts (V 0 to V 3)
- Switches OUTPUT 1 ON and OUTPUT 2 OFF for 200 ms, then OUTPUT 1 OFF and OUTPUT 2 ON for 200 ms
- Sends `RS485 01 SUCCESS` over RS-485 and prints any data received

All results are shown on the Serial Monitor.

Follow the steps below to run the example:

### 1. Hardware Connections

- Power the controller with 24 V DC.
- Connect the mini USB cable to your computer.
- Insert a SIM card and connect the LTE antenna (for the modem test).
- Connect digital input signals (18–32 V DC, sink or source) to INPUT 1 to INPUT 8.
- Connect 0–10 V signals to the 4 analog inputs.
- Connect loads (or a multimeter) to the transistor outputs.
- For the RS-485 test, connect an RS-485 device or a loopback to the RS-485 terminals.
- Ensure signals are within the supported input range.

### 2. Configure the Program

No additional user configuration is required for this example before upload.

### 3. Upload the Program

- Open the example program in Arduino IDE / PlatformIO
- Select the board **ESP32 Dev Module** and the correct port
- Upload the program to the NORVI controller

### 4. Observe the Serial Monitor

Open the Serial Monitor at **115200 baud**.

When the program runs successfully:

- The OLED display and the ADS1115 initialize
- The Wi-Fi access point starts and the MAC address, AP IP address and SSID are printed
- The LTE modem replies `OK` to `AT`, then returns the SIM status, IMEI and signal quality
- The front-panel button value is printed continuously
- The digital input states are printed continuously
- The analog input voltages are printed continuously
- The outputs switch ON and OFF alternately
- `RS485 01 SUCCESS` is sent continuously over RS-485 and is printed back if a loopback is connected

Example Output:

```
Hello

MAC Address: XX:XX:XX:XX:XX:XX
Setting up the Wi-Fi Access Point...
Access Point IP address: 192.168.4.1
Access Point SSID: XX:XX:XX:XX:XX:XX
Wi-Fi Access Point is active!

Testing Modem
AT
OK
+CPIN: READY
OK
XXXXXXXXXXXXXXX
OK
+CSQ: 20,99
OK
Testing Modem Done
Push button : 4095

11111111
V 0: 0.00 V
V 1: 0.00 V
V 2: 0.00 V
V 3: 0.00 V
RS485 01 SUCCESS
-------------------------------
```

(Sample values only. The eight digits show INPUT 1–8: 1 = OFF, 0 = ON. The RS-485 line appears only when a loopback or another device echoes the data.)

### How to Validate the Example

To verify the example works correctly:

1. Confirm the OLED display turns on and no `SSD1306 allocation failed` or `Failed to initialize ADS 1` message appears
2. Confirm the Wi-Fi access point appears on your phone or computer (SSID = device MAC address, password `12345678`)
3. Confirm the modem returns `OK` for `AT`, a SIM status for `AT+CPIN?`, the IMEI for `AT+GSN` and a signal value for `AT+CSQ`
4. Press each front-panel button and confirm the button value changes
5. Apply a valid signal to a digital input and observe the printed value change from 1 to 0
6. Apply a 0–10 V signal to an analog input and observe the printed voltage change
7. Observe the two outputs switching alternately (or check with a multimeter)
8. Connect an RS-485 device or loopback and confirm the data is received

### Device Preparation / Configuration

Before running the example ensure:

- The controller is powered correctly
- Required signals are connected
- SIM card and antenna are installed
- Any required configuration is completed

## Required Libraries

Install the following libraries before compiling.

- WiFi
- Wire
- Adafruit_GFX
- Adafruit_SSD1306
- Adafruit_ADS1X15

Installation:

1. Open Arduino IDE
2. Go to Library Manager
3. Search and install the required libraries (WiFi and Wire are included with the ESP32 core)

## Limitations

- This example is provided for demonstration purposes
- The OLED is initialized but no custom content is drawn in this example (only the library's default splash screen is shown)
- If the OLED or the ADS1115 is not detected, the program stops at start-up
- The LTE modem test runs once at start-up, takes about 11 seconds and needs a valid SIM card and network coverage
- After start-up, the modem is only reachable by typing commands in the Serial Monitor; typed commands are forwarded once per loop (about 1.2 seconds)
- The analog input voltages are calculated with a fixed voltage-divider ratio of 0.2065
- The digital input GPIOs (27, 34, 35, 14, 13, 5, 15, 19) follow the Version 2 terminal arrangement in the datasheet; on Version 1 units the input GPIOs differ, so verify the arrangement on your unit

## Safety Notes

- Do not exceed the rated input voltage (digital inputs 32 V DC maximum, analog inputs 38 V DC maximum)
- Do not exceed the transistor output rating (100 mA, 36 V DC maximum)
- Ensure proper grounding
- Disconnect power before wiring
- Incorrect wiring may damage the controller

Test Date: 11/11/2026  
Verified By: Kaveesha

## Support

Documentation:
https://norvi.io/docs

User guide: https://norvi.io/docs/norvi-gsm-ae02-v-l-user-guide/

For additional support or inquiries, contact the NORVI support team (support@norvi.io).

Datasheet : https://norvi.io/docs/norvi-gsm-ae02-v-l-datasheet/

## License

This example is provided for development and educational purposes.
