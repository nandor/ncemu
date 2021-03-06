// This file is part of the ncemu project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.

#ifndef GPU_H
#define GPU_H


#include <stdint.h>


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

    uint8_t   vram[ 320 * 240 ];
    uint32_t  pal[ 16 ];
} gpu_t;


void gpu_init( emulator_t *emu );
void gpu_free( emulator_t *emu );
void gpu_draw_sprite( emulator_t *emu, uint8_t *spr, int16_t x, int16_t y );
void gpu_blit( emulator_t *emu );
void gpu_load_pal( emulator_t *emu, uint8_t *loc );


#endif

