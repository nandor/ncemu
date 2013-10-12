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
  if ( ! emu->gpu.vblank )
  {
    emu->cpu.pc -= 4;
    return;
  }
  
  emu->gpu.vblank = 0;
}


void op_03_bcg( emulator_t *emu, opcode_t *op )
{
  emu->gpu.bgc = op->rz;
}


void op_04_spr( emulator_t *emu, opcode_t *op )
{
  emu->gpu.sprite_w = op->ll;
  emu->gpu.sprite_h = op->hh;
}


void op_05_drw( emulator_t *emu, opcode_t *op )
{
  gpu_draw_sprite( emu, &emu->ram[ (uint16_t)op->hhll ], op->rx, op->ry );
}


void op_06_drw( emulator_t *emu, opcode_t *op )
{
  gpu_draw_sprite( emu, &emu->ram[ emu->cpu.r[ op->rz ] ], op->rx, op->ry );
}


void op_07_rnd( emulator_t *emu, opcode_t *op)
{
  emu->cpu.r[ op->rx ] = random() % ( (uint16_t)op->hhll + 1 );
}


void op_08_flip( emulator_t *emu, opcode_t *op)
{
  switch ( op->hh )
  {
    case 0x0: emu->gpu.hflip = 0; emu->gpu.vflip = 0; break;
    case 0x1: emu->gpu.hflip = 0; emu->gpu.vflip = 1; break;
    case 0x2: emu->gpu.hflip = 1; emu->gpu.vflip = 0; break;
    case 0x3: emu->gpu.hflip = 1; emu->gpu.vflip = 1; break;
    default: emulator_error( emu, "Invalid flip: 0x%02x", op->hh );
  }
}


void op_09_snd0( emulator_t *emu, opcode_t *op )
{
  snd_stop( emu );
}


void op_0A_snd1( emulator_t *emu, opcode_t *op )
{
  snd_tone( emu, 500, op->hhll, 0 );
}


void op_0B_snd2( emulator_t *emu, opcode_t *op )
{
  snd_tone( emu, 1000, op->hhll, 0 );
}


void op_0C_snd3( emulator_t *emu, opcode_t *op )
{
  snd_tone( emu, 1500, op->hhll, 0 );
}


void op_0E_sng( emulator_t *emu, opcode_t *op)
{
  snd_tone( emu, emu->cpu.r[ op->rx ], op->hhll, 1 );
}


void op_0D_snp( emulator_t *emu, opcode_t *op )
{
  snd_t *snd = &emu->snd;
  
  snd->p_type = ( op->hhll & 0x00F0 ) >> 4;
  snd->p_att = snd_att[ ( op->ad & 0xF0 ) >> 4 ];
  snd->p_dec = snd_dec[ ( op->ad & 0x0F ) >> 0 ];
  snd->p_rel = snd_rel[ ( op->hhll & 0xF000 ) >> 12 ];
  snd->p_vol = SND_MAX_VOL / ( 2 * ( 16 - ( ( op->hhll & 0x000F ) >> 0 ) ) );
  snd->p_sus = SND_MAX_VOL / ( 2 * ( 16 - ( ( op->hhll & 0x0F00 ) >> 8 ) ) );
}


void op_10_jmp( emulator_t *emu, opcode_t *op ) 
{
  emu->cpu.pc = (uint16_t) op->hhll;
}


void op_12_jx( emulator_t *emu, opcode_t *op )
{
  if ( cpu_cond( emu, op->rx ) )
  {
    emu->cpu.pc = (uint16_t) op->hhll;
  }
}


void op_13_jme( emulator_t *emu, opcode_t *op )
{
  if ( emu->cpu.r[ op->rx ] == emu->cpu.r[ op->ry ] )
  {
    emu->cpu.pc = (uint16_t) op->hhll;
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
  emu->cpu.sp -= 2;
  memcpy( &emu->cpu.pc, &emu->ram[ emu->cpu.sp ], 2 );
}


void op_16_jmp( emulator_t *emu, opcode_t *op )
{
  emu->cpu.pc = emu->cpu.r[ op->rx ];
}


void op_17_cx( emulator_t *emu, opcode_t *op)
{
  if ( cpu_cond( emu, op->rx ) )
  {
    memcpy( &emu->ram[ emu->cpu.sp ], &emu->cpu.pc, 2 );
    emu->cpu.sp += 2;
    emu->cpu.pc = op->hhll;
  }
}


void op_18_call( emulator_t *emu, opcode_t *op)
{
  memcpy( &emu->ram[ emu->cpu.sp ], &emu->cpu.pc, 2 );
  emu->cpu.sp += 2;
  emu->cpu.pc = emu->cpu.r[ op->rx ];
}


void op_20_ldi( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->cpu.r[ op->rx ], &op->hhll, 2 );
  emu->cpu.r[ op->rx ] = (int16_t) op->hhll;
}


