// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include "emulator.h"
#include "gpu.h"


void gpu_draw_sprite( emulator_t *emu, uint8_t *spr, int16_t x, int16_t y )
{
  int16_t i, j, hit;
  uint32_t si, di;

  x = emu->cpu.r[ x ];
  y = emu->cpu.r[ y ];

  clear_c( &emu->cpu );
  for ( i = max( 0, -y ); i < min( emu->gpu.sprite_h, 240 - y ); ++i )
  {
    for ( j = max( 0, -x ); j < min( emu->gpu.sprite_w << 1, 320 - x ); ++j )
    {
      si = i * emu->gpu.sprite_w + ( j >> 1 );
      if ( emu->gpu.vflip )
      {
        di = ( y + emu->gpu.sprite_h - i ) * 320;
      }
      else
      {
        di = ( y + i) * 320;
      }
      
      if ( emu->gpu.hflip )
      {
        di += x + (emu->gpu.sprite_w << 1) - j;
      }
      else
      {
        di += x + j;
      }
      
      if ( emu->gpu.vram[ di ] ) set_c( &emu->cpu );
      
      if ( j & 1 )
      {
        if ( spr[ si ] & 0x0F )
        {
          emu->gpu.vram[ di ] = ( spr[ si ] & 0x0F ) >> 0;
        }
      }
      else
      {
        if ( spr[ si ] & 0xF0 )
        {
          emu->gpu.vram[ di ] = ( spr[ si ] & 0xF0 ) >> 4;
        }
      }
    }
  } 
}


void gpu_blit( emulator_t *emu )
{
  int i, j, idx;
  uint8_t *s, *pix, c;

  if ( SDL_MUSTLOCK( emu->screen ) ) SDL_LockSurface( emu->screen );

  if ( ! (s = (uint8_t*)emu->screen->pixels ) )
  {
    emulator_error( emu, "Cannot access surface" );
  }

  for ( i = 0; i < 240; ++i )
  {
    for ( j = 0; j < 320; ++j )
    {
      pix = s + i * emu->screen->pitch + j * emu->screen->format->BytesPerPixel;
      idx = i * 320 + j;

      c = emu->gpu.vram[ idx ];
      c = c ? c : emu->gpu.bgc;
      
      pix[2] = (emu->gpu.pal[ c ] & 0x00ff0000) >> 16;
      pix[1] = (emu->gpu.pal[ c ] & 0x0000ff00) >> 8;
      pix[0] = (emu->gpu.pal[ c ] & 0x000000ff) >> 0;
    }
  }

  if ( SDL_MUSTLOCK( emu->screen ) ) SDL_UnlockSurface( emu->screen );
  
  emu->gpu.vblank = 1;
}

void gpu_load_pal( emulator_t *emu, uint8_t *pal)
{
  int i;
  
  for ( i = 0x0; i <= 0xF; ++i )
  {
    emu->gpu.pal[i] = ( pal[ i * 3 + 0 ] << 16 ) | 
                      ( pal[ i * 3 + 1 ] <<  8 ) | 
                      ( pal[ i * 3 + 2 ] <<  0 );
                      
  }
}
