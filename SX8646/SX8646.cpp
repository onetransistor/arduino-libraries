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

#include "SX8646.h"

/***** Outside of SX8646 class *****/
SX8646 *pThis;

void handleInterrupt(void) {
	pThis->handleInterruptInClass();
}

/***** Class constructor *****/
SX8646::SX8646(int INTB) {
	pThis = this;
	_INTB = INTB;
}

void SX8646::begin(void) {
	pinMode(_INTB, INPUT);
	Wire.begin();
	delay(150); // maximum t_POR

	initSPM();
	
	// enable interrupt
	attachInterrupt(digitalPinToInterrupt(_INTB), handleInterrupt, FALLING);
}

void SX8646::setKeyCallback(void (*func)(const uint8_t)) {
	keyHandler = func;
}

/*void SX8646::softwareReset(void) {
	writeRegister(0xB1, 0xDE);
	writeRegister(0xB1, 0x00);

	delay(150); // maximum t_POR
}*/

void SX8646::initSPM(void) {
	uint8_t reg, spm[8];

	/* At power up NVM is checked for valid data. If empty/invalid it uses
	   QSM. Settings from either NVM or QSM are copied to SPM and used.
	   SPM is RAM memory, contents get lost after reset or power up. */
	reg = readRegister(0x08);
	
	if (reg & 0x08 != 0) return; // NvmValid=1

	spmRead(0x08, &spm[0]);
	spm[5] = 0x00; // CapSensitivity0_1
	spmWrite(0x08, &spm[0]);

	spmRead(0x08, &spm[0]);
	spm[5] = 0x70; // CapSensitivity0_1
	spmWrite(0x08, &spm[0]);

	// set I2C compensation and active mode
	writeRegister(0x09, 0x04);

	handleInterruptInClass(); // not yet enabled

	spmRead(0x10, &spm[0]);
	spm[3] = 0xFF; // CapThresh0
	spmWrite(0x10, &spm[0]);

	handleInterruptInClass(); // not yet enabled

	spmRead(0x18, &spm[0]);
	spm[1] = 0xFF; // CapThresh6
	spm[2] = 0xFF; // CapThresh7
	spmWrite(0x18, &spm[0]);
}

void SX8646::handleInterruptInClass(void) {
	uint8_t irqSrc = 0x00;

	irqSrc = readRegister(0x00);

	if (irqSrc & 0x04) { // Buttons interrupt flag
		keyHandler(readRegister(0x02));
		return;
	}

	readRegister(0x00);
}

/***** Medium level API *****/

void SX8646::spmRead(uint8_t addr, uint8_t *spm) {
	writeRegister(0x0D, 0x18); // I2C in SPM: SpmCfg[5:4]=1, Read mode: SpmCfg[3]=1
	delay(30); // not specified by datasheet but used by router software
	writeRegister(0x0E, addr); // SpmBaseAddr
	readRegisters(0x00, &spm[0], 8);
	writeRegister(0x0D, 0x00); // reset SpmCfg
}

void SX8646::spmWrite(uint8_t addr, uint8_t *spm) {
	writeRegister(0x0D, 0x10); // I2C in SPM: SpmCfg[5:4]=1
	delay(30); // section 6.6.1, page 62 in datasheet - not quite clear
	writeRegister(0x0E, addr); // SpmBaseAddr
	delay(30); // whether all these delays are needed
	writeRegisters(0x00, &spm[0], 8);
	delay(30); // but they are used by router software
	writeRegister(0x0D, 0x00); // reset SpmCfg
}

/***** Low level API *****/

void SX8646::writeRegisters(uint8_t addr, uint8_t *regs, int length) {
	if (length < 1) return;

	Wire.beginTransmission(SX8646_ADDR);
	Wire.write(addr);

	for (int i = 0; i < length; i++)
		Wire.write(regs[i]);

	Wire.endTransmission(true);
}

void SX8646::writeRegister(uint8_t addr, uint8_t val) {
	writeRegisters(addr, &val, 1);
}

void SX8646::readRegisters(uint8_t addr, uint8_t *regs, int length) {
	if (length < 1) return;

	Wire.beginTransmission(SX8646_ADDR);
	Wire.write(addr);
	Wire.endTransmission(false);

	Wire.requestFrom(SX8646_ADDR, length);
	while (Wire.available())
		*regs++ = Wire.read();
}

uint8_t SX8646::readRegister(uint8_t addr) {
	uint8_t reg[1] = { 0xFF };

	readRegisters(addr, &reg[0], 1);
	return reg[0];
}

