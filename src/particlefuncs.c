#include "particlefuncs.h"

//Have your particle call densitySwapFunc at the end if it isn't solid
void densitySwapFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    unsigned char* theByte = &FOREPIXELS[index];
    if(getColorBits(FOREPIXELS[index - GAMEWIDTH]) != 0) {
        if(DENSITIES[getColorBits(*theByte)] > DENSITIES[getColorBits(FOREPIXELS[index - GAMEWIDTH])])
        {
            unsigned char buf = FOREPIXELS[index - GAMEWIDTH];
            FOREPIXELS[index - GAMEWIDTH] = *theByte;
            *theByte = buf;
        }
            
    } else {
        if(isOddFrame) {
            if(getColorBits(FOREPIXELS[index - GAMEWIDTH - 1]) != 0) {
                if(DENSITIES[getColorBits(*theByte)] > DENSITIES[getColorBits(FOREPIXELS[index - GAMEWIDTH - 1])])
                {
                    unsigned char buf = FOREPIXELS[index - GAMEWIDTH - 1];
                    FOREPIXELS[index - GAMEWIDTH - 1] = *theByte;
                    *theByte = buf;
                }
            }
        } else {
            if(getColorBits(FOREPIXELS[index - GAMEWIDTH + 1]) != 0) {
                if(DENSITIES[getColorBits(*theByte)] > DENSITIES[getColorBits(FOREPIXELS[index - GAMEWIDTH + 1])])
                {
                    unsigned char buf = FOREPIXELS[index - GAMEWIDTH + 1];
                    FOREPIXELS[index - GAMEWIDTH + 1] = *theByte;
                    *theByte = buf;
                }
            }
        }
    }
}

//Default "sand" movement
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

    densitySwapFunc(FOREPIXELS, index, isOddFrame);
}

//Liquid movement with fast spread
void liquidFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    unsigned char* theByte = &FOREPIXELS[index];

    if(getColorBits(FOREPIXELS[index - GAMEWIDTH]) == 0) {
        FOREPIXELS[index - GAMEWIDTH] = *theByte;
        *theByte = 0;
    } else {
        
        bool movedDown = false;
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
        if(!movedDown) {
            if(isLiquidTravLeftBit(*theByte)) {
                if(getColorBits(FOREPIXELS[index - 1]) == 0 || getColorBits(FOREPIXELS[index - 1]) == getColorBits(*theByte)) {
                    int howFarCanTravel = 1;
                    while(howFarCanTravel < 7) {
                        howFarCanTravel++;
                        if(getColorBits(FOREPIXELS[index - howFarCanTravel]) != 0 && getColorBits(FOREPIXELS[index - howFarCanTravel]) != getColorBits(*theByte)) {
                            howFarCanTravel--;
                            break;
                        }
                    }
                    unsigned char buf = FOREPIXELS[index - howFarCanTravel];
                    FOREPIXELS[index - howFarCanTravel] = *theByte;
                    *theByte = buf;
                } else {
                    setLiquidTravLeftBit(theByte, 0);
                }
            } else {
                if(getColorBits(FOREPIXELS[index + 1]) == 0 || getColorBits(FOREPIXELS[index + 1]) == getColorBits(*theByte)) {
                    int howFarCanTravel = 1;
                    while(howFarCanTravel < 7) {
                        howFarCanTravel++;
                        if(getColorBits(FOREPIXELS[index + howFarCanTravel]) != 0 && getColorBits(FOREPIXELS[index + howFarCanTravel]) != getColorBits(*theByte)) {
                            howFarCanTravel--;
                            break;
                        }
                    }
                    unsigned char buf = FOREPIXELS[index + howFarCanTravel];
                    FOREPIXELS[index + howFarCanTravel] = *theByte;
                    *theByte = buf;
                } else {
                    setLiquidTravLeftBit(theByte, 1);
                }
            }
        }
    }
    densitySwapFunc(FOREPIXELS, index, isOddFrame);
}

//Solid inert material
void solidFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    //Nothing
}

ParticleFunc PARTICLEFUNCS[16] = {
    defaultFunc,
    defaultFunc,
    liquidFunc,
    solidFunc,

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