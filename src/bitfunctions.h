#ifndef BITFUNCTIONS_H
#define BITFUNCTIONS_H

#include "gamefacts.h"
#include "stdbool.h"

extern unsigned char colorBits;
extern unsigned char oddBit;

unsigned char getColorBits(unsigned char byte);

bool isOddBit(unsigned char byte);

void setOddBit(unsigned char *byte, unsigned char bit);

#endif