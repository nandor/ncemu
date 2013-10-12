// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include "emulator.h"
#include "gpu.h"


void gpu_init( emulator_t *emu )
{
  // Clear
  memset( &emu->gpu, 0, sizeof( gpu_t ) );
  
  // Palette
  emu->gpu.pal[0x0] = 0x000000; // Black 
  emu->gpu.pal[0x1] = 0x000000; // Black
  emu->gpu.pal[0x2] = 0x888888; // Gray
  emu->gpu.pal[0x3] = 0xBF3932; // Red
  emu->gpu.pal[0x4] = 0xDE7AAE; // Pink
  emu->gpu.pal[0x5] = 0x4C3D21; // Dark brown
  emu->gpu.pal[0x6] = 0x905F25; // Brown
  emu->gpu.pal[0x7] = 0xE49452; // Orange
  emu->gpu.pal[0x8] = 0xEAD979; // Yello
  emu->gpu.pal[0x9] = 0x537A3B; // Green
  emu->gpu.pal[0xA] = 0xABD54A; // Light green
  emu->gpu.pal[0xB] = 0x252E38; // Dark blue
  emu->gpu.pal[0xC] = 0x00467F; // Blue
  emu->gpu.pal[0xD] = 0x68ABCC; // Light blue
  emu->gpu.pal[0xE] = 0xBCDEE4; // Sky blue
  emu->gpu.pal[0xF] = 0xFFFFFF; // White  
}


void gpu_free( emulator_t *emu )
{
}


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
      
      
      if ( j & 1 )
      {
        if ( spr[ si ] & 0x0F )
        {
          if ( emu->gpu.vram[ di ] ) set_c( &emu->cpu );
          emu->gpu.vram[ di ] = ( spr[ si ] & 0x0F ) >> 0;
        }
      }
      else
      {
        if ( spr[ si ] & 0xF0 )
        {
          if ( emu->gpu.vram[ di ] ) set_c( &emu->cpu );
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
    emu->gpu.pal[i] = (uint32_t) ( pal[ i * 3 + 0 ] << 16 ) | 
                      (uint32_t) ( pal[ i * 3 + 1 ] <<  8 ) | 
                      (uint32_t) ( pal[ i * 3 + 2 ] <<  0 );
                      
  }
}
