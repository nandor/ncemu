// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef SND_H
#define SND_H


#include <stdint.h>
#include <SDL/SDL.h>


#define SND_SAMPLE_RATE      44100
#define SND_MAX_VOL          255


typedef struct _emulator_t emulator_t;
typedef struct _snd_t snd_t;

typedef enum _snd_type_t
{
    SND_TRIANGLE,
    SND_SAWTOOTH,
    SND_PULSE,
    SND_NOISE
} snd_type_t;


typedef struct _snd_t
{  
  // Sound generation parameters
  uint8_t p_adsr;
  snd_type_t p_type;
  uint16_t p_freq;
  uint16_t p_att;
  uint16_t p_dec;
  uint16_t p_rel;
  uint16_t p_sus;
  uint16_t p_vol;
  
  // Sound state
  int smp_att;
  int smp_dec;
  int smp_rel;
  int smp_sus;
  int smp_idx;
  int smp_count;
  int smp_total;
  int smp_period;
} snd_t;


extern uint16_t snd_att[];
extern uint16_t snd_dec[];
extern uint16_t snd_rel[];


void snd_init( emulator_t *emu );
void snd_free( emulator_t *emu );
void snd_stop( emulator_t *emu );
void snd_tone( emulator_t *emu, uint16_t freq, uint16_t len, uint8_t adsr );

#endif
