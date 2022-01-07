#include "readSectionHeader.h"
#include <errno.h>
#include <string.h> 

Elf32_Shdr *loadTabSectionHeader(FILE *f, Elf32_Main * ELF) {
    Elf32_Shdr *Tab = malloc(ELF->header.e_shentsize * ELF->header.e_shnum);   // Allocation d'un tableau de structure (16 * 40 octets)
    if (Tab == NULL){
        printf("Erreur d'allocation mémoire du tableau\n");
        return NULL;
    }
    fseek(f, ELF->header.e_shoff, SEEK_SET);                              // Déplacement au début des section headers
    for(int i = 0; i < ELF->header.e_shnum; i++){                         // Pour chaque section
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

void printSectionHeader(FILE *f, Elf32_Main * ELF) {
    int scan, compteur;
    char nom_section[512];
    char c;
    for(int i = 0; i < ELF->header.e_shnum; i++) {                        // Pour chaque section
        printf("[%d]", i);
        
        fseek(f, ELF->sectHeader[ELF->header.e_shstrndx].sh_offset + ELF->sectHeader[i].sh_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier

        compteur = 0;
        scan = fscanf(f, "%c", &c);  
        while ((scan != EOF) && (c != '\0') && (compteur < 17)) {   // Lecture du nom de la section dans la string table
            nom_section[compteur] = c;
            scan = fscanf(f, "%c", &c);
            compteur++;
        }
        nom_section[compteur] = '\0';              // Sans oublier d'ajouter le \0 de fin de séquence
        printf("   %s", nom_section);                       // Affichages...

        // Switch pour afficher le nom du type plutôt que son numéro
        switch (ELF->sectHeader[i].sh_type) {
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
        printf("  %08x", ELF->sectHeader[i].sh_addr);
        printf("  %06x", ELF->sectHeader[i].sh_offset);
        printf("  %06x", ELF->sectHeader[i].sh_size);
        printf("  %02x", ELF->sectHeader[i].sh_entsize);
        printf("   ");

        if (ELF->sectHeader[i].sh_flags & 1) {
            printf("W");
        }
        if (ELF->sectHeader[i].sh_flags & 2) {
            printf("A");
        }
        if (ELF->sectHeader[i].sh_flags & 4) {
            printf("X");
        }
        if (ELF->sectHeader[i].sh_flags & 16) {
            printf("M");
        }
        if (ELF->sectHeader[i].sh_flags & 32) {
            printf("S");
        }
        if (ELF->sectHeader[i].sh_flags & 64) {
            printf("I");
        }
        if (ELF->sectHeader[i].sh_flags & 128) {
            printf("L");
        }

        
        printf("  %d", ELF->sectHeader[i].sh_link);
        printf("  %d", ELF->sectHeader[i].sh_info);
        printf("  %x\n", ELF->sectHeader[i].sh_addralign);
    }
}