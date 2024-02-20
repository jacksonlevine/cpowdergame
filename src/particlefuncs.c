#include "particlefuncs.h"

unsigned char* safeGet(unsigned char *FOREPIXELS, int index) {
    if(index > 0 && index <= GAMEWIDTH * GAMEHEIGHT - 1) {
        return &FOREPIXELS[index];
    } else {
        return &FOREPIXELS[0];
    }
}

void safeSet(unsigned char *FOREPIXELS, int index, unsigned char byte) {
    unsigned char * got = safeGet(FOREPIXELS, index);
    if(got != NULL) {
        *got = byte;
    }
}

//Have your particle call densitySwapFunc at the end if it isn't solid
void densitySwapFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    unsigned char* theByte = safeGet(FOREPIXELS, index);
    if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH)) != 0) {
        if(DENSITIES[getColorBits(*theByte)] > DENSITIES[getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH))])
        {
            unsigned char buf = *safeGet(FOREPIXELS, index - GAMEWIDTH);
            safeSet(FOREPIXELS, index - GAMEWIDTH, *theByte);
            *theByte = buf;
        }
            
    } else {
        if(isOddFrame) {
            if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH - 1)) != 0) {
                if(DENSITIES[getColorBits(*theByte)] > DENSITIES[getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH - 1))])
                {
                    unsigned char buf = FOREPIXELS[index - GAMEWIDTH - 1];
                    FOREPIXELS[index - GAMEWIDTH - 1] = *theByte;
                    *theByte = buf;
                }
            }
        } else {
            if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH + 1)) != 0) {
                if(DENSITIES[getColorBits(*theByte)] > DENSITIES[getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH + 1))])
                {
                    unsigned char buf = *safeGet(FOREPIXELS, index - GAMEWIDTH + 1);
                    safeSet(FOREPIXELS, index - GAMEWIDTH + 1, *theByte);
                    *theByte = buf;
                }
            }
        }
    }
}

//Default "sand" movement
void defaultFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    unsigned char* theByte = safeGet(FOREPIXELS, index);

    if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH)) == 0) {
        safeSet(FOREPIXELS, index - GAMEWIDTH, *theByte);
        *theByte = 0;
    } else {
        if(isOddFrame) {
            if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH - 1)) == 0) {
                safeSet(FOREPIXELS, index - GAMEWIDTH - 1, *theByte);
                *theByte = 0;
            }
        } else {
            if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH + 1)) == 0) {
                safeSet(FOREPIXELS, index - GAMEWIDTH + 1, *theByte);
                *theByte = 0;
            }
        }
    }

    densitySwapFunc(FOREPIXELS, index, isOddFrame);
}

//Liquid movement. This is not a valid particlefunc so wrap it with another to make different types. Higher spread value will 'disperse' quicker.
void genericLiquidFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame, int spread) {
    unsigned char* theByte = safeGet(FOREPIXELS, index);

    if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH)) == 0) {
        safeSet(FOREPIXELS, index - GAMEWIDTH, *theByte);
        *theByte = 0;
    } else {
        
        bool movedDown = false;
        if(isOddFrame) {
            if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH - 1)) == 0) {
                safeSet(FOREPIXELS, index - GAMEWIDTH - 1, *theByte);
                *theByte = 0;
            }
        } else {
            if(getColorBits(*safeGet(FOREPIXELS, index - GAMEWIDTH + 1)) == 0) {
                safeSet(FOREPIXELS, index - GAMEWIDTH + 1, *theByte);
                *theByte = 0;
            }
        }
        if(!movedDown) {
            if(isLiquidTravLeftBit(*theByte)) {
                if(getColorBits(*safeGet(FOREPIXELS, index - 1)) == 0 || getColorBits(*safeGet(FOREPIXELS, index - 1)) == getColorBits(*theByte)) {
                    int howFarCanTravel = 1;
                    while(howFarCanTravel < spread) {
                        howFarCanTravel++;
                        if(getColorBits(*safeGet(FOREPIXELS, index - howFarCanTravel)) != 0 && getColorBits(*safeGet(FOREPIXELS, index - howFarCanTravel)) != getColorBits(*theByte)) {
                            howFarCanTravel--;
                            break;
                        }
                    }
                    unsigned char buf = *safeGet(FOREPIXELS, index - howFarCanTravel);
                    safeSet(FOREPIXELS, index - howFarCanTravel, *theByte);
                    *theByte = buf;
                } else {
                    setLiquidTravLeftBit(theByte, 0);
                }
            } else {
                if(getColorBits(*safeGet(FOREPIXELS, index + 1)) == 0 || getColorBits(*safeGet(FOREPIXELS, index + 1)) == getColorBits(*theByte)) {
                    int howFarCanTravel = 1;
                    while(howFarCanTravel < spread) {
                        howFarCanTravel++;
                        if(getColorBits(*safeGet(FOREPIXELS, index + howFarCanTravel)) != 0 && getColorBits(*safeGet(FOREPIXELS, index + howFarCanTravel)) != getColorBits(*theByte)) {
                            howFarCanTravel--;
                            break;
                        }
                    }
                    unsigned char buf = *safeGet(FOREPIXELS, index + howFarCanTravel);
                    safeSet(FOREPIXELS, index + howFarCanTravel, *theByte);
                    *theByte = buf;
                } else {
                    setLiquidTravLeftBit(theByte, 1);
                }
            }
        }
    }
    densitySwapFunc(FOREPIXELS, index, isOddFrame);
}


