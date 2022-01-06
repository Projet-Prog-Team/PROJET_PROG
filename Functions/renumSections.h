#ifndef __renumSections_h__
#define __renumSections_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "readSectionHeader.h"

void writeHeader(const char * f_source, const char * f_dest, Elf32_Ehdr * header);

void writeSectionContentFile(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* sect_header);

void writeSectionHeader(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* sect_header);

void deleteRel(Elf32_Ehdr * header, Elf32_Shdr* sect_header);

void renumSect(const char * f_source, const char * f_dest);

#endif

