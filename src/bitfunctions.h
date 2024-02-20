#ifndef BITFUNCTIONS_H
#define BITFUNCTIONS_H

#include "gamefacts.h"
#include "stdbool.h"

extern unsigned char colorBits;
extern unsigned char oddBit;
extern unsigned char liquidTravLeftBit;
extern unsigned char wetnessBits; //only for wet dirt

unsigned char getColorBits(unsigned char byte);

bool isOddBit(unsigned char byte);

void setOddBit(unsigned char *byte, unsigned char bit);

bool isLiquidTravLeftBit(unsigned char byte);

void setLiquidTravLeftBit(unsigned char *byte, unsigned char bit);

unsigned char getWetness(unsigned char byte);
void setWetness(unsigned char *byte, unsigned char wetness);

#endif