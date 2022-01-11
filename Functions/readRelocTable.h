#ifndef __readRelocTable_h__
#define __readRelocTable_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include <string.h>
#include "../structure.h"

// Charge toutes les tables de réimplantation dans la structure
Elf32_Rel ** loadRelocTable(FILE *f, Elf32_Main * ELF);

// Affiche toutes les tables de réimplantation à partir de la structure
void printRelocTable(FILE *f, Elf32_Main * ELF);

#endif