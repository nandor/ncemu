// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include "emulator.h"
#include "crc.h"


void emulator_init( emulator_t *emu )
{
  // Clear stuff
  memset( emu, 0, sizeof( emulator_t ) );

  // Create the window 
  SDL_Init( SDL_INIT_EVERYTHING );
  if ( ! (emu->screen = SDL_SetVideoMode( 320, 240, 24, SDL_SWSURFACE ) ) )
  {
    emulator_error( emu, "Cannot create SDL screen" );
  }

  // Initialise the palette
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


void emulator_load_ch16( emulator_t * emu, const char *fn )
{
  struct 
  {
    uint8_t   magic[4];
    uint8_t   pad;
    uint8_t   ver;
    uint32_t  size;
    uint16_t  start;
    uint32_t  crc32;
  } __attribute__ (( packed )) hdr;

  FILE *fin;
  void *rom;
  long file_size;

  if ( ! ( fin = fopen( fn, "rb" ) ) ) 
  {
    emulator_error( emu, "Cannot open ROM '%s'", fn );
  }

  fseek( fin, 0, SEEK_END );
  file_size = ftell( fin );
  fseek( fin, 0, SEEK_SET );

  if ( fread( &hdr, sizeof( hdr ), 1, fin) != 1 ) 
  {
    emulator_error( emu, "Cannot read ROM header '%s'", fn );
  }

  if ( hdr.magic[0] != 'C' || hdr.magic[1] != 'H' || 
       hdr.magic[2] != '1' || hdr.magic[3] != '6' ) 
  {
    emulator_error( emu, "Invalid ROM header '%s'", fn);
  }

  if ( sizeof(hdr) + hdr.size != file_size )
  {
    emulator_error( emu, "Invalid ROM size '%s'", hdr.size );
  }

  if ( !( rom = malloc(hdr.size) ) ||
       fread( rom, sizeof( uint8_t ), hdr.size, fin) != hdr.size )
  {
    emulator_error( emu, "Cannot read ROM '%s'", fn );
  }
  
  if ( hdr.crc32 != crc32( 0, rom, hdr.size ) )
  {
    emulator_error( emu, "Mismatched CRC for ROM '%s'", fn);
  }

  memcpy( &emu->ram, rom, hdr.size );
  emu->cpu.pc = hdr.start;
}


void emulator_blit( emulator_t *emu )
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
      c = c ? c : emu->gpu.bg;
      
      pix[0] = (emu->gpu.pal[ c ] & 0x00ff0000) >> 16;
      pix[1] = (emu->gpu.pal[ c ] & 0x0000ff00) >> 8;
      pix[2] = (emu->gpu.pal[ c ] & 0x000000ff) >> 0;
    }
  }

  if ( SDL_MUSTLOCK( emu->screen ) ) SDL_UnlockSurface( emu->screen );
}


void emulator_run( emulator_t *emu )
{
  int running = 1;
  SDL_Event evt;

  while ( running )
  {
    while ( SDL_PollEvent( &evt ) )
    {
      switch (evt.type)
      {
        case SDL_QUIT:
        {
          running = 0;
          break;
        }
      }
    }

    cpu_tick( emu );
    emulator_blit( emu );
  
    SDL_Flip( emu->screen );
  } 
}


void emulator_error( emulator_t *emu, const char *fmt, ...)
{
  va_list ap;

  va_start( ap, fmt);
  vsnprintf( emu->err_msg, sizeof( emu->err_msg ), fmt, ap );
  va_end( ap );

  longjmp( emu->err_jmp, 0 );
}


void emulator_free( emulator_t *emu )
{
  if ( emu->screen )
  {
    SDL_FreeSurface( emu->screen );
    emu->screen = NULL;
  }

  SDL_Quit( );
}