//Gas movement. This is not a valid particlefunc so wrap it with another to make different types. Higher spread value will 'disperse' quicker.
void genericGasFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame, int spread) {
    unsigned char* theByte = safeGet(FOREPIXELS, index);

    if(getColorBits(*safeGet(FOREPIXELS, index + GAMEWIDTH)) == 0) {
        safeSet(FOREPIXELS, index + GAMEWIDTH, *theByte);
        *theByte = 0;
        index = index + GAMEWIDTH;
        theByte = safeGet(FOREPIXELS, index);
    } else {
        
        if(isOddFrame) {
            if(getColorBits(*safeGet(FOREPIXELS, index + GAMEWIDTH - 1)) == 0) {
                safeSet(FOREPIXELS, index + GAMEWIDTH - 1, *theByte);
                *theByte = 0;
                index = index + GAMEWIDTH - 1;
                theByte = safeGet(FOREPIXELS, index);
            }
        } else {
            if(getColorBits(*safeGet(FOREPIXELS, index + GAMEWIDTH + 1)) == 0) {
                safeSet(FOREPIXELS, index + GAMEWIDTH + 1, *theByte);
                *theByte = 0;
                index = index + GAMEWIDTH + 1;
                theByte = safeGet(FOREPIXELS, index);
            }
        }

        if(isLiquidTravLeftBit(*theByte)) {
        if(getColorBits(*safeGet(FOREPIXELS, index - 1)) == 0 || getColorBits(*safeGet(FOREPIXELS, index - 1)) == getColorBits(*theByte)) {
            int howFarCanTravel = 1;
            while(howFarCanTravel < spread) {
                howFarCanTravel++;
                if(getColorBits(*safeGet(FOREPIXELS, index - howFarCanTravel)) != 0 && getColorBits(*safeGet(FOREPIXELS, index - howFarCanTravel)) != getColorBits(*theByte)) {
                    howFarCanTravel--;
                    break;
                }
            }
            unsigned char buf = *safeGet(FOREPIXELS, index - howFarCanTravel);
            safeSet(FOREPIXELS, index - howFarCanTravel, *theByte);
            *theByte = buf;
        } else {
            setLiquidTravLeftBit(theByte, 0);
        }
        } else {
            if(getColorBits(*safeGet(FOREPIXELS, index + 1)) == 0 || getColorBits(*safeGet(FOREPIXELS, index + 1)) == getColorBits(*theByte)) {
                int howFarCanTravel = 1;
                while(howFarCanTravel < spread) {
                    howFarCanTravel++;
                    if(getColorBits(*safeGet(FOREPIXELS, index + howFarCanTravel)) != 0 && getColorBits(*safeGet(FOREPIXELS, index + howFarCanTravel)) != getColorBits(*theByte)) {
                        howFarCanTravel--;
                        break;
                    }
                }
                unsigned char buf = *safeGet(FOREPIXELS, index + howFarCanTravel);
                safeSet(FOREPIXELS, index + howFarCanTravel, *theByte);
                *theByte = buf;
            } else {
                setLiquidTravLeftBit(theByte, 1);
            }
        }

    }
    
    densitySwapFunc(FOREPIXELS, index, isOddFrame);
}

