// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <setjmp.h>
#include "cpu.h"
#include "gpu.h"

typedef struct _emulator_t
{
  cpu_t   cpu;
  gpu_t   gpu;

  union
  {
    uint8_t ram[0x10000];
    struct
    {
      uint8_t   rom[0xFDF0];
      uint8_t   stack[0x200];
      uint32_t  io;
    };
  };

  jmp_buf err_jmp;
  char    err_msg[8192];
} emulator_t;


void emulator_init( emulator_t *emu );
void emulator_load_ch16( emulator_t * emu, const char *fn );
void emulator_run( emulator_t *emu );
void emulator_error( emulator_t *emu, const char *fmt, ...);
void emulator_free( emulator_t *emu );

#endif

