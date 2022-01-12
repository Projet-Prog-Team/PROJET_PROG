#ifndef __readSymTable_h__
#define __readSymTable_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"

// Charge la table des symboles dans la structure
Elf32_Sym *loadSymTable(FILE *f, Elf32_Main * ELF);

// Affiche la table des symboles à partir de la structure
void printSymTable(FILE * f, Elf32_Main * ELF);


#endif