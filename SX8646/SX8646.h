/*
  Arduino library for Semtech SX8646 Ultra Low Power, Capacitive Button
  Touch Controller (8 sensors) with Enhanced LED Drivers

  Product page:
  https://www.semtech.com/products/smart-sensing/touch-proximity-devices/SX8646

  Copyright (C) 2019 One Transistor <one1transistor@gmail.com>
  https://www.onetransistor.eu/2019/04/arduino-library-semtech-sx8646.html

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SX8646_H
#define SX8646_H

#include "Arduino.h"
#include "Wire.h"

#define SX8646_ADDR	0x2B

// key assignments for Sercomm SHG1500 front panel
#define PANEL_NONE	0x00
#define PANEL_RIGHT	0x02
#define PANEL_DOWN	0x04
#define PANEL_LEFT	0x08
#define PANEL_OK	0x10
#define PANEL_UP	0x20

class SX8646 {

public:
	SX8646(int INTB);
	void begin(void);
	//void softwareReset(void); // not realy needed
	void setKeyCallback(void (*func)(const uint8_t));

	void handleInterruptInClass(void); // do not call!

private:
	int _INTB;
	void (*keyHandler)(const uint8_t);

	void initSPM(void);

	// SPM = Shadow Parameter Memory (must be read/written in blocks of 8 bytes!)
	void spmRead(uint8_t addr, uint8_t *spm);
	void spmWrite(uint8_t addr, uint8_t *spm);

	void writeRegisters(uint8_t addr, uint8_t *regs, int length);
	void writeRegister(uint8_t addr, uint8_t val);
	void readRegisters(uint8_t addr, uint8_t *regs, int length);
	uint8_t readRegister(uint8_t addr);
};

#endif
