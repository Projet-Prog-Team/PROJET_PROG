#ifndef __readSectionContent_h__
#define __readSectionContent_h__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include <elf.h>
#include "../structure.h"

void printRawSectionContent(FILE *file, Elf32_Main * ELF);

#endif