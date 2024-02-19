#include "bitfunctions.h"

unsigned char colorBits = 0b00001111;
unsigned char oddBit = 0b00010000;

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