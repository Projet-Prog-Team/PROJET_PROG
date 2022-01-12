#include "readSymTable.h"

Elf32_Sym *loadSymTable(FILE *f, Elf32_Main * ELF) {
    int i = 0;
    while ((i < ELF->header.e_shnum) && (ELF->sectHeader[i].sh_type != 2)) { // Tant que i n'est pas l'indice de la table de symboles
        i++;
    }
    if (i >= ELF->header.e_shnum) { // Si il n'y a pas de table des symboles
        ELF->nb_symboles = -1;
        return NULL;
    } else {    // Il y a une table des symboles
        Elf32_Sym *Tab = malloc(ELF->sectHeader[i].sh_size * 16);
        if (Tab == NULL) {
            printf("Error: Unable to allocate Symbol Table memory\n");
            return NULL;
        }
        fseek(f, ELF->sectHeader[i].sh_offset, SEEK_SET);
        for (int j = 0; j < ELF->sectHeader[i].sh_size / 16; j++) { // Chargement de chaque entrée de la table des symboles dans la structure
            if(fread(&Tab[j].st_name, 4, 1, f) != 1) {exit(1);}
            Tab[j].st_name = bswap_32(Tab[j].st_name);
            if(fread(&Tab[j].st_value, 4, 1, f) != 1) {exit(1);}
            Tab[j].st_value = bswap_32(Tab[j].st_value);
            if(fread(&Tab[j].st_size, 4, 1, f) != 1) {exit(1);}
            Tab[j].st_size = bswap_32(Tab[j].st_size);
            if(fread(&Tab[j].st_info, 1, 1, f) != 1) {exit(1);}
            if(fread(&Tab[j].st_other, 1, 1, f) != 1) {exit(1);}
            if(fread(&Tab[j].st_shndx, 2, 1, f) != 1) {exit(1);}
            Tab[j].st_shndx = bswap_16(Tab[j].st_shndx);
        }
        ELF->nb_symboles = ELF->sectHeader[i].sh_size / 16;  // Calcul du nombre de symboles
        return Tab;
    }
}

void printSymTable(FILE * f, Elf32_Main * ELF) {
    if (ELF->symTable != NULL) {
        for (int j = 0; j < ELF->nb_symboles; j++) {
            printf("%d:\t", j);
            printf("%08x\t", ELF->symTable[j].st_value);
            printf("%d\t", ELF->symTable[j].st_size);
            switch (ELF->symTable[j].st_info & ELF32_ST_TYPE(ELF->symTable[j].st_info)) {
                case 0:
                    printf("NOTYPE\t");
                    break;
                case 1:
                    printf("OBJECT\t");
                    break;
                case 2:
                    printf("FUNC\t");
                    break;
                case 3:
                    printf("SECTION\t");
                    break;
                case 4:
                    printf("FILE\t");
                    break;
                case 13:
                    printf("LOPROC\t");
                    break;
                case 15:
                    printf("HIPROC\t");
                    break;
            }
            switch(1 & ELF32_ST_BIND(ELF->symTable[j].st_info)){
                case 0:
                    printf("LOCAL\t");
                    break;
                case 1:
                    printf("GLOBAL\t");
                    break;
                case 2:
                    printf("WEAK\t");
                    break;
                case 13:
                    printf("LOPROC\t");
                    break;
                case 15:
                    printf("HIPROC\t");
                    break;
            }
            printf("DEFAULT\t");

            switch(ELF->symTable[j].st_shndx){
                case 0:
                    printf("UND\t");
                    break;
                case 65280:
                    printf("LORESERVE\t");
                    break;
                case 65311:
                    printf("HIPROC\t");
                    break;
                case 65521:
                    printf("ABS\t");
                    break;
                case 65522:
                    printf("COMMON\t");
                    break;
                case 65535:
                    printf("HIRESERVE\t");
                    break; 
                default:
                    printf("%d\t", ELF->symTable[j].st_shndx);
                    break;    
            }

            char * nom_section;

            nom_section = printName(f, ELF->sectHeader[ELF->header.e_shstrndx - 1].sh_offset + ELF->symTable[j].st_name);
            printf("%s\t", nom_section);

            free(nom_section);

            printf("\n");
        }
    }
}