# Aerobic Septic System Monitor

**Table of Content**

[TOC]

## Overview

Recently I had an incident where sewer gas had forced it's way into the house. Among the question of how this happened was the obvious question "Is the aerobic septic system working?" Having recently found the joy of IoT that the ESP8266 and ESP32 bring with the ease of programming like an Arduino and simplified connection to Wi-Fi and the Internet I thought "My dishwasher, pet feeder, vacuum, and light switches are all connected to the Internet, why not my septic system!"

Thus the birth of this project which will look at monitoring an existing mechanical system (AC sensing), logging usage data and alerting system failures all through a Wi-Fi Internet connection. 

### What is an Aerobic Septic System

An an Aerobic Septic System, also referred to as an Aerobic Wastewater Treatment System, is a small-scale sewage treatment system similar to a septic tank, but which uses an aerobic process (adding air or oxygen) for digestion rather than the anaerobic (without air) process used in septic systems.

Adding air promotes the growth of organisms that break down the solids, which are put through a clarifier and chlorinated for disinfection which produces a cleaner, more environmentally friendly discharge. This cleaner discharge eliminates the need for a drainage field (leach field) allowing the system to be installed on a small property where a standard septic tank system would not have been possible. Ref: [Aerobic Septic Systems Explained | JT Septic Co | NE Oklahoma Septic Experts](https://jtsepticco.com/aerobic-septic-systems/)

### Components

The first and obvious thing you are going to need is an ESP32 board. I chose to go with a development board as they are easy to prototype with exposing all the pins along with a USB-UART bridge, reset- and boot-mode buttons, an LDO regulator and a micro-USB connector. I chose a ESP32-WROOM-32U. The U designates and external IPEX MHF4 antenna connector. While my aerobic septic control box (timers, alarms, etc.) is mounted on the house relatively close to the WiFi router many control boxes get mounted far from the house (100-200ft) at the aerobic tank.  An external antenna in the design made more sense since I was still dealing with a a reasonable distance from the router and a concreate wall.  Better safe, than sorry.

For an antenna I purchased a simple 8dBi antenna with a IPEX MHF4 to RP-SMA female pigtail cable that allowed for through hole mounting of the RP-SMA connector such that the antenna can be mounted outside a waterproof enclosure box. if you need more range I would suggest that you look at using a high gain Yagi directional antenna which can get you up to 20dBi.

While building a PCB to mount the ESP32 and other components was appealing, this project is a prototype and many of the sensor components are not designed well to place on a PCB board. So, I decided it would be best to make all connections to the ESP32 through a breakout board. This will also make the design more modular should a component need to be replaced.  Remember, I'm connecting sensors to AC circuits which are much more susceptible to power surges (i.e. lighting!).

Note that not all ESP32 development boards are equal when it comes to board width pin spacing. Some come in a 0.9" pin spacing, while others in a 1.0" pin spacing.  The HiLetgo board I chose comes in a 1.0" pin spacing. Not realizing this and thinking that spacing was the same for all development boards I initially bought a breakout board with the smaller 0.9" pin spacing. The one suggested in following parts list will accept both the 0.9" and 1.0" pin spacing.

The basic function of this project is to measure if the power is on/off and if the pump is actually running.  To determine if the power is on (i.e. the timer has switched on) I found some 220V optocoupler modules that can output a 3-5 volt TTL (digital) signal.  Connected to any GPIO (general-purpose input/output) pin you get 1 for on and 0 for off. 

While the power may be turned on, there is no guarantee that the pump is actually running since the pump circuit contains a low water level float switch designed to turn the pump off when the tank is empty. This means that I have to monitor the current flow in the circuit to determine if the pump is really running. The current sensor I chose only measures 0-5 amps. Should your pump pull more than 5 amps this is not an issue as we are only interested if current is flowing and not how much current is flowing.  My choice was purely economic. These sensors are cheaper than one that could read a full 20 amp range. That said, I may opt for the more expensive sensor in the future to look at pump load such that I might determine things like a clogged intake or sprinkler outlet.

To log data I will need an SD card and real-time clock(RTC) to correctly timestamp the data. An Arduino compatible SD card reader with a SPI interface are extremity easy to find , as well as an I2C DS3231 real-time clock module. The real-time clock module suggested below also contains an AT24C32 EPROM memory chip which I will need to store things like the Wi-Fi SSID/password and administrator users/passwords. If you don't buy a RTC with a memory chip you can add a separate memory module.

While it's not necessary I thought it would be nice to add an OLED display. These are small, cheap and run on I2C. For this project the plan is to display things like the IP address (maybe even a QR code) and other useful status Information for when you open the enclosure to check/program.

To power the ESP32 and added modules I will need a USB wall charger and cable. The plan is to run power into the added enclosure using an extension cord with the female plug side in the box and male plug side cut off and spliced into the aerobic system power terminals. This way any standard USB wall charger can be plugged in to the female plug. The reason for the 6 foot cable is that it allows you to walk out to the septic system with your laptop and plug into ESP32 to diagnose and/or program as needed.

#### Parts List

| Quantity | Description                                             | Source Suggestion                                            |
| -------- | ------------------------------------------------------- | ------------------------------------------------------------ |
| 1        | ESP32 Development Board                                 | [Amazon.com: HiLetgo 2pcs ESP32-DevKitC ESP32-WROOM-32U Core Board ESP32 ESP-32 ESP-WROOM-32U Development Board for Arduino : Electronics](https://www.amazon.com/dp/B09KLS2YB3?psc=1&ref=ppx_yo2ov_dt_b_product_details) |
| 1        | Antenna                                                 | [Amazon.com: 2 x 8dBi WiFi RP-SMA Male Antenna 2.4GHz 5.8GHz Dual Band +2 x 15CM U.FL/IPEX to RP-SMA Female Pigtail Cable for Mini PCIe Card Wireless Routers, PC Desktop, Repeater, FPV UAV Drone and PS4 Build : Electronics](https://www.amazon.com/dp/B07R21LN5P?psc=1&ref=ppx_yo2ov_dt_b_product_details) |
| 1        | Breakout Board                                          | [Amazon.com: naughtystarts for ESP32 Breakout Board 3.5mm / 0.14" Terminal GPIO Expansion Board for 0.9" / 1.0" Size ESP32 Module ESP-WROOM-32 ESP32-DevKitC (Pack of 2pcs) : Electronics](https://www.amazon.com/dp/B0BYS6THLF?psc=1&ref=ppx_yo2ov_dt_b_product_details) |
| 4        | 220V Optocoupler Module                                 | [Amazon.com: JESSINIE 3Pcs 1Channel AC 220V Optocoupler Module 220V Voltage Detect 220V Optocoupler Isolation Microcontroller TTL Level 3V-5V : Industrial & Scientific](https://www.amazon.com/dp/B0C73GGKHX?psc=1&ref=ppx_yo2ov_dt_b_product_details) or [HiLetgo AC 220V Optocoupler Module AC Optocoupler Alternating Current Detect Module 3-5V or PLC 24V Level Power for PLC MCU(1 Channel): Amazon.com: Industrial & Scientific](https://www.amazon.com/HiLetgo-Optocoupler-Alternating-Current-Channel/dp/B0CHJLYY3D/ref=sr_1_4?crid=3NMY4ZOHAL63&keywords=220V%2BOptocoupler%2BModule&qid=1707616569&s=industrial&sprefix=220v%2Boptocoupler%2Bmodule%2Cindustrial%2C115&sr=1-4&th=1) |
| 1        | Current Sensor                                          | [3PCS Analog Current Meter Sensor Module AC 0~5A Ammeter Sensor Board Based on ZMCT103C for Arduino: Amazon.com: Industrial & Scientific](https://www.amazon.com/dp/B0B6CNC6MM?psc=1&ref=ppx_yo2ov_dt_b_product_details) |
| 1        | SD Card Reader                                          | [Amazon.com: HiLetgo 5pcs Micro SD TF Card Adater Reader Module 6Pin SPI Interface Driver Module with chip Level Conversion for Arduino UNO R3 MEGA 2560 Due : Electronics](https://www.amazon.com/dp/B07BJ2P6X6?psc=1&ref=ppx_yo2ov_dt_b_product_details) |
| 1        | Real-Time Clock w/Memory                                | [Amazon.com: HiLetgo 5pcs DS3231 AT24C32 Clock Module Real Time Clock Module IIC RTC Module for Arduino Without Battery : Industrial & Scientific](https://www.amazon.com/dp/B00LX3V7F0?psc=1&ref=ppx_yo2ov_dt_b_product_details) Alternate Memory Module [EC Buying 5Pcs AT24C02 Module I2C IIC Interface EEPROM Memory Module Intelligent Car Accessories with Dupont Wire at Amazon.com](https://www.amazon.com/dp/B0C73CRV1G?ref=ppx_yo2ov_dt_b_product_details&th=1) |
| 1        | Display (optional)                                      | [Amazon.com: Frienda 10 Pieces I2C OLED Display Module OLED Display Screen Driver IIC I2C Tabellone Seriale con Display Auto-Luminoso Compatible with Arduino/Raspberry PI : Electronics](https://www.amazon.com/dp/B08VNRH5HR?ref=ppx_yo2ov_dt_b_product_details&th=1) |
| 1        | USB Wall Charger                                        | [Amazon.com: (3 Pack) USB C Wall Charger, Dual Port PD Power Adapter Fast Charging Block for iPhone 15/15 Pro/15 Pro Max/15 Plus/14/13/12/11,X, Pad, Google Pixel, Samsung Galaxy and More : Cell Phones & Accessories](https://www.amazon.com/dp/B0BDWYL28S?ref=ppx_yo2ov_dt_b_product_details&th=1) |
| 1        | 6' USB Cable                                            | [Amazon.com: AILKIN Micro USB Cable, 5-Pack 6ft High Speed Nylon Braided Android Charging Cables for Samsung Galaxy J8/J7/S7/S6/Edge/Note5, Sony, Motorola, HTC, LG Android Tablets and More USB to Micro USB Cords : Electronics](https://www.amazon.com/dp/B071H25C43?ref=ppx_yo2ov_dt_b_product_details&th=1) |
| 1        | 220 Ω 1/4W Resistor (optional, beta testing)            | [Amazon](https://www.amazon.com/), [DigiKey](https://www.digikey.com/), or [Mouser Electronics](https://www.mouser.com/) |
| 1        | 5mm LED (optional, beta testing)                        | [Amazon](https://www.amazon.com/), [DigiKey](https://www.digikey.com/), or [Mouser Electronics](https://www.mouser.com/) |
| 1        | 10K Ω 1/4W Resistor (optional, voltage divider circuit) | [Amazon](https://www.amazon.com/), [DigiKey](https://www.digikey.com/), or [Mouser Electronics](https://www.mouser.com/) |
| 1        | 20K Ω 1/4W Resistor (optional, voltage divider circuit) | [Amazon](https://www.amazon.com/), [DigiKey](https://www.digikey.com/), or [Mouser Electronics](https://www.mouser.com/) |

### Visual Studio Code

While the [Arduino IDE 2](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started-ide-v2/) has come a long way from what is now referred to as the Arduino Legacy IDE or Arduino IDE 1, a lot of developers including myself prefer to use VS Code, short for [Visual Studio Code](https://code.visualstudio.com/). VS Code is an extremely versatile editor that through the implementation of extensions can work across multiple languages and platforms. To use VS code you will need to install the [Arduino](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino) extension. You might also want to look at the [PlatformIO IDE for VS Code](https://platformio.org/) extension.

## Beta Testing

The beta testing chapter is a look at each of the different components that make up the Aerobic Septic System Monitor. Each section is an exercise in connecting and programming a component.  If you are new to the Arduino/ESP32 you will want to work through each section to gain a better understanding of each component attached to the ESP32. This chapter is optional and if you want to get directly to monitoring your aerobic septic system you can jump to "v1.0 The Basics".

### Blink (ESP32)

If you are new to the Arduino/ESP32 world, blink is the classic "Hello World!" program for the Arduino developer.  This exercise involves using the build-in LED on pin 13 of many Arduino boards or wiring to an LED to an one of the many digital pins with a 220 ohm ballast resistor to limit the current flow. Then loading a program, in the Arduino world know as a sketch that causes the LED to blink. The following is a wiring example for attaching an LED.

![Blink Schematic](.\images\Blink Schematic.png)

I my case, I used the breakout board and an Ideal Lever Wire Connector to connect the LED and 220 ohm resistor without the need to solder.

![Blink Photo](.\images\Blink Photo.jpg)

The blink sketch is includes in this project in the folder labeled ```\Blink```.  With the ESP32 Development Board I chose there is no built in LED so I had to manually redefine the LED_BUILTIN constant in the code. In my case I used pin 4. If you are new to programming an Arduino/ESP32 I recommend starting by reading "[Getting Started with Arduino IDE 2](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started-ide-v2/)". You can also search the internet for "Arduino blink" or "programming the Arduino" to find a great deal of articles and tutorials.

### Beyond Blink (ESP32)

Beyond Blink is an exercise to look at the simplicity and power the ESP32 has when it comes to connecting to your Wi-Fi network and the Internet. The code was adapted from the [Last Minute Engineers - Learn Electronics the Easy Way](https://lastminuteengineers.com/) article [In-depth: Create A Simple ESP32 Web Server In Arduino IDE (lastminuteengineers.com)](https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/) and demonstrated how to connect your ESP32 to a Wi-Fi network hosing a web page that presents a button that can be used to turn the LED on and off.

This exercise uses the same wired LED from the Blink exercise. You will again need to edit the code to modify LED_BUILTIN if you are using a pin other than 4 for the LED. You will also need to edit the ```configuration.h``` header file to add your Wi-Fi SSID and password. The code can be found in the ```\BeyondBlink``` folder.

### Is the Power On (220V Optocoupler Module)

As part of this project I need to be able to see if the power is on. In particular, I want to monitor the pump timer, the air compressor pump timer, the high water alarm and the air compressor alarm. To do this I will use a simple optocoupler module which can be connected to the hot 120V lead ('L' for load, typically a black wire) of any of the items I previously listed and common ('N' for neutral, typically a white wire) on one side of the module. On the other side I connect the module to 3.3 voltage (VCC - positive voltage), ground (GND - ground, negative) and one of the GPOI (General-Purpose Input-Output) pins of the ESP32 (OUT - signal). You can learn more about the ESP32 GPOI pins at [ESP32 Pinout Reference: Which GPIO pins should you use? | Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/). Below is a diagram for a test circuit. 

![Is the Power On Schematic](.\images\Is the Power On Schematic.png)

The following is a photo of the AC circuit test board I built for testing based on the test circuit above. When plugged in and the the switch is on, the plug has power and the optocoupler module (green PCB (Printed Circuit Board), far right) will sense voltage. In the next module I will discuss the current sensor (blue PCB, bottom).

 ![](.\images\AC Test Circuit.jpg)

The code can be found in the ```\IsThePowerOn``` folder and is extremely simplistic. Basically when power is sensed the LED I wired in the previous exercises is turned on and a message is sent to the serial port.  As before you will need to change the ```LED_BUILTIN``` constant if you use a pin other than GPIO 4.  You will also need to change the ```POWER_ON_OPTOCOUPLER_MODULE``` constant if you us a GPIO pin other than 34. In the photo below VCC on the optocoupler is wired to 3V3, OUT to GPIO pin 43 and GND to GND on the development breakout board.

![Is The Power On Wiring](.\images\Is The Power On Wiring.jpg)

### Is the Pump Running (Current Sensor)

While determining if the pump has been turned on by either the pump timer or the high water override float is a simple mater of using a 220V optocoupler module to detect voltage, I can't always be certain that the pump is running since a low water float is tied directly to the pump circuit after the control box wiring. The low water float is wired in to the circuit to keep the pump from running and possibly burning out when there is no water in the septic clear water tank. 

Now I could loop back a wire from the low water float/pump connection and wire it to a 220V optocoupler module to determine if the pump was running, but depending on the distance of the control box from the pump this could be extremely difficult. Also, since the low water float is most likely wired directly to one of the pump leads it is also most likely connected with a special water proof heat shrink connector which complicates things and making this project more complicating is not an objective.

For this reason I've chosen to use a current sensor. A current sensor will tell us if current is being drawn through the pump circuit, hence the pump is running. After a good bit of research I chose a current sensor based on the ZMCT103C current transformer that uses an LM358 with dual operational amplifiers. The sensor PCB design is open source and while it can be easily purchased from multiple sources including [Amazon](https://www.amazon.com/), [eBay](https://www.ebay.com/) and [AliExpress](https://www.aliexpress.us/). It can be a bit difficult to find good specification details and documentation. However, after some digging on the Internet I was able to find the following schematic and other relevant details from the following sites.

[Current_sensor_ZMCT103C_LM358 - EasyEDA open source hardware lab (oshwlab.com)](https://oshwlab.com/sulagaev/current_sensor_zmct103c_lm358)

[ZMCT103C Precision Current Sensor : Datasheet & Its Applications (watelectronics.com)](https://www.watelectronics.com/zmct103c-precision-current-sensor/)

[ZMCT103C - Precision Current Sensor Pinout, Features, Datasheet, Working, Application, Alternative (components101.com)](https://components101.com/sensors/zmct103c-precision-current-sensor-pinout-features-datasheet-alternative)

![Current Sensor ZMCT103C LM358 Schematic](.\images\Current Sensor ZMCT103C LM358 Schematic.png)

<img src=".\images\Current Sensor ZMCT103C LM358 PCB.png" alt="Current Sensor ZMCT103C LM358 PCB" style="zoom:190%;" /> <img src=".\images\Current Sensor ZMCT103C LM358.jpg" alt="Current Sensor ZMCT103C LM358" style="zoom:35%;" />

| Name     | Designator  | Footprint                      | Quantity |
| -------- | ----------- | ------------------------------ | -------- |
| 1uF      | C1,C4,C6    | C0805                          | 3        |
| 10nF     | C2,C5       | C0805                          | 2        |
| 10uF     | C3          | C0805                          | 1        |
| ZMCT103C | CT1         | ZMCT103C_NOWIRE                | 1        |
| green    | LED1        | LED0402-RD                     | 1        |
| SIP4     | P1          | HDR1X4                         | 1        |
| 100      | R1          | R0805                          | 1        |
| 1k       | R2,R3,R4,R6 | R0805                          | 4        |
| 100k     | R5,R9       | R0805                          | 2        |
| 10k      | R7,R10,R11  | R0805                          | 3        |
| 100K     | R12         | RES-ADJ-TH_3296W               | 1        |
| LM358    | U1          | SOP-8_L4.9-W3.9-P1.27-LS6.0-BL | 1        |

There are some concerns with using this sensor configuration. First, the amperage range is only 5A (amps). Since most aerobic septic system pumps are at least 1/2 Hp they can easily pull 10 or more amps. The circuit breaker for the pump on my system is 20A. Now you can buy current sensors that can read 20-30A, but they will cost you at least $20, where this sensor averages $3. Not that cheaper is better, but I am trying to keep this project both simple and inexpensive. In the end I don't need to know how much current the pump is drawing, just that it is drawing current. 

The 5A limit also raises the concern of how much current can be going through the AC wire that loops through the ZMCT103C coil. After some digging I first discovered that the ZMCT103C can actually sense up to 10A. And after even more digging I found a specification maximum of 20A. 

The second concern is that the sensor circuit requires 5V (volts). 5V in means, that the analog output could get as high as 5V out. Now the ESP32 development board does offer a 5V power source since it operates off of the power in the USB cable. The problem here is that the ESP32 GPIO pins are only rated for 3.3V. The obvious solution to this is to use a voltage divider circuit between the sensor and the ESP32. The circuit consist of two resistor.

![Voltage Divider Schematic](C:\Users\tgoll\source\repos\AerobicSepticSystemMonitor\images\Voltage Divider Schematic.png)

Based on the value of the two resistors and the maximum input voltage, you can calculate the maximum output voltage with the following equation.

![Voltage Divider Formula](C:\Users\tgoll\source\repos\AerobicSepticSystemMonitor\images\Voltage Divider Formula.svg)

If you want to learn more just Google "voltage divider circuit". For our situation a solution can easily be accomplished with a 10K ohm (R1) resistor and 20K ohm (R2) resistor.

Now after considering a voltage divider circuit I did some further digging into the sensor circuit. It turns out that the LM358 amplifier has an operating voltage range of 3-30V. So why not power the sensor with the 3.3V power supply. Well I'm rather certain after looking at the sensor circuit schematic and without reading all of the data sheets on the ZMCT103C that the resistors in conjunction with the ZMCT103C have been calculated to split the voltage such that 5A translates to a 5V range. So running the circuit on 3.3V will most likely not translate 5A in to a 3.3V range. But again, I don't care about exactly measuring the amperage, I just want to know there is amperage.

*** Need to Test 3.3V



### Where is my Data (SD Card Reader)



### What Time is it? (RTS DS3231)



### Don't Let me Forget (AT24C32)



### Show me (OLED Display)



*** This project is a work in progress. Please click the "Watch" button so you get notification on issues, discussions and pull request in GitHub. If you find a bug, please open an issue. If you have a question or comment please start a discussion. If you like what you see click the "Star" button. And please return in the future to see our progress.* 

## v1.0 The Basics

*** This project is a work in progress. Please click the "Watch" button so you get notification on issues, discussions and pull request in GitHub. If you find a bug, please open an issue. If you have a question or comment please start a discussion. If you like what you see click the "Star" button. And please return in the future to see our progress.* 

## v2.0 Logging

*** This project is a work in progress. Please click the "Watch" button so you get notification on issues, discussions and pull request in GitHub. If you find a bug, please open an issue. If you have a question or comment please start a discussion. If you like what you see click the "Star" button. And please return in the future to see our progress.* 

## v3.0 Alerting

*** This project is a work in progress. Please click the "Watch" button so you get notification on issues, discussions and pull request in GitHub. If you find a bug, please open an issue. If you have a question or comment please start a discussion. If you like what you see click the "Star" button. And please return in the future to see our progress.* 
