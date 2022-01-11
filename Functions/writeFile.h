#ifndef __writeFile_h__
#define __writeFile_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"
#include <string.h>

void writeHeader(Elf32_Main * ELF, FILE * fdest);

void writeSectionContent(Elf32_Main * ELF, FILE * fdest);

void writeSectionHeader(Elf32_Main * ELF, FILE * fdest);

#endif