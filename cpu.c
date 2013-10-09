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
 
  if ( ! ( instr = &instr_table[ op.op ] ) || ! instr->op )
  {
    emulator_error( emu, "Invalid opcode 0x%08x", op.dw );
  }

  printf("%x %x\n", op.op, op.dw );

  (*(instr->op)) ( emu, &op );

  if ( ! instr->set_pc ) emu->cpu.pc += 4;

  if ( ! instr->set_z ) clear_z( &emu->cpu );
  if ( ! instr->set_o ) clear_o( &emu->cpu );
  if ( ! instr->set_c ) clear_c( &emu->cpu );
  if ( ! instr->set_n ) clear_n( &emu->cpu );
}

int cpu_cond ( emulator_t *emu, int cond )
{
    switch (cond)
    {
        case 0x0: return get_z( &emu->cpu );       
        case 0x1: return ! get_z( &emu->cpu );
        case 0x2: return get_n( &emu->cpu );
        case 0x3: return ! get_n( &emu->cpu );
        case 0x4: return ! get_n( &emu->cpu ) && ! get_z( &emu->cpu );
        case 0x5: return get_o( &emu->cpu );
        case 0x6: return ! get_o( &emu->cpu );
        case 0x7: return ! get_c( &emu->cpu ) && ! get_o( &emu->cpu );
        case 0x8: return ! get_c( &emu->cpu );
        case 0x9: return get_c( &emu->cpu );
        case 0xA: return get_c( &emu->cpu) && get_z( &emu->cpu );
        case 0xB: return get_c( &emu->cpu );
        case 0xC: return get_o( &emu->cpu ) == get_n( &emu->cpu );
        case 0xD: return get_o( &emu->cpu ) != get_n( &emu->cpu );
        case 0xE: return get_o( &emu->cpu ) != get_n( &emu->cpu ) && get_z( &emu->cpu );
        case 0xF: emulator_error( emu, "Invalid condition" );
    }
    
    return 0;
}


