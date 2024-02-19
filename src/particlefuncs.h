#ifndef PARTICLEFUNCS_H
#define PARTICLEFUNCS_H

#include "stdbool.h"
#include "gamefacts.h"
#include "bitfunctions.h"
#include "particlefacts.h"
#include "stdio.h"

typedef void (*ParticleFunc)(unsigned char*, int, bool);

extern ParticleFunc PARTICLEFUNCS[16];

void densitySwapFunc(unsigned char* FOREPIXELS, int index, bool isOddFrame);

#endif