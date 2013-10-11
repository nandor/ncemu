// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include "emulator.h"
#include "gpu.h"


void gpu_draw_sprite( emulator_t *emu, uint8_t *spr, int16_t x, int16_t y )
{
  int16_t i, j;
  uint32_t si, di;

  x = emu->cpu.r[ x ];
  y = emu->cpu.r[ y ];

  for ( i = max( 0, -y ); i < min( emu->gpu.sprite_h, 240 - y ); ++i )
  {
    for ( j = max( 0, -x ); j < min( emu->gpu.sprite_w << 1, 320 - x ); ++j )
    {
      si = i * emu->gpu.sprite_w + ( j >> 1 );
      di = ( y + i ) * 320 + x + j;

      if ( j & 1 )
      {
        emu->gpu.vram[ di ] = ( spr[ si ] & 0x0F ) >> 0;
      }
      else
      {
        emu->gpu.vram[ di ] = ( spr[ si ] & 0xF0 ) >> 4;
      }
    }
  } 
}

