#ifndef __implantation_h__
#define __implantation_h__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "structure.h"

// Supprime les tables de relocations dans la structure principale et corrige les valeurs qui doivent être modifiés suite à ces suppressions.
void deleteRel(Elf32_Main * ELF);

// Corrige la valeur des symboles dans la table des symboles en fonction des adresses auxquelles les sections .text et .data vont être chargées.
void correctSymTable(FILE * f, Elf32_Main * ELF, uint32_t text, uint32_t data);

// Fais les réimplantations de type R_ARM_ABS.
void correctABSReloc(Elf32_Main * ELF);
#endif