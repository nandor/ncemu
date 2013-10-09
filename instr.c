// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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


void op_04_spr( emulator_t *emu, opcode_t *op )
{

}


void op_05_drw( emulator_t *emu, opcode_t *op )
{
  
}


void op_06_drw( emulator_t *emu, opcode_t *op )
{

}


void op_07_rnd( emulator_t *emu, opcode_t *op)
{

}


void op_08_flip( emulator_t *emu, opcode_t *op)
{

}


void op_09_snd0( emulator_t *emu, opcode_t *op )
{

}


void op_0A_snd1( emulator_t *emu, opcode_t *op )
{

}


void op_0B_snd2( emulator_t *emu, opcode_t *op )
{

}


void op_0C_snd3( emulator_t *emu, opcode_t *op )
{

}


void op_0E_sng( emulator_t *emu, opcode_t *op)
{

}


void op_0D_snp( emulator_t *emu, opcode_t *op )
{

}


void op_10_jmp( emulator_t *emu, opcode_t *op ) 
{
  emu->cpu.pc = op->hhll;
}


void op_12_jx( emulator_t *emu, opcode_t *op )
{

}


void op_13_jme( emulator_t *emu, opcode_t *op )
{

}


void op_14_call( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ emu->cpu.sp ], &emu->cpu.pc, 2);
  emu->cpu.sp += 2;
  emu->cpu.pc = op->hhll;
}


void op_15_ret( emulator_t *emu, opcode_t *op )
{
  
}


void op_16_jmp( emulator_t *emu, opcode_t *op )
{

}


void op_17_cx( emulator_t *emu, opcode_t *op)
{

}


void op_18_call( emulator_t *emu, opcode_t *op)
{

}


void op_20_ldi( emulator_t *emu, opcode_t *op )
{
  emu->cpu.r[ op->rx ] = (int16_t) op->hhll;
}


void op_21_ldi( emulator_t *emu, opcode_t *op )
{
  emu->cpu.sp = op->hhll;
}


void op_22_ldm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->cpu.r[ op-> rx ], &emu->ram[ op->hhll ], 2 );
}


void op_23_ldm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->cpu.r[ op->rx ], &emu->ram[ emu->cpu.r[ op-> ry ] ], 2 );
}


void op_24_mov( emulator_t *emu, opcode_t *op )
{
  emu->cpu.r[ op->rx ] = emu->cpu.r[ op->ry ];
}


void op_30_stm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ op->hhll ], &emu->cpu.r[ op->rx ], 2 );
}


void op_31_stm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ emu->cpu.r[ op->ry ] ], &emu->cpu.r[ op->rx ], 2 );
}


void op_60_andi( emulator_t *emu, opcode_t *op )
{

}


void op_61_and( emulator_t *emu, opcode_t *op )
{

}


void op_62_and( emulator_t *emu, opcode_t *op )
{

}


void op_63_tsti( emulator_t *emu, opcode_t *op )
{
  int16_t tmp;

  tmp = emu->cpu.r[ op->rx ] & op->hhll;
  
  if (tmp == 0) set_z( &emu->cpu );
  if (tmp & 0x80) set_n( &emu->cpu );
}


void op_64_tst( emulator_t *emu, opcode_t *op )
{

}


instr_t instr_table[0x100] =
{
  // 0x - Misc/Video/Audio
  { op_00_nop,    0, 0, 0, 0, 0 },
  { op_01_cls,    0, 0, 0, 0, 0 },
  { op_02_vblnk,  0, 0, 0, 0, 0 },
  { op_03_bcg,    0, 0, 0, 0, 0 },
  { op_04_spr,    0, 0, 0, 0, 0 },
  { op_05_drw,    0, 0, 0, 0, 1 },
  { op_06_drw,    0, 0, 0, 0, 1 },
  { op_07_rnd,    0, 0, 0, 0, 0 },
  { op_08_flip,   0, 0, 0, 0, 0 },
  { op_09_snd0,   0, 0, 0, 0, 0 },
  { op_0A_snd1,   0, 0, 0, 0, 0 },
  { op_0B_snd2,   0, 0, 0, 0, 0 },
  { op_0C_snd3,   0, 0, 0, 0, 0 },
  { op_0D_snp,    0, 0, 0, 0, 0 },
  { op_0E_sng,    0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },

  // 1x - Jumps (Branches)
  { op_10_jmp,    1, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { op_12_jx,     1, 0, 0, 0, 0 },
  { op_13_jme,    1, 0, 0, 0, 0 },
  { op_14_call,   1, 0, 0, 0, 0 },
  { op_15_ret,    1, 0, 0, 0, 0 },
  { op_16_jmp,    1, 0, 0, 0, 0 },
  { op_17_cx,     1, 0, 0, 0, 0 },
  { op_18_call,   1, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },

  // 2x - Loads
  { op_20_ldi,    0, 0, 0, 0, 0 },
  { op_21_ldi,    0, 0, 0, 0, 0 },
  { op_22_ldm,    0, 0, 0, 0, 0 },
  { op_23_ldm,    0, 0, 0, 0, 0 },
  { op_24_mov,    0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  
  // 3x - Stores
  { op_30_stm,    0, 0, 0, 0, 0 },
  { op_31_stm,    0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  
  // 4x - Addition
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  
  // 5x - Subtraction
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  
  // 6x - Bitwise AND
  { op_60_andi,   0, 1, 0, 1, 0 },
  { op_61_and,    0, 1, 0, 1, 0 },
  { op_62_and,    0, 1, 0, 1, 0 },
  { op_63_tsti,   0, 1, 0, 1, 0 },
  { op_64_tst,    0, 1, 0, 1, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  { NULL,         0, 0, 0, 0, 0 },
  
  // 7x - Bitwise OR
  // 8x - Bitwise XOR
  // 9x - Multiplication
  // Ax - Division
  // Bx - Logical / Arithmetic Shifts
  // Cx - Push / Pop
  // Dx - Palette
};
