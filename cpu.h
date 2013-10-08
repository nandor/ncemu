// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct _emulator_t emulator_t;

typedef struct _cpu_t
{
    int16_t pc;
    int8_t  flags;
    int16_t r[16];
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
} opcode_t;

typedef void ( *instr_t ) ( emulator_t *, opcode_t * );

void cpu_tick ( emulator_t * emu );

#endif

