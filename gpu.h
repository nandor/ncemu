// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef GPU_H
#define GPU_H

#include <stdint.h>

#define DISPLAY_X 320
#define DISPLAY_Y 240

typedef struct _emulator_t emulator_t;

typedef struct 
{
    uint8_t bgc;
    union
    {
      uint16_t sprw;

      struct
      {
        uint8_t sprite_w;
        uint8_t sprite_h;
      };
    };

    uint8_t hflip;
    uint8_t vflip;
    uint8_t vblank;

    uint8_t   vram[ DISPLAY_X * DISPLAY_Y ];
    uint32_t  pal[ 16 ];
} gpu_t;


void gpu_draw_sprite( emulator_t *emu, uint8_t *spr, int16_t x, int16_t y );

#endif

