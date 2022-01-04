#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "debug.h"

/*
Elf32_Shdr *affiche_tab_sect(FILE *f, Elf32_Ehdr header) {
    Elf32_Shdr *Tab = malloc(header.e_shentsize * header.e_shnum);   // Allocation d'un tableau de structure (16 * 40 octets)
    fseek(f, header.e_shoff, SEEK_SET);                              // Déplacement au début des section headers
    for(int i = 0; i < header.e_shnum; i++){                         // Pour chaque section
        Elf32_Word *Adresse = (Elf32_Word *) &Tab[i];                // Adresse de parcours des mots de chaque structure
        for(int j = 0; j < 10; j++){                                 // Pour chaque mot (32 bits)
            if (fread(Adresse, 4, 1, f) != 1) {                      // Lecture du mot (1 bloc de 4 octets)
                printf("Erreur : impossible de lire le mot dans la section header\n");
                free(Tab);
                return NULL;
            };
            *Adresse = bswap_32(*Adresse);                           // Inversion du boutisme
            Adresse = Adresse + 1;                                   // Passage au mot suivant
        }
    }
    return Tab;
}

void afficher_tableau(Elf32_Shdr * Tab, Elf32_Ehdr header, FILE *f) {
    char * nom_section = malloc(sizeof(char) * 256);
    for(int i = 0; i < header.e_shnum; i++) {                        // Pour chaque section
        printf("\n\n[%d] : \n", i);

        fseek(f, Tab[header.e_shstrndx].sh_offset + Tab[i].sh_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier
        if (fscanf(f, "%s", nom_section) == EOF) {                   // On lit le nom de la section et le place dans nom_section
            printf("Erreur : impossible de lire le nom de section\n");
            return;
        };
        printf("sh_name = %s\n", nom_section);                       // Affichages...


        // Switch pour afficher le nom du type plutôt que son numéro
        switch (Tab[i].sh_type) {
            case 0:
                printf("NULL\n");
                break;
            case 1:
                printf("PROGBITS\n");
                break;
            case 2:
                printf("SYMTAB\n");
                break;
            case 3:
                printf("STRTAB\n");
                break;
            case 4:
                printf("RELA\n");
                break;
            case 5:
                printf("HASH\n");
                break;
            case 6:
                printf("DYNAMIC\n");
                break;
            case 7:
                printf("NOTE\n");
                break;
            case 8:
                printf("NOBITS\n");
                break;
            case 9:
                printf("REL\n");
                break;
            case 10:
                printf("SHLIB\n");
                break;
            case 11:
                printf("DYNSYM\n");
                break;
            case 1879048192:
                printf("LOPROC\n");
                break;
            case 2147483647:
                printf("HIPROC\n");
                break;
            case 2147483648:
                printf("HOUSER\n");
                break;
            case 4294967295:
                printf("HIUSER\n");
                break;
        }
        printf("%x\n", Tab[i].sh_addr);
        printf("%x\n", Tab[i].sh_offset);
        printf("%x\n", Tab[i].sh_size);
        printf("%x\n", Tab[i].sh_entsize);
        // Switch pour afficher le nom du flag plutôt que son numéro
        switch (Tab[i].sh_flags) {
            case 1:
                printf("WA\n");
                break;
            case 2:
                printf("AX\n");
                break;
            case 4:
                printf("I\n");
                break;
            case 4026531840:
                printf("MS\n");
                break;
        }
        printf("sh_link = %d\n", Tab[i].sh_link);
        printf("sh_info = %d\n", Tab[i].sh_info);
        printf("sh_addralign = %x\n", Tab[i].sh_addralign);
    }   
}
*/



