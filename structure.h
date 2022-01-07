#ifndef __structure_h__
#define __structure_h__
#include <getopt.h>
#include <stdio.h>

typedef struct {
    int TabNb[256];
    int nbSections;
} TableauSectionReloc;

typedef struct {
    Elf32_Ehdr header;
    Elf32_Shdr * sectHeader;
    Elf32_Sym * symTable;
    Elf32_Rel ** relTable;
    TableauSectionReloc tabSecRel;
    int nb_symboles;
} Elf32_Main;


#endif
