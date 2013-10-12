// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include "emulator.h"
#include "snd.h"


uint16_t snd_att[] = 
{ 
    2, 8, 16, 24, 38, 56, 68, 80, 100, 
    250, 500, 800, 1000, 3000, 5000, 8000 
};


uint16_t snd_dec[] =
{ 
  6, 24, 48, 72, 114, 168, 204, 240, 300, 
  750, 1500, 2400, 3000, 9000, 15000, 24000 
};


uint16_t snd_rel[] = 
{ 
  6, 24, 48, 72, 114, 168, 204, 240, 300, 
  750, 1500, 2400, 3000, 9000, 15000, 24000 
};


int16_t gen_tone( snd_t *snd )
{  

  if ( ( snd->smp_idx / snd->smp_period ) & 1 )
  {
    return -snd->p_vol;
  }
  else
  {
    return snd->p_vol;
  }
}


int16_t gen_adsr( snd_t *snd )
{
  return 0;
}



void snd_callback( void *data, uint8_t *buf, int len)
{
  snd_t *snd;
  int i;
  
  if ( ! (snd = (snd_t*) data) )
  {
    return;
  }
  
  for ( i = 0; i < ( len >> 1 ); ++i )
  {
    if ( snd->smp_idx < snd->smp_count )
    {
      ((int16_t*)buf) [ i ] = (* (snd->p_adsr ? gen_adsr : gen_tone ) ) ( snd );
      ++snd->smp_idx;
    }
    else
    {
      ((int16_t*)buf) [ i ] = 0;
    }
  }
}


void snd_init( emulator_t *emu )
{
  SDL_AudioSpec spec;
  
  emu->snd.p_type = SND_TRIANGLE;
  emu->snd.p_freq = 500;
  emu->snd.p_att = 2;
  emu->snd.p_dec = 6;
  emu->snd.p_rel = 6;
  emu->snd.p_sus = SND_MAX_VOL << 7;
  emu->snd.p_vol = SND_MAX_VOL << 7;
  
  spec.freq = SND_SAMPLE_RATE;
  spec.format = AUDIO_S16SYS;
  spec.channels = 1;
  spec.samples = 4096;
  spec.userdata = (void*) &emu->snd;
  spec.callback = snd_callback;
  
  if ( SDL_OpenAudio( &spec, NULL) < 0 )
  {
    emulator_error( emu, "Cannot initialise sound: %s\n", SDL_GetError( ) );
  }
}


void snd_free( emulator_t *emu )
{
  SDL_LockAudio( );
  SDL_CloseAudio( );
  SDL_UnlockAudio( );
}


void snd_tone( emulator_t *emu, uint16_t freq, uint16_t len, uint8_t adsr )
{
  SDL_PauseAudio( 1 );
  
  emu->snd.p_freq = freq;  
  emu->snd.p_adsr = adsr;
  
  emu->snd.smp_att = SND_SAMPLE_RATE * emu->snd.p_att / 1000;
  emu->snd.smp_dec = SND_SAMPLE_RATE * emu->snd.p_dec / 1000;
  emu->snd.smp_rel = SND_SAMPLE_RATE * emu->snd.p_rel / 1000;
  
  emu->snd.smp_idx = 0;
  emu->snd.smp_count = ( len * SND_SAMPLE_RATE ) / 1000;
  emu->snd.smp_period = SND_SAMPLE_RATE / emu->snd.p_freq;
  emu->snd.smp_total = len * SND_SAMPLE_RATE / 1000;
  
  if ( emu->snd.smp_count < emu->snd.smp_att )
  {
    emu->snd.smp_att = emu->snd.smp_count;
    emu->snd.smp_dec = 0;
  }
  
  if ( emu->snd.smp_count + emu->snd.smp_att > emu->snd.smp_dec )
  {
    emu->snd.smp_dec = emu->snd.smp_count + emu->snd.smp_att;
  }
  
  emu->snd.smp_count += emu->snd.smp_rel;
  emu->snd.smp_sus = emu->snd.smp_count - emu->snd.smp_att - emu->snd.smp_dec;
  
  if ( emu->snd.smp_sus < 0 )
  {
    emu->snd.smp_sus = 0;
  }
  
  SDL_PauseAudio( 0 );
}

void snd_stop( emulator_t *emu )
{
  SDL_PauseAudio( 1 );
}