void op_21_ldi( emulator_t *emu, opcode_t *op )
{
  emu->cpu.sp = (uint16_t) op->hhll;
}


void op_22_ldm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->cpu.r[ op->rx ], &emu->ram[ (uint16_t)op->hhll ], 2 );
}


void op_23_ldm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->cpu.r[ op->rx ], &emu->ram[ (uint16_t)emu->cpu.r[ op->ry ] ], 2 );
}


void op_24_mov( emulator_t *emu, opcode_t *op )
{
  emu->cpu.r[ op->rx ] = emu->cpu.r[ op->ry ];
}


void op_30_stm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ (uint16_t) op->hhll ], &emu->cpu.r[ op->rx ], 2 );
}


void op_31_stm( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ (uint16_t)emu->cpu.r[ op->ry ] ], &emu->cpu.r[ op->rx ], 2 );
}


void op_40_addi( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_add( emu, *rx, op->hhll );
  *rx += op->hhll;
}

void op_41_add( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_add( emu, *rx, ry );
  *rx += ry;
}


void op_42_add( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rz = &emu->cpu.r[ op->rz ];
  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_add( emu, rx, ry );  
  *rz = rx + ry;
}


void op_50_subi( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_sub( emu, *rx, op->hhll );
  *rx -= op->hhll;
}


void op_51_sub( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_sub( emu, *rx, ry );
  *rx -= ry;
}


void op_52_sub( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rz = &emu->cpu.r[ op->rz ];
  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_sub( emu, rx, ry );  
  *rz = rx - ry;
}

void op_53_cmpi( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_sub( emu, *rx, op->hhll );
}


void op_54_cmp( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_sub( emu, *rx, ry );
}


void op_60_andi( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;
  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx &= op->hhll );
}


void op_61_and( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];

  cpu_flags_bit( emu, *rx &= ry );
}


void op_62_and( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  rz = &emu->cpu.r[ op->rz ];

  cpu_flags_bit( emu, *rz = rx & ry );
}


void op_63_tsti( emulator_t *emu, opcode_t *op )
{
  cpu_flags_bit( emu, emu->cpu.r[ op->rx ] & op->hhll );
}


void op_64_tst( emulator_t *emu, opcode_t *op )
{
  cpu_flags_bit( emu, emu->cpu.r[ op->rx ] & emu->cpu.r[ op->ry ] );
}

void op_70_ori( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx |= op->hhll );
}


void op_71_or( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];

  cpu_flags_bit( emu, *rx |= ry );
}


void op_72_or( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  rz = &emu->cpu.r[ op->rz ];

  cpu_flags_bit( emu, *rz = rx | ry );
}


void op_80_xori( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx ^= op->hhll );
}


void op_81_xor( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];

  cpu_flags_bit( emu, *rx ^= ry );
}


void op_82_xor( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  rz = &emu->cpu.r[ op->rz ];
  
  cpu_flags_bit( emu, *rz = rx ^ ry );
}


void op_90_muli( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_mul( emu, *rx, op->hhll );
  *rx *= op->hhll;
}


void op_91_mul( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_mul( emu, *rx, ry );
  *rx *= ry;
}


void op_92_mul( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rz = &emu->cpu.r[ op->rz ];
  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_mul( emu, rx, ry );  
  *rz = rx * ry;
}


void op_A0_divi( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_div( emu, *rx, op->hhll );
  *rx /= op->hhll;
}


void op_A1_div( emulator_t *emu, opcode_t *op )
{
  int16_t *rx, ry;

  rx = &emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_div( emu, *rx, ry );
  *rx /= ry;
}


void op_A2_div( emulator_t *emu, opcode_t *op )
{
  int16_t *rz, rx, ry;

  rz = &emu->cpu.r[ op->rz ];
  rx = emu->cpu.r[ op->rx ];
  ry = emu->cpu.r[ op->ry ];
  cpu_flags_div( emu, rx, ry );  
  *rz = rx / ry;
}


void op_B0_shl( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx <<= op->rz );
}


void op_B1_shr( emulator_t *emu, opcode_t *op )
{
  uint16_t *rx;
  
  rx = ( uint16_t* )&emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx >>= op->rz );
}


void op_B2_sar( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;
  
  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx >>= op->rz );
}


void op_B3_shl( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx <<= emu->cpu.r[ op->ry ] );
}


