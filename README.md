## Arduino compatible libraries

**Note:** Unless otherwise specified, the libraries are distributed under the terms of GNU General Public License version 3.

### Display

#### TM1628 (TM1628ts)

A 7-Segment LED Driver from *Titan Microelectronics Co.,Ltd.* Library for a DVD player front panel (TeleSystem TS5.9RX). Uses a serial protocol similar to SPI. [Information and examples](https://www.onetransistor.eu/2017/06/tm1628-7segment-display-arduino.html).

![TM1628 panel segments](https://2.bp.blogspot.com/--RBNngst1g8/XMMIlo5YStI/AAAAAAAAMjo/cK7IXY8uYWAxbXFlDY5vyQ4Vy7gaYojagCLcBGAs/s400/tm1628.png)

#### SC75823

13-Segment LCD Driver from *Silan Microelectronics Co.,Ltd.* Used in the front panel of a car radio & CD player. [Information and examples](https://www.onetransistor.eu/2017/06/sc75823-13-segment-lcd-display-arduino.html).

![SC75823 panel segments](https://3.bp.blogspot.com/--kiwzZxx1Vw/XMMJfCByoxI/AAAAAAAAMjw/s_C7tcpYRRIKvjiKDGwmUT83P8i0n7kOACLcBGAs/s400/sc75823.png)

#### HT16515

15-Segment VFD Driver from *Holtek Semiconductor Inc.* Used with *Futaba Corporation* **FV865ND** VFD panel for a satellite receiver. [Information and examples](https://www.onetransistor.eu/2017/06/fv865nd-ht16515-arduino.html).

![HT16515 with FV865ND](https://3.bp.blogspot.com/-ofvk8MYT7i0/XMMKREO7l4I/AAAAAAAAMj4/5o2mcjpgOxgPffEP0v1cJVPFCCDLS6sbQCLcBGAs/s400/ht16515.png)

#### HX8347-I (Truly_HX8347I)

LCD driver from *Himax Technologies, Inc.* Integrated in *Truly International Holdings, Ltd.* **TFT3P5026-E** 320x240 color TFT 2.8" display. Used at front panel of Sercomm SHG1500 router (hardware revision LCDv6). Front panel has 2x15 pin 1.27 mm pinheader connector. Display uses SPI protocol (write only, no MISO). Includes backlight control. [Information and examples](https://www.onetransistor.eu/2018/10/router-lcd-and-keypad-to-arduino.html). Backlight only library is **TrulyBacklight** (not needed anymore).

![SHG1500 Front Panel](https://1.bp.blogspot.com/-zAk4kzmVo0Y/W7ixtLYi-eI/AAAAAAAALOI/-jWgCXYncf0eomH2yKayEUc7eO58Rvf6gCLcBGAs/s400/front_panel.jpg)

### Device Control

#### MxL5007T

An integrated VHF/UHF tuner from *MaxLinear, Inc.* Taken from a DVB-T receiver and placed in a metallic box from a broken tuner. [Information and examples](https://www.onetransistor.eu/2014/08/mxl5007t-based-radio.html).

![MxL5007T tuner](https://1.bp.blogspot.com/-8Kf_FHSy4bo/W7kC8uvH6VI/AAAAAAAALPg/gXfYyP14o2ofT1IRQeUDeoLqIhHQeg7-gCLcBGAs/s400/mxl5007t_metal_case.jpg)

#### SX8646

Ultra Low Power, Capacitive Button Touch Controller (8 sensors) with Enhanced LED Drivers from *Semtech Corporation*. Found on the front panel keypad of Sercomm SHG1500 routers. [Information and examples](https://www.onetransistor.eu/2019/04/arduino-library-semtech-sx8646.html).

![SX8646 button panel](https://3.bp.blogspot.com/-cpDmO5muxRk/XMV4FgVX-8I/AAAAAAAAMmk/fcvBjeqDAGskOBsLN2bIqDq21ZEW7kosQCLcBGAs/s400/shg1500_keypad.jpg)

