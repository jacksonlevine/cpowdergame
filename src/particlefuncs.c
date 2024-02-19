#include "particlefuncs.h"

void defaultFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    unsigned char* theByte = &FOREPIXELS[index];

    if(getColorBits(FOREPIXELS[index - GAMEWIDTH]) == 0) {
        FOREPIXELS[index - GAMEWIDTH] = *theByte;
        *theByte = 0;
    } else {
        if(isOddFrame) {
            if(getColorBits(FOREPIXELS[index - GAMEWIDTH - 1]) == 0) {
                FOREPIXELS[index - GAMEWIDTH - 1] = *theByte;
                *theByte = 0;
            }
        } else {
            if(getColorBits(FOREPIXELS[index - GAMEWIDTH + 1]) == 0) {
                FOREPIXELS[index - GAMEWIDTH + 1] = *theByte;
                *theByte = 0;
            }
        }
    }
}

ParticleFunc PARTICLEFUNCS[16] = {
    defaultFunc,
    defaultFunc,
    defaultFunc,
    defaultFunc,

    defaultFunc,
    defaultFunc,
    defaultFunc,
    defaultFunc,

    defaultFunc,
    defaultFunc,
    defaultFunc,
    defaultFunc,

    defaultFunc,
    defaultFunc,
    defaultFunc,
    defaultFunc
};