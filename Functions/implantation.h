#ifndef __implantation_h__
#define __implantation_h__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "structure.h"


void deleteRel(Elf32_Main * ELF);

void correctSymTable(FILE * f, Elf32_Main * ELF, uint32_t text, uint32_t data);

#endif