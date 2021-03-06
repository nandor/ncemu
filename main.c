// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

int main( int argc, char **argv)
{
  if ( argc != 2 )
  {
    fprintf( stderr, "Usage: ncemu [rom]\n" );
    return 0;
  }
  
  emulator_t emu;

  emulator_init( &emu );
  emulator_load_ch16( &emu, argv[1] );
  emulator_run( &emu );
  emulator_free( &emu );
  
  return 0;
}

