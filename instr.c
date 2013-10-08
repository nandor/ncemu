// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include "emulator.h"

void op_00_nop( emulator_t *emu, opcode_t *op)
{
}


void op_01_cls( emulator_t *emu, opcode_t *op)
{

}


void op_02_vblnk( emulator_t *emu, opcode_t *op)
{

}


void op_03_bcg( emulator_t *emu, opcode_t *op )
{

}


void op_10_jmp( emulator_t *emu, opcode_t *op ) 
{

}


void op_20_ldi( emulator_t *emu, opcode_t *op )
{

}


void op_21_ldi( emulator_t *emu, opcode_t *op )
{

}


void op_30_stm( emulator_t *emu, opcode_t *op )
{

}


instr_t instr_table[0x100] =
{
  // 0x - Misc/Video/Audio
  op_00_nop,
  op_01_cls,
  op_02_vblnk,
  
  // 1x - Jumps (Branches)
  
};
