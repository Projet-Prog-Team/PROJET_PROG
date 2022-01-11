#ifndef __readElfHeader_h__
#define __readElfHeader_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../structure.h"

// Charge le header ELF dans la structure
Elf32_Ehdr loadHeader(FILE * elfFile);

// Affiche l'header ELF à partir de la structure
void printHeader(Elf32_Main * ELF_main);

#endif

