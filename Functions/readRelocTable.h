#ifndef __readRelocTable_h__
#define __readRelocTable_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "readSymTable.h"
#include <string.h>

typedef struct {
    int TabNb[256];
    int nbSections;
} TableauSectionReloc;

Elf32_Rel **loadRelocTable(FILE *f, Elf32_Shdr * sectionHeader, Elf32_Ehdr header, TableauSectionReloc * tabSecRel);

void printRelocTable(FILE *f, Elf32_Rel ** tabRel, Elf32_Sym * Tab, Elf32_Shdr * sectionHeader, Elf32_Ehdr header, TableauSectionReloc * tabSecRel);

Elf32_Rel **readRelocTable(const char * file, int affichage);


#endif