#ifndef __renumSections_h__
#define __renumSections_h__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"

void writeHeader(const char * f_source, const char * f_dest, Elf32_Ehdr * header);

void writeSectionContentFile(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* new_sect_header, Elf32_Sym * symTab, int nb_symboles, int offset);

void writeSectionHeader(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* sect_header);

Elf32_Shdr * deleteRel(Elf32_Ehdr * header, Elf32_Shdr* sect_header, Elf32_Sym * symTab, int nb_symboles, int * offset);

void renumSect(const char * f_source, const char * f_dest);

#endif