//Fast liquid like water
void fastLiquidFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    genericLiquidFunc(FOREPIXELS, index, isOddFrame, 4);
}

//Slow liquid like magma
void slowLiquidFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    genericLiquidFunc(FOREPIXELS, index, isOddFrame, 1);
}

//Solid inert material
void solidFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    //Nothing
}

//Magma/lava
void magmaFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    static int drawPixels[] = {
        1, 0,
        -1, 0,
        0, 1,
        0, -1,
        -1, 1,
        -1, -1,
        1, 1,
        1, -1,
        -2, 0,
        -2, 1,
        -2, -1,
        2, 0,
        2, 1,
        2, -1,
        0, 2,
        1, 2,
        -1, 2,
        0, -2,
        1, -2,
        -1, -2
    };

    for(int i = 0; i < sizeof(drawPixels)/sizeof(int); i+= 2) {
        int ind = index + (drawPixels[i+1]) * GAMEWIDTH + drawPixels[i];
        if(ind > 0 && ind <= GAMEWIDTH*GAMEHEIGHT - 1) {
            
            //Affect surrounding pixels here (Don't affect yourself! Thats for them to do):
            if(getColorBits(*safeGet(FOREPIXELS, ind)) == WATER) {
                safeSet(FOREPIXELS, ind, STEAM);
            }

        }
    }
    slowLiquidFunc(FOREPIXELS, index, isOddFrame);
}

//Water
void waterFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {
    static int drawPixels[] = {
        1, 0,
        -1, 0,
        0, 1,
        0, -1,
        -1, 1,
        -1, -1,
        1, 1,
        1, -1,
        // -2, 0,
        // -2, 1,
        // -2, -1,
        // 2, 0,
        // 2, 1,
        // 2, -1,
        // 0, 2,
        // 1, 2,
        // -1, 2,
        // 0, -2,
        // 1, -2,
        // -1, -2
    };
    for(int i = 0; i < sizeof(drawPixels)/sizeof(int); i+= 2) {
        int ind = index + (drawPixels[i+1]) * GAMEWIDTH + drawPixels[i];
        if(ind > 0 && ind <= GAMEWIDTH*GAMEHEIGHT - 1) {
            
            //Affect surrounding pixels here (Don't affect yourself! Thats for them to do):
            if(getColorBits(*safeGet(FOREPIXELS, ind)) == MAGMA) {
                safeSet(FOREPIXELS, ind, STONE);
            }

        }
    }
    fastLiquidFunc(FOREPIXELS, index, isOddFrame);
}

void steamFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame) {

    static int drawPixels[] = {
        1, 0,
        -1, 0,
        0, 1,
        0, -1,
        -1, 1,
        -1, -1,
        1, 1,
        1, -1
    };
    for(int i = 0; i < sizeof(drawPixels)/sizeof(int); i+= 2) {
        int ind = index + (drawPixels[i+1]) * GAMEWIDTH + drawPixels[i];
        if(ind > 0 && ind <= GAMEWIDTH*GAMEHEIGHT - 1) {
            
            //Affect surrounding pixels here (Don't affect yourself! Thats for them to do):
            if(getColorBits(*safeGet(FOREPIXELS, ind)) == STEAM) {
                safeSet(FOREPIXELS, ind, WATER);
            }

        }
    }

    genericGasFunc(FOREPIXELS, index, isOddFrame, 1);
}

ParticleFunc PARTICLEFUNCS[16] = {
    defaultFunc,
    defaultFunc,
    waterFunc,
    solidFunc,

    solidFunc,
    magmaFunc,
    steamFunc,
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