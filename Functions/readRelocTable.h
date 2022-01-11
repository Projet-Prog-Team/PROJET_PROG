#ifndef __readRelocTable_h__
#define __readRelocTable_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include <string.h>
#include "../structure.h"

Elf32_Rel ** loadRelocTable(FILE *f, Elf32_Main * ELF);

void printRelocTable(FILE *f, Elf32_Main * ELF);

Elf32_Rel **readRelocTable(const char * file, int affichage);


#endif