void op_B4_shr( emulator_t *emu, opcode_t *op )
{
  uint16_t *rx;

  rx = ( uint16_t* )&emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx >>= emu->cpu.r[ op->ry ] );
}


void op_B5_sar( emulator_t *emu, opcode_t *op )
{
  int16_t *rx;

  rx = &emu->cpu.r[ op->rx ];
  cpu_flags_bit( emu, *rx >>= emu->cpu.r[ op->ry ] );
}



void op_C0_push( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ emu->cpu.sp ], &emu->cpu.r[ op->rx ], 2 );
  emu->cpu.sp += 2;
}


void op_C1_pop( emulator_t *emu, opcode_t *op )
{
  emu->cpu.sp -= 2;
  memcpy(&emu->cpu.r[ op->rx ], &emu->ram[ emu->cpu.sp ], 2);
}


void op_C2_pushall( emulator_t *emu, opcode_t *op )
{
  int i;

  for ( i = 0x0; i <= 0xF; ++i )
  {
    memcpy( &emu->ram[ emu->cpu.sp ], &emu->cpu.r[ i ], 2 );
    emu->cpu.sp += 2;
  }
}


void op_C3_popall( emulator_t *emu, opcode_t *op )
{
  int i;

  for ( i = 0xF; i >= 0x0; --i )
  {
    emu->cpu.sp -= 2;
    memcpy( &emu->cpu.r[ i ], &emu->ram[ emu->cpu.sp ], 2 );
  }
}


void op_C4_pushf( emulator_t *emu, opcode_t *op )
{
  memcpy( &emu->ram[ emu->cpu.sp ], &emu->cpu.flags, 1 );
  emu->cpu.sp += 2;
}


void op_C5_popf( emulator_t *emu, opcode_t *op )
{
  emu->cpu.sp -= 2;
  memcpy( &emu->cpu.flags, &emu->ram[ emu->cpu.sp ], 1 );
}


void op_D0_pal( emulator_t *emu, opcode_t *op )
{
  gpu_load_pal( emu, &emu->ram[ (uint16_t) op->hhll ] );
}


void op_D1_pal( emulator_t *emu, opcode_t *op )
{
  gpu_load_pal( emu, &emu->ram[ (uint16_t) emu->cpu.r[ op->rx ] ] );
}


op_t op_table[0x100] =
{
  // 0x - Misc/Video/Audio
  op_00_nop,  
  op_01_cls,  
  op_02_vblnk, 
  op_03_bcg,  
  op_04_spr,  
  op_05_drw,  
  op_06_drw,  
  op_07_rnd, 
  op_08_flip, 
  op_09_snd0, 
  op_0A_snd1,  
  op_0B_snd2, 
  op_0C_snd3, 
  op_0D_snp,  
  op_0E_sng,  
  NULL,

  // 1x - Jumps (Branches)
  op_10_jmp,
  NULL,
  op_12_jx,
  op_13_jme,
  op_14_call,
  op_15_ret,
  op_16_jmp,
  op_17_cx,
  op_18_call,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,

  // 2x - Loads
  op_20_ldi,
  op_21_ldi,
  op_22_ldm,
  op_23_ldm,
  op_24_mov,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 3x - Stores
  op_30_stm,
  op_31_stm,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 4x - Addition
  op_40_addi,
  op_41_add,
  op_42_add,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 5x - Subtraction
  op_50_subi,
  op_51_sub,
  op_52_sub,
  op_53_cmpi,
  op_54_cmp,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 6x - Bitwise AND
  op_60_andi,
  op_61_and,
  op_62_and,
  op_63_tsti,
  op_64_tst,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 7x - Bitwise OR
  op_70_ori,
  op_71_or,
  op_72_or,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 8x - Bitwise XOR
  op_80_xori,
  op_81_xor,
  op_82_xor,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // 9x - Multiplication
  op_90_muli,
  op_91_mul,
  op_92_mul,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // Ax - Division
  op_A0_divi,
  op_A1_div,
  op_A2_div,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // Bx - Logical / Arithmetic Shifts
  op_B0_shl,
  op_B1_shr,
  op_B2_sar,
  op_B3_shl,
  op_B4_shr,
  op_B5_sar,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // Cx - Push / Pop
  op_C0_push,
  op_C1_pop,
  op_C2_pushall,
  op_C3_popall,
  op_C4_pushf,
  op_C5_popf,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  
  // Dx - Palette
  op_D0_pal,
  op_D1_pal,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
 
  // Ex - Unused
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,

  // Fx - Unused
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL, 
  NULL
};
