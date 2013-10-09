// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <string.h>
#include <stdio.h>
#include "emulator.h"
#include "cpu.h"


extern instr_t instr_table[0x100];


void cpu_tick( emulator_t * emu )
{
  opcode_t op;
  instr_t *instr;

  memcpy(&op, emu->ram + emu->cpu.pc, 4);
 
  if ( ! ( instr = &instr_table[ op.op ] ) )
  {
    emulator_error( emu, "Invalid opcode 0x%8x", op.dw );
  }

  printf("%x %8x\n", op.op, op.dw );

  (*(instr->op)) ( emu, &op );

  if ( ! instr->set_pc ) emu->cpu.pc += 4;

  if ( ! instr->set_z ) clear_z( &emu->cpu );
  if ( ! instr->set_o ) clear_o( &emu->cpu );
  if ( ! instr->set_c ) clear_c( &emu->cpu );
  if ( ! instr->set_n ) clear_n( &emu->cpu );
}


