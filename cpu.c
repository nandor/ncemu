// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#include <string.h>
#include <stdio.h>
#include "emulator.h"
#include "cpu.h"


extern op_t op_table[0x100];


void cpu_tick( emulator_t * emu )
{
  opcode_t op;
  op_t func;

  memcpy(&op, emu->ram + emu->cpu.pc, 4);

  if ( ! ( func = op_table[ op.op ] ) )
  {
    emulator_error( emu, "Invalid opcode 0x%08x", op.dw );
  }
 
  emu->cpu.pc += 4;
  (*func) ( emu, &op );
}


int cpu_cond ( emulator_t *emu, int cond )
{
  cpu_t *cpu;

  cpu = &emu->cpu;  
  switch (cond)
  {    
    case 0x0: return get_z( cpu );       
    case 0x1: return ! get_z( cpu );
    case 0x2: return get_n( cpu );
    case 0x3: return ! get_n( cpu );
    case 0x4: return ! get_n( cpu ) && ! get_z( cpu );
    case 0x5: return get_o( cpu );
    case 0x6: return ! get_o( cpu );
    case 0x7: return ! get_c( cpu ) && ! get_o( cpu );
    case 0x8: return ! get_c( cpu );
    case 0x9: return get_c( cpu );
    case 0xA: return get_c( cpu) && get_z( cpu );
    case 0xB: return get_c( cpu );
    case 0xC: return get_o( cpu ) == get_n( cpu );
    case 0xD: return get_o( cpu ) != get_n( cpu );
    case 0xE: return get_o( cpu ) != get_n( cpu ) && get_z( cpu );
    case 0xF: emulator_error( emu, "Invalid condition" );
  }
  
  return 0;
}


void cpu_flags_add( emulator_t *emu, int16_t a, int16_t b )
{
  uint32_t rd;
  int16_t rh;

  emu->cpu.flags = 0x0;
  rd = ( uint16_t )a + ( uint16_t) b;
  rh = a + b;

  if ( rd > UINT16_MAX ) set_c( &emu->cpu );
  if ( rh == 0) set_z( &emu->cpu );
  if ( rh > 0 && a < 0 && b < 0 ) set_o( &emu->cpu );
  if ( rh < 0 && a > 0 && b > 0 ) set_o( &emu->cpu );
  if ( rh < 0 ) set_n( &emu->cpu );
}


void cpu_flags_sub( emulator_t *emu, int16_t a, int16_t b )
{
  uint32_t rd;
  int16_t rh;

  emu->cpu.flags = 0x0;
  rd = ( uint16_t )a - ( uint16_t) b;
  rh = a + b;

  if ( rd > UINT16_MAX ) set_c( &emu->cpu );
  if ( rh == 0) set_z( &emu->cpu );
  if ( rh > 0 && a < 0 && b < 0 ) set_o( &emu->cpu );
  if ( rh < 0 && a > 0 && b > 0 ) set_o( &emu->cpu );
  if ( rh < 0 ) set_n( &emu->cpu );
}


void cpu_flags_mul( emulator_t *emu, int16_t a, int16_t b )
{
  uint32_t rd;
  
  emu->cpu.flags = 0x0;
  rd = ( uint16_t)a * ( uint16_t ) b;

  if (rd > UINT16_MAX ) set_c( &emu->cpu );
  if (rd == 0 ) set_z( &emu->cpu );
  if (rd < 0 ) set_n( &emu->cpu );
}


void cpu_flags_div( emulator_t *emu, int16_t a, int16_t b )
{
  int16_t r;

  emu->cpu.flags = 0x0;
  r = a / b;

  if ( a % b != 0 ) set_c( &emu->cpu );
  if ( r == 0 ) set_z( &emu->cpu );
  if ( r < 0 ) set_n( &emu->cpu );
}


void cpu_flags_bit( emulator_t *emu, int16_t r )
{
  if (r == 0 ) set_z( &emu->cpu );
  if (r < 0 ) set_n( &emu->cpu );
}
