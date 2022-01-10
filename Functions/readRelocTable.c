#include "readRelocTable.h"


Elf32_Rel ** loadRelocTable(FILE * f, Elf32_Main * ELF){
    int i = 0;
    int k = 0;
    Elf32_Rel **Tab = malloc(sizeof(Elf32_Rel *));
    // Parcours de toutes les sections
    while ((i < ELF->header.e_shnum)) {
        // Si c'est une section de réimplantation
        if ((ELF->sectHeader[i].sh_type == 9)) {
            Tab = realloc(Tab, sizeof(Elf32_Rel *) * (k + 1));
            Tab[k] = malloc(ELF->sectHeader[i].sh_size);
            fseek(f, ELF->sectHeader[i].sh_offset, SEEK_SET);
            ELF->tabSecRel.TabNb[k] = ELF->sectHeader[i].sh_size / 8;
            ELF->tabSecRel.nbSections++;
            for (int j = 0; j < ELF->sectHeader[i].sh_size / 8; j++) {
                fread(&Tab[k][j].r_offset, 4, 1, f);
                Tab[k][j].r_offset = bswap_32(Tab[k][j].r_offset);
                fread(&Tab[k][j].r_info, 4, 1, f);
                Tab[k][j].r_info = bswap_32(Tab[k][j].r_info);
            }
            k++;
        }
        i++;
    }
    if (k == 0) {
        for(int i = 0; i < ELF->tabSecRel.nbSections; i++) {
            free(Tab[i]);
        }
        free(Tab);
        return NULL;
    } else {
        return Tab;
    }
}

void printRelocTable(FILE * f, Elf32_Main * ELF){
    if (ELF->relTable != NULL) {
        Elf32_Word symIndex;
        for(int i=0; i < ELF->tabSecRel.nbSections; i++){    // i : indice de la section courante
            for(int j=0; j < ELF->tabSecRel.TabNb[i]; j++){  // j : numéro de la ligne de la section courante
                printf("%08x\t", ELF->relTable[i][j].r_offset);

                // Info
                printf("%08x\t", ELF->relTable[i][j].r_info);

                // Type
                switch(ELF32_R_TYPE(ELF->relTable[i][j].r_info)) {
                    case 2:
                        printf("R_ARM_ABS32\t");
                        break;
                    case 5:
                        printf("R_ARM_ABS16\t");
                        break;
                    case 8:
                        printf("R_ARM_ABS8\t");
                        break;
                    case 28:
                        printf("R_ARM_CALL\t");
                        break;
                    case 29:
                        printf("R_ARM_JUMP24\t");
                        break;
                }
                // Sym Value
                symIndex = ELF32_R_SYM(ELF->relTable[i][j].r_info);
                printf("%08x\t", ELF->symTable[symIndex].st_value);
                
                // Sym Name

                int scan, compteur;
                char nom_section[512];
                char c;
                if (ELF->symTable[symIndex].st_value == 0) {
                    fseek(f, ELF->sectHeader[ELF->header.e_shstrndx].sh_offset + ELF->sectHeader[ELF->symTable[symIndex].st_shndx].sh_name, SEEK_SET); // On se rend à la position du nom du symbole
                    compteur = 0;
                    scan = fscanf(f, "%c", &c);  
                    while ((scan != EOF) && (c != '\0')) {   // Lecture du nom de symbole dans la string table
                        nom_section[compteur] = c;
                        scan = fscanf(f, "%c", &c);
                        compteur++;
                    }
                    nom_section[compteur] = '\0';              // Sans oublier d'ajouter le \0 de fin de séquence
                    printf("%s\t", nom_section);
                } else {
                    fseek(f, ELF->sectHeader[ELF->header.e_shstrndx -1].sh_offset + ELF->symTable[symIndex].st_name, SEEK_SET); // On se rend à la position du nom du symbole
                    compteur = 0;
                    scan = fscanf(f, "%c", &c);  
                    while ((scan != EOF) && (c != '\0')) {   // Lecture du nom de symbole dans la string table
                        nom_section[compteur] = c;
                        scan = fscanf(f, "%c", &c);
                        compteur++;
                    }
                    nom_section[compteur] = '\0';              // Sans oublier d'ajouter le \0 de fin de séquence
                    printf("%s\t", nom_section);
                }
                printf("\n");      
            }
            printf("\n");                    
        }
    } else {
        printf("There are no relocations in this file.\n");
    }
}