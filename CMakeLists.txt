# This file is part of the ncemu project
# Licensing information can be found in the LICENSE file
# (C) 2013 Licker Nandor. All rights reserved.

CMAKE_MINIMUM_REQUIRED( VERSION 2.8)
PROJECT( NCEMU )

SET( SOURCES main.c
             emulator.c
             cpu.c
             instr.c
             gpu.c
             crc.c )

SET( HEADERS emulator.h
             cpu.h
             gpu.h
             crc.h )

ADD_EXECUTABLE( ncemu ${SOURCES} ${HEADERS} )
TARGET_LINK_LIBRARIES( ncemu SDL)
