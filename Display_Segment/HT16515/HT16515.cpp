/*
	Library for Holtek HT16515 VFD Driver with FV865ND display
	
	Known Limitations: Left indicators are multiplexed and do not work
	
	Copyright (C) 2017 One Transistor [https://onetransistor.github.io]

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include "Arduino.h"
#include "HT16515.h"

HT16515::HT16515(int clockPin, int csPin, int dataWritePin, int dataReadPin) {
	ht_clk = clockPin;
	ht_cs = csPin;
	ht_di = dataWritePin;
	ht_do = dataReadPin;
}

void HT16515::init(int intensity) {
	for (int i = 0; i < 36; i++)
		ht_buffer[i] = 0x00;
	
	pinMode(ht_clk, OUTPUT);
	pinMode(ht_cs, OUTPUT);
	pinMode(ht_di, OUTPUT);
	pinMode(ht_do, INPUT);
	
	digitalWrite(ht_clk, HIGH);
	digitalWrite(ht_cs, HIGH);
	
	delay(200);
	
	sendCommand(0x04); // display mode

	writeBuffer();

	setIntensity(intensity);
}

void HT16515::sendByte(byte data) {
	for (int i = 0; i < 8; i++) {
		digitalWrite(ht_clk, LOW);
		digitalWrite(ht_di, data & 1 ? HIGH : LOW);
		data >>= 1;
		digitalWrite(ht_clk, HIGH);
	}
}

byte HT16515::receiveByte() {
	byte temp = 0;

	for (char i = 0; i < 8; i++) {
		temp >>= 1;
		digitalWrite(ht_clk, LOW);
		if (digitalRead(ht_do))
			temp |= 0x80;
		digitalWrite(ht_clk, HIGH);
	}

	return temp;
}

void HT16515::sendCommand(byte data) {
	digitalWrite(ht_cs, LOW);
	sendByte(data);
	digitalWrite(ht_cs, HIGH);
}

void HT16515::writeBuffer() {
	sendCommand(0x40); // data setting
	digitalWrite(ht_cs, LOW);
	sendByte(0xc0); // address setting
	for (int i = 0; i < 36; i++)
		sendByte(ht_buffer[i]);
	digitalWrite(ht_cs, HIGH);
}

void HT16515::clearBuffer() {
	for (int i = 0; i < 36; i++)
		ht_buffer[i] = 0x00;
}

byte HT16515::readKeyboard() {
	byte keys = 0;

	digitalWrite(ht_cs, LOW);
	sendByte(0x42); // command 2
	for (int i = 0; i < 5; i++)
		keys |= receiveByte() << i;
	digitalWrite(ht_cs, HIGH);

	return keys;
}

void HT16515::putCharAt(byte data, int pos) {
	int offset = (pos - 1) * 3;

	for (int i = 0; i < 15; i++)
		bitWrite(ht_buffer[offset + i / 8], i % 8, bitRead(ht_font[data], i));
}

void HT16515::putDigitAt(byte digit, int pos) {
	int offset = (pos - 1) * 3;
	int fontOff = 0x10;
	if (digit > 9)
		fontOff = 0x17;

	for (int i = 0; i < 15; i++)
		bitWrite(ht_buffer[offset + i / 8], i % 8, bitRead(ht_font[digit + fontOff], i));
}

void HT16515::putString(char str[8]) {
	for (int i = 8; i; i--)
		if (str[8 - i]) {
			char c = str[8 - i] - 0x20;
			putCharAt(c, i);
		}
		else break;
}

void HT16515::setIntensity(int intensity) {
	if (intensity < 0)
		sendCommand(0x80); // turn off
	else
		sendCommand(0x88 | min(intensity, 7)); // control display
}

void HT16515::setIndicator(int indicator, int on) {
	if (indicator > 0) {
		bitWrite(ht_buffer[indicator / 8], indicator % 8, on % 2);
		return;
	}
	
	// TO DO: Multiplexed indicators
	/*for (int i = 0; i < 7; i++)
		if (bitRead(ht_buffer[3 * i + (19 + indicator) % 2], (19 + indicator) % 2)) { // search for a segment
			bitWrite(ht_buffer[3 * i + 2], 3, on % 2);
			break;
		}*/
}

void HT16515::putNumberAt(long int num, int startpos, int negative, byte base) {
	if ((startpos > 8) || (base < 2) || (base > 16))
		return;

	byte pos = startpos;

	do {
		putDigitAt(num % base, pos);
		num = num / base;
		pos++;
	} while ((num > 0) && (pos <= 7));

	if ((pos < 7) && (negative))
		putCharAt(13, pos);
}