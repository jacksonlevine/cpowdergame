#include "bitfunctions.h"

unsigned char colorBits = 0b00001111;
unsigned char oddBit = 0b00010000;
unsigned char liquidTravLeftBit = 0b00100000;
unsigned char wetnessBits = 0b11100000;

unsigned char getColorBits(unsigned char byte) {
    return (byte & colorBits);
}

bool isOddBit(unsigned char byte) {
    return (byte & oddBit);
}

void setOddBit(unsigned char *byte, unsigned char bit) {
    *byte &= ~oddBit;
    *byte |= (bit ? oddBit : 0);
}

bool isLiquidTravLeftBit(unsigned char byte) {
    return (byte & liquidTravLeftBit);
}

void setLiquidTravLeftBit(unsigned char *byte, unsigned char bit) {
    *byte &= ~liquidTravLeftBit;
    *byte |= (bit ? liquidTravLeftBit : 0);
}

unsigned char getWetness(unsigned char byte) {
    return ((byte & wetnessBits) >> 5);
}

void setWetness(unsigned char *byte, unsigned char wetness) {
    *byte &= ~wetnessBits;
    *byte |= (wetness << 5);
}