Elf32_Shdr *affiche_tab_sect(FILE *f) {
    Elf32_Shdr *Tab = malloc(40 * 16);   // Allocation d'un tableau de structure (16 * 40 octets)
    if (Tab == NULL){
        printf("Erreur d'allocation mémoire du tableau\n");
        return NULL;
    }
    fseek(f, 772, SEEK_SET);                              // Déplacement au début des section headers
    for(int i = 0; i < 16; i++){                         // Pour chaque section
        Elf32_Word *Adresse = (Elf32_Word *) &Tab[i];                // Adresse de parcours des mots de chaque structure
        for(int j = 0; j < 10; j++){                                 // Pour chaque mot (32 bits)
            if (fread(Adresse, 4, 1, f) != 1) {                      // Lecture du mot (1 bloc de 4 octets)
                printf("Erreur : impossible de lire le mot dans la section header\n");
                free(Tab);
                return NULL;
            };
            *Adresse = bswap_32(*Adresse);                           // Inversion du boutisme
            Adresse = Adresse + 1;                                   // Passage au mot suivant
        }
    }
    return Tab;
}

void afficher_tableau(Elf32_Shdr * Tab, FILE *f) {
    int scan, compteur;
    char nom_section[512];
    char c;
    for(int i = 0; i < 16; i++) {                        // Pour chaque section
        printf("[%d]", i);
        
        fseek(f, Tab[15].sh_offset + Tab[i].sh_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier

        compteur = 0;
        scan = fscanf(f, "%c", &c);  
        while ((scan != EOF) && (c != '\0') && (compteur < 16)) {   // Lecture du nom de la section dans la string table
            nom_section[compteur] = c;
            scan = fscanf(f, "%c", &c);
            compteur++;
        }
        nom_section[compteur] = c;              // Sans oublier d'ajouter le \0 de fin de séquence
        printf("   %s", nom_section);                       // Affichages...

        // Switch pour afficher le nom du type plutôt que son numéro
        switch (Tab[i].sh_type) {
            case 0:
                printf("  NULL");
                break;
            case 1:
                printf("  PROGBITS");
                break;
            case 2:
                printf("  SYMTAB");
                break;
            case 3:
                printf("  STRTAB");
                break;
            case 4:
                printf("  RELA");
                break;
            case 5:
                printf("  HASH");
                break;
            case 6:
                printf("  DYNAMIC");
                break;
            case 7:
                printf("  NOTE");
                break;
            case 8:
                printf("  NOBITS");
                break;
            case 9:
                printf("  REL");
                break;
            case 10:
                printf("  SHLIB");
                break;
            case 11:
                printf("  DYNSYM");
                break;
            case 1879048195:
                printf("   ARM_ATTRIBUTES");
                break;
            case 1879048192:
                printf("  LOPROC");
                break;
            case 2147483647:
                printf("  HIPROC");
                break;
            case 2147483648:
                printf("   LOUSER");
                break;
            case 4294967295:
                printf("   HIUSER");
                break;
            default:
                printf("   UNKNOWN");
                break;
        }
        printf("  %08x", Tab[i].sh_addr);
        printf("  %06x", Tab[i].sh_offset);
        printf("  %06x", Tab[i].sh_size);
        printf("  %02x", Tab[i].sh_entsize);
        // TO DO : afficher le nom du flag plutôt que son numéro

        printf("   ");

        if (Tab[i].sh_flags & 1) {
            printf("W");
        }
        if (Tab[i].sh_flags & 2) {
            printf("A");
        }
        if (Tab[i].sh_flags & 4) {
            printf("X");
        }
        if (Tab[i].sh_flags & 16) {
            printf("M");
        }
        if (Tab[i].sh_flags & 32) {
            printf("S");
        }
        if (Tab[i].sh_flags & 64) {
            printf("I");
        }
        if (Tab[i].sh_flags & 128) {
            printf("L");
        }

        
        printf("  %d", Tab[i].sh_link);
        printf("  %d", Tab[i].sh_info);
        printf("  %x\n", Tab[i].sh_addralign);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Erreur : nombre incorrect d'arguments\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    afficher_tableau(affiche_tab_sect(f), f);

    fclose(f);
    return 0;
}