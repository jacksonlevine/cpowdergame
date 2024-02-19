#ifndef PARTICLEFUNCS_H
#define PARTICLEFUNCS_H

#include "stdbool.h"
#include "gamefacts.h"
#include "bitfunctions.h"

typedef void (*ParticleFunc)(unsigned char*, int, bool);

extern ParticleFunc PARTICLEFUNCS[16];

#endif