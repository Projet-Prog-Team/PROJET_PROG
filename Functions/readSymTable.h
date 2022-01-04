#ifndef __readSymTable_h__
#define __readSymTable_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "readSectionHeader.h"

Elf32_Sym *loadSymTable(FILE *f, Elf32_Shdr * sectionHeader, Elf32_Ehdr header, int * nb_symboles);

void printSymTable(Elf32_Sym * Tab, Elf32_Shdr * sectionHeader, Elf32_Ehdr header, FILE *f, int nb_symboles);

Elf32_Sym *readSymTable(const char * file, int affichage);


#endif