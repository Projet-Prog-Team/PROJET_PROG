#ifndef __structure_h__
#define __structure_h__
#include "util.h"

// Structure qui contient le nombre de sections de réimplantation ainsi que la taille de chacune d'elles
typedef struct {
    int TabNb[256];
    int nbSections;
} TableauSectionReloc;

// Structure qui contient le contenu d'une section ainsi que sa taille
typedef struct {
    unsigned char * section;
    int size;
} SectionContent;

// Structure qui permet de trier la table des sectionContent et la table des sectionsHeader en même temps
typedef struct {
    int indice;
    int offset;
} tempTab;

// Structure principale qui contient toutes les structures nécéssaires aux fonctions codées
typedef struct {
    Elf32_Ehdr header;
    Elf32_Shdr * sectHeader;
    Elf32_Sym * symTable;
    Elf32_Rel ** relTable;
    SectionContent * sectionContent;
    TableauSectionReloc tabSecRel;
    int nb_symboles;
} Elf32_Main;

#endif
