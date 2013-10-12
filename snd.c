// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include "emulator.h"
#include "snd.h"


uint16_t attack[]  = { 2, 8, 16, 24, 38, 56, 68, 80, 100, 250, 500, 800, 1000, 3000, 5000, 8000 };
uint16_t decay[]   = { 6, 24, 48, 72, 114, 168, 204, 240, 300, 750, 1500, 2400, 3000, 9000, 15000, 24000 };
uint16_t release[] = { 6, 24, 48, 72, 114, 168, 204, 240, 300, 750, 1500, 2400, 3000, 9000, 15000, 24000 };

void snd_callback( void *data, uint8_t *buf, int len)
{

}


void snd_init( emulator_t *emu )
{
  SDL_AudioSpec spec;
  
  spec.freq     = 44100;
  spec.format   = AUDIO_S16SYS;
  spec.channels = 1;
  spec.samples  = 4096;
  spec.userdata = (void*) emu;
  spec.callback = snd_callback;
  
  if ( SDL_OpenAudio( &spec, NULL) < 0 )
  {
    emulator_error( emu, "Cannot initialise sound: %s\n", SDL_GetError( ) );
  }
}


void snd_free( emulator_t *emu )
{
  SDL_CloseAudio( );
}


void snd_tone( emulator_t *emu, uint16_t freq, uint16_t len )
{

}

void snd_stop( emulator_t *emu )
{

}
