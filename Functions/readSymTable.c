#include "readSymTable.h"

Elf32_Sym *loadSymTable(FILE *f, Elf32_Main * ELF) {
    int i = 0;
    while ((i < ELF->header.e_shnum) && (ELF->sectHeader[i].sh_type != 2)) {
        i++;
    }
    if (i >= ELF->header.e_shnum) {
        ELF->nb_symboles = -1;
        return NULL;
    } else {
        Elf32_Sym *Tab = malloc(ELF->sectHeader[i].sh_size * 16);
        fseek(f, ELF->sectHeader[i].sh_offset, SEEK_SET);
        for (int j = 0; j < ELF->sectHeader[i].sh_size / 16; j++) {
            fread(&Tab[j].st_name, 4, 1, f);
            Tab[j].st_name = bswap_32(Tab[j].st_name);
            fread(&Tab[j].st_value, 4, 1, f);
            Tab[j].st_value = bswap_32(Tab[j].st_value);
            fread(&Tab[j].st_size, 4, 1, f);
            Tab[j].st_size = bswap_32(Tab[j].st_size);
            fread(&Tab[j].st_info, 1, 1, f);
            fread(&Tab[j].st_other, 1, 1, f);
            fread(&Tab[j].st_shndx, 2, 1, f);
            Tab[j].st_shndx = bswap_16(Tab[j].st_shndx);
        }
        ELF->nb_symboles = ELF->sectHeader[i].sh_size / 16;
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

            int scan, compteur;
            char nom_section[512];
            char c;
            fseek(f, ELF->sectHeader[ELF->header.e_shstrndx - 1].sh_offset + ELF->symTable[j].st_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier
            compteur = 0;
            scan = fscanf(f, "%c", &c);  
            while ((scan != EOF) && (c != '\0')) {   // Lecture du nom de la section dans la string ELF->symTablele
                nom_section[compteur] = c;
                scan = fscanf(f, "%c", &c);
                compteur++;
            }
            nom_section[compteur] = '\0';              // Sans oublier d'ajouter le \0 de fin de séquence
            printf("%s\t", nom_section);              // Affichages...
            printf("\n");
        }
    }
}