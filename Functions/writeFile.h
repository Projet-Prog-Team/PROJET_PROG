#ifndef __writeFile_h__
#define __writeFile_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"
#include <string.h>

// Ecrit le header dans fdest à l'aide de la structure ELF
void writeHeader(Elf32_Main * ELF, FILE * fdest);

// Ecrit le contenu des sections dans fdest à l'aide de la structure ELF
void writeSectionContent(Elf32_Main * ELF, FILE * fdest);

// Ecrit les sections header dans fdest à l'aide de la structure ELF
void writeSectionHeader(Elf32_Main * ELF, FILE * fdest);

#endif