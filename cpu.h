// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct _emulator_t emulator_t;


typedef struct _cpu_t
{
    uint16_t pc;
    uint16_t sp;
    int16_t r[16];
    uint8_t flags;    
} cpu_t;


typedef union _opcode_t
{
  uint32_t dw;

  struct
  {
    uint8_t op;
    union
    {
      struct
      {
        uint8_t rx : 4;
        uint8_t ry : 4;
      };

      uint8_t ad;
    };

    union
    {
      uint16_t hhll;
      uint16_t vtsr;

      struct
      {
        uint8_t ll;
        uint8_t hh;
      };

      struct
      {
        uint8_t rz : 4;
      };
    };
  };
} __attribute__(( packed )) opcode_t;


typedef void ( *op_t ) ( emulator_t *, opcode_t * );


#define set_n( c ) ( (c)->flags |= (1 << 7 ) )
#define set_o( c ) ( (c)->flags |= (1 << 6 ) )
#define set_z( c ) ( (c)->flags |= (1 << 2 ) )
#define set_c( c ) ( (c)->flags |= (1 << 1 ) )
#define get_n( c ) ( ( (c)->flags & (1 << 7 ) ) != 0 )
#define get_o( c ) ( ( (c)->flags & (1 << 6 ) ) != 0 )
#define get_z( c ) ( ( (c)->flags & (1 << 2 ) ) != 0 )
#define get_c( c ) ( ( (c)->flags & (1 << 1 ) ) != 0 )
#define clear_n( c ) ( (c)->flags &= ~(1 << 7 ) )
#define clear_o( c ) ( (c)->flags &= ~(1 << 6 ) )
#define clear_z( c ) ( (c)->flags &= ~(1 << 2 ) )
#define clear_c( c ) ( (c)->flags &= ~(1 << 1 ) )


void cpu_tick( emulator_t *emu );
int  cpu_cond( emulator_t *emu, int cond );
void cpu_flags_add( emulator_t *emu, int16_t a, int16_t b );
void cpu_flags_sub( emulator_t *emu, int16_t a, int16_t b );  
void cpu_flags_mul( emulator_t *emu, int16_t a, int16_t b );
void cpu_flags_div( emulator_t *emu, int16_t a, int16_t b );
void cpu_flags_bit( emulator_t *emu, int16_t r );

#endif

