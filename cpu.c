// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <string.h>
#include "emulator.h"
#include "cpu.h"


extern instr_t instr_table[0x100];


void cpu_tick( emulator_t * emu )
{
  opcode_t op;

  memcpy(&op, emu->ram + emu->cpu.pc, 4);
  printf("%d\n", op.op);
}

