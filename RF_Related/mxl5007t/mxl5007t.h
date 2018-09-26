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

#ifndef MXL5007T_H
#define MXL5007T_H

#include <Arduino.h>
#include <Wire.h>

#define MXL_ADDR (0xC0 >> 1)

/* Chip versions */
#define MxL_UNKNOWN_ID 0x00
#define MxL_5007_V1_F1 0x11
#define MxL_5007_V1_F2 0x12
#define MxL_5007_V4 0x14
#define MxL_5007_V2_100_F1 0x21
#define MxL_5007_V2_100_F2 0x22
#define MxL_5007_V2_200_F1 0x23
#define MxL_5007_V2_200_F2 0x24

/* Mode */
#define MxL_MODE_ISDBT 0
#define MxL_MODE_DVBT 1
#define MxL_MODE_ATSC 2
#define MxL_MODE_CABLE 0x10

/* Intermediate frequency output */
#define MxL_IF_4_MHZ 0		/*  4000000 */
#define MxL_IF_4_5_MHZ 1	/*  4500000 */
#define MxL_IF_4_57_MHZ 2   /*  4570000 */
#define MxL_IF_5_MHZ 3		/*  5000000 */
#define MxL_IF_5_38_MHZ 4   /*  5380000 */
#define MxL_IF_6_MHZ 5		/*  6000000 */
#define MxL_IF_6_28_MHZ 6   /*  6280000 */
#define MxL_IF_9_1915_MHZ 7 /*  9191500 */
#define MxL_IF_35_25_MHZ 8  /* 35250000 */
#define MxL_IF_36_15_MHZ 9  /* 36150000 */
#define MxL_IF_44_MHZ 10	/* 44000000 */

/* Filter bandwidth */
#define MxL_BW_6MHz 6
#define MxL_BW_7MHz 7
#define MxL_BW_8MHz 8

class MxL5007T
{
  public:
	void init(unsigned char mode = 1, unsigned char ifFreq = 9, unsigned char inverted = 0, unsigned char loopThrough = 0);
	void reset();
	void sleep();
	void wakeUp();
	char *chipId();
	void setFrequencyAndBw(unsigned long int rfFreqHz, unsigned char bw);

	void getLockStatus(unsigned char &rfLock, unsigned char &refLock);

  private:
	void writeReg(unsigned char reg, unsigned char val);
	unsigned char readReg(unsigned char reg);
	void writeRegs(struct reg_pair_t *reg_pair);
	static void setRegBits(struct reg_pair_t *regPair, unsigned char reg, unsigned char mask, unsigned char val);
	static void copyRegBits(struct reg_pair_t *regPair1, struct reg_pair_t *regPair2);
	static void setModeBits(unsigned char mode, signed char ifDiffOutLevel = 0);
	static void setIfFreqBits(unsigned char ifFreq, unsigned char inverted = 0);
	static void setBandwidthBits(unsigned char bw);
};

#endif