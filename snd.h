// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef SND_H
#define SND_H


#include <stdint.h>
#include <SDL/SDL.h>


typedef struct _emulator_t emulator_t;


typedef struct _snd_t
{
  uint8_t att;
  uint8_t dec;
  uint8_t vol;
  uint8_t type;
  uint8_t sus;
  uint8_t rls;
} snd_t;


void snd_init( emulator_t *emu );
void snd_free( emulator_t *emu );
void snd_stop( emulator_t *emu );
void snd_tone( emulator_t *emu, uint16_t freq, uint16_t len );

#endif
