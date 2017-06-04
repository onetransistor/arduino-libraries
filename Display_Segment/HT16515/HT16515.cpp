

#include "Arduino.h"
#include "HT16515.h"

HT16515::HT16515(int clockPin, int csPin, int dataWritePin, int dataReadPin) {
	ht_clk = clockPin;
	ht_cs = csPin;
	ht_di = dataWritePin;
	ht_do = dataReadPin;
}

void HT16515::init() {
	pinMode(ht_clk, OUTPUT);
	pinMode(ht_cs, OUTPUT);
	pinMode(ht_di, OUTPUT);
	pinMode(ht_do, INPUT);
	
	digitalWrite(ht_clk, HIGH);
	digitalWrite(ht_cs, HIGH);
}