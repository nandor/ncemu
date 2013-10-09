// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

int main( int argc, char **argv)
{
  emulator_t emu;

  if ( setjmp( emu.err_jmp ) ) {
    fprintf( stderr, "ERROR: %s\n", emu.err_msg);
    return -1;
  }

  emulator_init( &emu );
  emulator_load_ch16( &emu, "Maze.c16" );
  emulator_run( &emu );
  emulator_free( &emu );
  
  return 0;
}

