// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emulator.h"

int min(int16_t a, int16_t b) { return a < b ? a : b; }
int max(int16_t a, int16_t b) { return a > b ? a : b; }

void op_00_nop( emulator_t *emu, opcode_t *op)
{
}


void op_01_cls( emulator_t *emu, opcode_t *op)
{
  memset( &emu->gpu.vram, 0, sizeof( emu->gpu.vram ) );
}


void op_02_vblnk( emulator_t *emu, opcode_t *op)
{
}


void op_03_bcg( emulator_t *emu, opcode_t *op )
{
}


void op_04_spr( emulator_t *emu, opcode_t *op )
{
  emu->gpu.sprite_w = op->ll;
  emu->gpu.sprite_h = op->hh;
}


void op_05_drw( emulator_t *emu, opcode_t *op )
{
  
}


void op_06_drw( emulator_t *emu, opcode_t *op )
{
  uint8_t * spr;
  int16_t x, y, i, j, si, di;

  if ( ! (spr = &emu->ram[ emu->cpu.r[ op->rz ] ] ) )
  {
    emulator_error( emu, "Invalid sprite index" );
  }

  x = emu->cpu.r[ op->rx ];
  y = emu->cpu.r[ op->ry ];

  for ( i = 0; i < emu->gpu.sprite_h; ++i )
  {
    for ( j = 0; j < emu->gpu.sprite_w; ++j )
    {
      si = i * emu->gpu.sprite_w + j;
      di = ( y + i ) * 320 + ( x + ( j << 1 ) );

      emu->gpu.vram[ di ]    = (spr[ si ] & 0x0F) >> 0;
      emu->gpu.vram[ di + 1] = (spr[ si ] & 0xF0) >> 8;
    }
  }
}


void op_07_rnd( emulator_t *emu, opcode_t *op)
{
  emu->cpu.r[ op->rx ] = random() % ( op->hhll + 1 );
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
  if ( cpu_cond( emu, op->rx ) )
  {
    emu->cpu.pc = op->hhll;
  }
  else
  {
    emu->cpu.pc += 4;
  }
}


void op_13_jme( emulator_t *emu, opcode_t *op )
{
  if ( emu->cpu.r[ op->rx ] == emu->cpu.r[ op->ry ] )
  {
    emu->cpu.pc = op->hhll;
  }
  else
  {
    emu->cpu.pc += 4;
  }
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


void op_40_addi( emulator_t *emu, opcode_t *op )
{
  int16_t t, a, b;

  a = emu->cpu.r[ op->rx ];
  b = op->hhll;

  t = a + b;

  emu->cpu.r[ op->rx ] = t;

  if ( t < 0 ) set_n( &emu->cpu );
  if ( t == 0) set_z( &emu->cpu );
  if ( t >= 0 && a < 0 && b < 0 ) set_o( &emu->cpu );
  if ( t <= 0 && a > 0 && b > 0 ) set_o( &emu->cpu );
  // TODO: set c
}


void op_41_add( emulator_t *emu, opcode_t *op )
{

}


void op_42_add( emulator_t *emu, opcode_t *op )
{

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
  { op_40_addi,   0, 1, 1, 1, 1 },
  { op_41_add,    0, 1, 1, 1, 1 },
  { op_42_add,    0, 1, 1, 1, 1 },
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
