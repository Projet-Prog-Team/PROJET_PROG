#ifndef __readSectionHeader_h__
#define __readSectionHeader_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "debug.h"
#include "readElfHeader.h"

Elf32_Shdr *loadTabSectionHeader(FILE *f, Elf32_Ehdr header);

void printSectionHeader(Elf32_Shdr * Tab, Elf32_Ehdr header, FILE *f);

int readSectionsHeader(const char * file, int affichage);

#endif