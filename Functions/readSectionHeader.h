#ifndef __readSectionHeader_h__
#define __readSectionHeader_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "debug.h"
#include "../structure.h"

// Charge l'header des sections dans la structure
Elf32_Shdr *loadTabSectionHeader(FILE *f, Elf32_Main * ELF);

// Affiche l'header des sections à partir de la structure
void printSectionHeader(FILE *f, Elf32_Main * ELF);

#endif