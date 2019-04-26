/*
    Driver for the MaxLinear MxL5007T silicon tuner

    Copyright (C) 2008, 2009 Michael Krufky <mkrufky@linuxtv.org>

	https://git.linuxtv.org//media_tree.git/tree/drivers/media/tuners/mxl5007t.c
	https://git.linuxtv.org//media_tree.git/tree/drivers/media/tuners/mxl5007t.h
  
    Arduino Library porting by OneTransistor <one1transistor@gmail.com>

	https://www.onetransistor.eu/2014/08/mxl5007t-based-radio.html
	

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

#include "mxl5007t.h"

struct reg_pair_t
{
	unsigned char reg;
	unsigned char val;
};

static struct reg_pair_t init_tab[] = {
	{0x02, 0x06},
	{0x03, 0x48},
	{0x05, 0x04},
	{0x06, 0x10},
	{0x2e, 0x15}, /* OVERRIDE */
	{0x30, 0x10}, /* OVERRIDE */
	{0x45, 0x58}, /* OVERRIDE */
	{0x48, 0x19}, /* OVERRIDE */
	{0x52, 0x03}, /* OVERRIDE */
	{0x53, 0x44}, /* OVERRIDE */
	{0x6a, 0x4b}, /* OVERRIDE */
	{0x76, 0x00}, /* OVERRIDE */
	{0x78, 0x18}, /* OVERRIDE */
	{0x7a, 0x17}, /* OVERRIDE */
	{0x85, 0x06}, /* OVERRIDE */
	{0x01, 0x01}, /* TOP_MASTER_ENABLE */
	{0, 0}};

static struct reg_pair_t init_tab_cable[] = {
	{0x02, 0x06},
	{0x03, 0x48},
	{0x05, 0x04},
	{0x06, 0x10},
	{0x09, 0x3f},
	{0x0a, 0x3f},
	{0x0b, 0x3f},
	{0x2e, 0x15}, /* OVERRIDE */
	{0x30, 0x10}, /* OVERRIDE */
	{0x45, 0x58}, /* OVERRIDE */
	{0x48, 0x19}, /* OVERRIDE */
	{0x52, 0x03}, /* OVERRIDE */
	{0x53, 0x44}, /* OVERRIDE */
	{0x6a, 0x4b}, /* OVERRIDE */
	{0x76, 0x00}, /* OVERRIDE */
	{0x78, 0x18}, /* OVERRIDE */
	{0x7a, 0x17}, /* OVERRIDE */
	{0x85, 0x06}, /* OVERRIDE */
	{0x01, 0x01}, /* TOP_MASTER_ENABLE */
	{0, 0}};

static struct reg_pair_t reg_pair_rftune[] = {
	{0x0f, 0x00}, /* abort tune */
	{0x0c, 0x15},
	{0x0d, 0x40},
	{0x0e, 0x0e},
	{0x1f, 0x87}, /* OVERRIDE */
	{0x20, 0x1f}, /* OVERRIDE */
	{0x21, 0x87}, /* OVERRIDE */
	{0x22, 0x1f}, /* OVERRIDE */
	{0x80, 0x01}, /* freq dependent */
	{0x0f, 0x01}, /* start tune */
	{0, 0}};

void MxL5007T::init(unsigned char mode, unsigned char ifFreq, unsigned char inverted, unsigned char loopThrough)
{
	Wire.begin();

	wakeUp();
	reset();

	setModeBits(mode, 0);
	setIfFreqBits(ifFreq, inverted);

	// Following registers depend on XTAL frequency of tuner
	// Read more at: https://www.onetransistor.eu/2014/08/mxl5007t-based-radio.html
	setRegBits(init_tab, 0x03, 0xF0, 0x40);
	setRegBits(init_tab, 0x05, 0x0F, 0x04);

	setRegBits(init_tab, 0x04, 0x01, loopThrough);

	if (mode >= MxL_MODE_CABLE)
	{
		copyRegBits(init_tab, init_tab_cable);
		writeRegs(init_tab_cable);
	}
	else
		writeRegs(init_tab);
}

void MxL5007T::writeReg(unsigned char reg, unsigned char val)
{
	Wire.beginTransmission(MXL_ADDR);
	Wire.write(reg);
	Wire.write(val);
	Wire.endTransmission();
}

unsigned char MxL5007T::readReg(unsigned char reg)
{
	unsigned char val = 0;

	writeReg(0xFB, reg);

	if (Wire.requestFrom(MXL_ADDR, 1) == 1)
		val = Wire.read();

	return val;
}

void MxL5007T::writeRegs(struct reg_pair_t *regPair)
{
	unsigned char i = 0;

	while (regPair[i].reg || regPair[i].val)
	{
		writeReg(regPair[i].reg, regPair[i].val);
		i++;
	}
}

void MxL5007T::reset()
{
	Wire.beginTransmission(MXL_ADDR);
	Wire.write(0xFF);
	Wire.endTransmission();
}

void MxL5007T::sleep()
{
	writeReg(0x01, 0x00);
	writeReg(0x0F, 0x00);
}

void MxL5007T::wakeUp()
{
	writeReg(0x01, 0x01);
}

