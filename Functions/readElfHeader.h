#ifndef __readElfHeader_h__
#define __readElfHeader_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"

Elf32_Ehdr loadHeader(FILE * elfFile);

void printHeader(Elf32_Main * ELF_main);

#endif

