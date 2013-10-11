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
  emu->cpu.sp = 0xFDF0;

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

  // Initialise the controller
  emu->km[0x0].key = SDLK_w;        emu->km[0x0].loc = 0xFFF0; emu->km[0x0].bit = 0; 
  emu->km[0x1].key = SDLK_s;        emu->km[0x1].loc = 0xFFF0; emu->km[0x1].bit = 1; 
  emu->km[0x2].key = SDLK_a;        emu->km[0x2].loc = 0xFFF0; emu->km[0x2].bit = 2; 
  emu->km[0x3].key = SDLK_d;        emu->km[0x3].loc = 0xFFF0; emu->km[0x3].bit = 3; 
  emu->km[0x4].key = SDLK_q;        emu->km[0x4].loc = 0xFFF0; emu->km[0x4].bit = 4; 
  emu->km[0x5].key = SDLK_e;        emu->km[0x5].loc = 0xFFF0; emu->km[0x5].bit = 5; 
  emu->km[0x6].key = SDLK_1;        emu->km[0x6].loc = 0xFFF0; emu->km[0x6].bit = 6; 
  emu->km[0x7].key = SDLK_2;        emu->km[0x7].loc = 0xFFF0; emu->km[0x7].bit = 7;

  emu->km[0x8].key = SDLK_UP;       emu->km[0x8].loc = 0xFFF2; emu->km[0x8].bit = 0; 
  emu->km[0x9].key = SDLK_DOWN;     emu->km[0x9].loc = 0xFFF2; emu->km[0x9].bit = 1; 
  emu->km[0xA].key = SDLK_LEFT;     emu->km[0xA].loc = 0xFFF2; emu->km[0xA].bit = 2; 
  emu->km[0xB].key = SDLK_RIGHT;    emu->km[0xB].loc = 0xFFF2; emu->km[0xB].bit = 3; 
  emu->km[0xC].key = SDLK_PAGEUP;   emu->km[0xC].loc = 0xFFF2; emu->km[0xC].bit = 4; 
  emu->km[0xD].key = SDLK_PAGEDOWN; emu->km[0xD].loc = 0xFFF2; emu->km[0xD].bit = 5; 
  emu->km[0xE].key = SDLK_HOME;     emu->km[0xE].loc = 0xFFF2; emu->km[0xE].bit = 6; 
  emu->km[0xF].key = SDLK_END;      emu->km[0xF].loc = 0xFFF2; emu->km[0xF].bit = 7;  
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
  emu->cpu.pc = hdr.start - 4;
}


void emulator_run( emulator_t *emu )
{
  int running = 1, i;
  uint16_t *port;
  SDL_Event evt;
  Uint8 *key_state;

  while ( running )
  {
    // Process SDL events
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
    
    for (i = 0; i < 16667; ++i)
    {
      cpu_tick( emu );
    }

    // Retrieve controller states
    key_state = SDL_GetKeyState( NULL );
    memset( &emu->ram[ 0xFFF0 ], 0, 4 );
    
    for ( i = 0; i < 16; ++i )
    {
      if ( emu->km[ i ].key )
      {
        port = ( uint16_t* )&emu->ram[ emu->km[ i ].loc ];
        if ( key_state[ emu->km[ i ].key ] )
        {
          *port |= ( 1 << emu->km[ i ].bit );
        }
        else
        {
          *port &= ~( 1 << emu->km[ i ].bit );
        }
      }
    }

    gpu_blit( emu );   

    SDL_Delay( 15.0 );
    SDL_Flip( emu->screen );
  } 
}


void emulator_error( emulator_t *emu, const char *fmt, ...)
{
  va_list ap;

  va_start( ap, fmt);
  vfprintf( stderr, fmt, ap );
  fputc( '\n', stderr );
  va_end( ap );

  emulator_free( emu );
  exit( -1 );
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


void emulator_dump( emulator_t *emu )
{
  printf( "PC:  0x%04x   SP: 0x%04x\n", emu->cpu.pc, emu->cpu.sp);
  printf( "SPR: 0x%04x   BG: 0x%04x\n", emu->gpu.sprw, emu->gpu.bgc);
  printf( "FLAG: C=%d Z=%d 0=%d N=%d\n", get_c( &emu->cpu ), 
                                        get_z( &emu->cpu ), 
                                        get_o( &emu->cpu ), 
                                        get_n ( &emu->cpu ) );
  printf( "R0:  %06d     R8: %06d\n", emu->cpu.r[ 0x0 ], emu->cpu.r[ 0x8 ] );
  printf( "R1:  %06d     R9: %06d\n", emu->cpu.r[ 0x1 ], emu->cpu.r[ 0x9 ] );
  printf( "R2:  %06d     RA: %06d\n", emu->cpu.r[ 0x2 ], emu->cpu.r[ 0xA ] );
  printf( "R3:  %06d     RB: %06d\n", emu->cpu.r[ 0x3 ], emu->cpu.r[ 0xB ] );
  printf( "R4:  %06d     RC: %06d\n", emu->cpu.r[ 0x4 ], emu->cpu.r[ 0xC ] );
  printf( "R5:  %06d     RD: %06d\n", emu->cpu.r[ 0x5 ], emu->cpu.r[ 0xD ] );
  printf( "R6:  %06d     RE: %06d\n", emu->cpu.r[ 0x6 ], emu->cpu.r[ 0xE ] );
  printf( "R7:  %06d     RF: %06d\n", emu->cpu.r[ 0x7 ], emu->cpu.r[ 0xF ] );
}