char *MxL5007T::chipId()
{
	unsigned char val = readReg(0xD9);

	switch (val)
	{
	case MxL_5007_V1_F1:
		return "MxL5007T_v1.f1";
	case MxL_5007_V1_F2:
		return "MxL5007T_v1.f2";
	case MxL_5007_V2_100_F1:
		return "MxL5007T_v2.100.f1";
	case MxL_5007_V2_100_F2:
		return "MxL5007T_v2.100.f2";
	case MxL_5007_V2_200_F1:
		return "MxL5007T_v2.200.f1";
	case MxL_5007_V2_200_F2:
		return "MxL5007T_v2.200.f2";
	case MxL_5007_V4:
		return "MxL5007T_v4";
	default:
		return "Unknown";
	}
}

void MxL5007T::setRegBits(struct reg_pair_t *regPair, unsigned char reg, unsigned char mask, unsigned char val)
{
	unsigned char i = 0;

	while (regPair[i].reg || regPair[i].val)
	{
		if (regPair[i].reg == reg)
		{
			regPair[i].val &= ~mask;
			regPair[i].val |= val;
		}
		i++;
	}
}

void MxL5007T::copyRegBits(struct reg_pair_t *regPair1, struct reg_pair_t *regPair2)
{
	unsigned char i = 0, j = 0;

	while (regPair1[i].reg || regPair1[i].val)
	{
		while (regPair2[j].reg || regPair2[j].val)
		{
			if (regPair1[i].reg != regPair2[j].reg)
			{
				j++;
				continue;
			}
			regPair2[j].val = regPair1[i].val;
			break;
		}
		i++;
	}
}

void MxL5007T::setModeBits(unsigned char mode, signed char ifDiffOutLevel)
{
	switch (mode)
	{
	case MxL_MODE_ATSC:
		setRegBits(init_tab, 0x06, 0x1f, 0x12);
		break;
	case MxL_MODE_DVBT:
		setRegBits(init_tab, 0x06, 0x1f, 0x11);
		break;
	case MxL_MODE_ISDBT:
		setRegBits(init_tab, 0x06, 0x1f, 0x10);
		break;
	case MxL_MODE_CABLE:
		setRegBits(init_tab_cable, 0x09, 0xff, 0xc1);
		setRegBits(init_tab_cable, 0x0a, 0xff, 8 - ifDiffOutLevel);
		setRegBits(init_tab_cable, 0x0b, 0xff, 0x17);
		break;
	default:
		setRegBits(init_tab, 0x06, 0x1f, 0x11); // DVB-T
	}
}

void MxL5007T::setIfFreqBits(unsigned char ifFreq, unsigned char inverted)
{
	unsigned char val = 0;

	switch (ifFreq)
	{
	case MxL_IF_4_MHZ:
		val = 0x00;
		break;
	case MxL_IF_4_5_MHZ:
		val = 0x02;
		break;
	case MxL_IF_4_57_MHZ:
		val = 0x03;
		break;
	case MxL_IF_5_MHZ:
		val = 0x04;
		break;
	case MxL_IF_5_38_MHZ:
		val = 0x05;
		break;
	case MxL_IF_6_MHZ:
		val = 0x06;
		break;
	case MxL_IF_6_28_MHZ:
		val = 0x07;
		break;
	case MxL_IF_9_1915_MHZ:
		val = 0x08;
		break;
	case MxL_IF_35_25_MHZ:
		val = 0x09;
		break;
	case MxL_IF_36_15_MHZ:
		val = 0x0a;
		break;
	case MxL_IF_44_MHZ:
		val = 0x0b;
		break;
	default:
		val = 0x0a; // 36.15 MHz
		return;
	}

	setRegBits(init_tab, 0x02, 0x0F, val);
	setRegBits(init_tab, 0x02, 0x10, inverted ? 0x10 : 0x00);
}

void MxL5007T::setBandwidthBits(unsigned char bw)
{
	unsigned char val;

	switch (bw)
	{
	case MxL_BW_6MHz:
		val = 0x15;
		break;
	case MxL_BW_7MHz:
		val = 0x2A;
		break;
	case MxL_BW_8MHz:
		val = 0x3F;
		break;
	default:
		val = 0x3F;
		return;
	}
	setRegBits(reg_pair_rftune, 0x0C, 0x3F, val);
}

/* RF tuning register set include frequency and bandwidth information */
void MxL5007T::setFrequencyAndBw(unsigned long int rfFreqHz, unsigned char bw)
{
	unsigned long int dig_rf_freq = 0;
	unsigned long int temp;
	unsigned long int frac_divider = 1000000;
	unsigned int i;

	setBandwidthBits(bw);

	dig_rf_freq = rfFreqHz / 1000000;
	temp = rfFreqHz % 1000000;

	for (i = 0; i < 6; i++)
	{
		dig_rf_freq <<= 1;
		frac_divider /= 2;
		if (temp > frac_divider)
		{
			temp -= frac_divider;
			dig_rf_freq++;
		}
	}

	if (temp > 7812)
		dig_rf_freq++;

	setRegBits(reg_pair_rftune, 0x0D, 0xFF, (unsigned int)dig_rf_freq);
	setRegBits(reg_pair_rftune, 0x0E, 0xFF, (unsigned int)(dig_rf_freq >> 8));

	if (rfFreqHz > 333000000)
		setRegBits(reg_pair_rftune, 0x80, 0x40, 0x40);

	writeRegs(reg_pair_rftune);

	delay(3);
}

void MxL5007T::getLockStatus(unsigned char &rfLock, unsigned char &refLock)
{
	unsigned char val;

	rfLock = 0;
	refLock = 0;

	val = readReg(0xD8);

	if ((val & 0x0C) == 0x0C)
		rfLock = 1;
	if ((val & 0x03) == 0x03)
		refLock = 1;
}
