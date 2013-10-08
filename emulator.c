// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "emulator.h"
#include "crc.h"


void emulator_init( emulator_t *emu )
{

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

  if ( ! ( fin = fopen( fn, "rb" ) ) ) 
  {
    emulator_error( emu, "Cannot open ROM '%s'", fn );
  }

  if ( fread( &hdr, sizeof( hdr ), 1, fin) != 1 ) 
  {
    emulator_error( emu, "Cannot read ROM header '%s'", fn );
  }

  if ( hdr.magic[0] != 'C' || hdr.magic[1] != 'H' || 
       hdr.magic[2] != '1' || hdr.magic[3] != '6' ) 
  {
    emulator_error( emu, "Invalid ROM header '%s'", fn);
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


void emulator_run( emulator_t *emu )
{
  int i;

  for ( i = 0; i < 10; ++i )
  {
    cpu_tick( emu );
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

}


