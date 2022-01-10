#ifndef __structure_h__
#define __structure_h__

typedef struct {
    int TabNb[256];
    int nbSections;
} TableauSectionReloc;

typedef struct {
    unsigned char * section;
    int size;
} SectionContent;

typedef struct {
    int indice;
    int offset;
} tempTab;

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
