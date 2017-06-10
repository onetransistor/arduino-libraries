/*
	Library for Silan SC75823 LCD controller

	Copyright (C) 2017 One Transistor [http://onetransistor.github.io]

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
#include "SC75823.h"

SC75823::SC75823(int clockPin, int dataPin, int cePin, int inhPin)
{
	sc_clk = clockPin;
	sc_data = dataPin;
	sc_ce = cePin;
	sc_inh = inhPin;
}

void SC75823::init()
{
	pinMode(sc_clk, OUTPUT);
	pinMode(sc_data, OUTPUT);
	pinMode(sc_ce, OUTPUT);
	pinMode(sc_inh, OUTPUT);

	digitalWrite(sc_clk, LOW);
	digitalWrite(sc_ce, LOW);
	digitalWrite(sc_inh, HIGH);

	delay(100); // Just in case
}

void SC75823::sendByte(byte data)
{
	for (int i = 0; i < 8; i++)
		{
			digitalWrite(sc_data, (data & 1) ? HIGH : LOW);
			digitalWrite(sc_clk, HIGH);
			digitalWrite(sc_clk, LOW);
			data >>= 1;
		}
}

void SC75823::sendData()
{
	digitalWrite(sc_ce, HIGH);
	for (int i = 0; i < 20; i++)
		sendByte(sc_buffer[i]);
	digitalWrite(sc_ce, LOW);
}

void SC75823::clearBuffer()
{
	for (int i = 0; i < 20; i++)
		sc_buffer[i] = 0x00;
}

void SC75823::sendAddress(byte address)
{
	digitalWrite(sc_ce, LOW);
	sendByte(address);
}

void SC75823::writeBuffer()
{
	sendAddress(0x41);
	sendData();
}

void SC75823::putCharAt(byte data, int pos)
{
	for (int i = 1; i <= 14; i++)
		{
			int bitOff = 15 * (8 - pos) + i;
			bitWrite(sc_buffer[bitOff / 8], bitOff % 8, bitRead(sc_font[data], i));
		}
}

void SC75823::putSmallDigit(byte digit)
{
	if (digit > 16) return;

	bitWrite(sc_buffer[1], 7, bitRead(sc_digit[digit], 0));
	bitWrite(sc_buffer[3], 6, bitRead(sc_digit[digit], 1));
	bitWrite(sc_buffer[5], 5, bitRead(sc_digit[digit], 2));
	bitWrite(sc_buffer[7], 4, bitRead(sc_digit[digit], 3));
	bitWrite(sc_buffer[12], 6, bitRead(sc_digit[digit], 4));
	bitWrite(sc_buffer[13], 1, bitRead(sc_digit[digit], 5));
	bitWrite(sc_buffer[14], 5, bitRead(sc_digit[digit], 6));
}

void SC75823::putString(char str[8])
{
	for (int i = 8; i; i--)
		if (str[8 - i])
			{
				char c = str[8 - i] - 0x20;
				putCharAt(c, i);
			}
}

void SC75823::setSignal(int strength)
{
	bitWrite(sc_buffer[18], 7, 1); // show icon
	switch (strength)
		{
		case 3:
			bitWrite(sc_buffer[18], 1, 1); // 3
		case 2:
			bitWrite(sc_buffer[18], 3, 1); // 2
		case 1:
			bitWrite(sc_buffer[18], 5, 1); // 1
			break;
		case 0:  // show antenna icon with no signal
			bitWrite(sc_buffer[18], 5, 0); // 1
			bitWrite(sc_buffer[18], 3, 0); // 2
			bitWrite(sc_buffer[18], 1, 0); // 3
			break;
		default:
		case -1: // hide antenna icon
			bitWrite(sc_buffer[18], 7, 0);
			bitWrite(sc_buffer[18], 1, 0); // 3
			bitWrite(sc_buffer[18], 3, 0); // 2
			bitWrite(sc_buffer[18], 5, 0); // 1
			break;
		} ;
}

void SC75823::setIndicator(int indicator, int on)
{
	bitWrite(sc_buffer[indicator / 8], indicator % 8, on);
}

void SC75823::setInnerDisc(int centre, byte mask)
{
	bitWrite(sc_buffer[19], 0, centre % 2);
	for (int i = 0; i < 3; i++)
		bitWrite(sc_buffer[(123 + i)/8], (123 + i) % 8, bitRead(mask, i));
}

void SC75823::setSmallDisc(int frame, byte mask)
{
	bitWrite(sc_buffer[16], 7, frame % 2);
	for (int i = 0; i < 6; i++)
		bitWrite(sc_buffer[(134 - i)/8], (134 - i) % 8, bitRead(mask, i));
}

void SC75823::setBigDisc(int frame, byte mask)
{
	bitWrite(sc_buffer[17], 7, frame % 2);
	for (int i = 0; i < 6; i++)
		bitWrite(sc_buffer[(142 - i)/8], (142 - i) % 8, bitRead(mask, i));
}

void SC75823::turnOn(int on, int swMode)
{
	if ((sc_inh < 0) || (swMode > 0))   // no INH connection or user chose soft mode
		{
			bitWrite(sc_buffer[19], 6, 1 - (on % 2));
			bitWrite(sc_buffer[19], 5, 1 - (on % 2));

			writeBuffer(); // do it

			return;
		}
	digitalWrite(sc_inh, on ? HIGH : LOW);
}