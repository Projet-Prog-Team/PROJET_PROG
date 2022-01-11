#ifndef __readSymTable_h__
#define __readSymTable_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"

Elf32_Sym *loadSymTable(FILE *f, Elf32_Main * ELF);

void printSymTable(FILE * f, Elf32_Main * ELF);


#endif