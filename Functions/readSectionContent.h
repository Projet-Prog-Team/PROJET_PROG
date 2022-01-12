#ifndef __readSectionContent_h__
#define __readSectionContent_h__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include <elf.h>
#include "../structure.h"

// Charge le contenu de toutes les sections dans la structure
SectionContent * loadSectionContent(FILE * file, Elf32_Main * ELF);

// Affiche le contenu de toutes les sections à partir de la structure
void printSectionContent(Elf32_Main * ELF);

// Affiche le contenu de la section noSection à partir de la structure
void printSectionContentOne(Elf32_Main * ELF, int noSection);

#endif