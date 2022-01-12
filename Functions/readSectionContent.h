#ifndef __readSectionContent_h__
#define __readSectionContent_h__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include <elf.h>
#include "../structure.h"

SectionContent * loadSectionContent(FILE * file, Elf32_Main * ELF);

void printSectionContent(Elf32_Main * ELF);

void printSectionContentOne(Elf32_Main * ELF, int noSection);

#endif