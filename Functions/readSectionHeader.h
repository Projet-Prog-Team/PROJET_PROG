#ifndef __readSectionHeader_h__
#define __readSectionHeader_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "debug.h"
#include "../structure.h"

Elf32_Shdr *loadTabSectionHeader(FILE *f, Elf32_Main * ELF);

void printSectionHeader(FILE *f, Elf32_Main * ELF);

#endif