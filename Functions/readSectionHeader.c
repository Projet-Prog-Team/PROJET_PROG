#include "readSectionHeader.h"
#include <errno.h>
#include <string.h> 

Elf32_Shdr *loadTabSectionHeader(FILE *f, Elf32_Main * ELF) {
    Elf32_Shdr *Tab = malloc(ELF->header.e_shentsize * ELF->header.e_shnum);   // Allocation d'un tableau de structure (16 * 40 octets)
    if (Tab == NULL) {
        printf("Error: Unable to allocate section header table\n");
        exit(1);
    }
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
    char * nom_section;
    for(int i = 0; i < ELF->header.e_shnum; i++) {                        // Pour chaque section
        printf("[%d]\t", i);
        
        nom_section = printName(f, ELF->sectHeader[ELF->header.e_shstrndx].sh_offset + ELF->sectHeader[i].sh_name);
        printf("%s\t", nom_section);                       // Affichages...
        free(nom_section);

        // Switch pour afficher le nom du type plutôt que son numéro
        switch (ELF->sectHeader[i].sh_type) {
            case 0:
                printf("NULL\t");
                break;
            case 1:
                printf("PROGBITS\t");
                break;
            case 2:
                printf("SYMTAB\t");
                break;
            case 3:
                printf("STRTAB\t");
                break;
            case 4:
                printf("RELA\t");
                break;
            case 5:
                printf("HASH\t");
                break;
            case 6:
                printf("DYNAMIC\t");
                break;
            case 7:
                printf("NOTE\t");
                break;
            case 8:
                printf("NOBITS\t");
                break;
            case 9:
                printf("REL\t");
                break;
            case 10:
                printf("SHLIB\t");
                break;
            case 11:
                printf("DYNSYM\t");
                break;
            case 1879048195:
                printf("ARM_ATTRIBUTES\t");
                break;
            case 1879048192:
                printf("LOPROC\t");
                break;
            case 2147483647:
                printf("HIPROC\t");
                break;
            case 2147483648:
                printf("LOUSER\t");
                break;
            case 4294967295:
                printf("HIUSER\t");
                break;
            default:
                printf("UNKNOWN");
                break;
        }
        printf("%08x\t", ELF->sectHeader[i].sh_addr);
        printf("%06x\t", ELF->sectHeader[i].sh_offset);
        printf("%06x\t", ELF->sectHeader[i].sh_size);
        printf("%02x\t", ELF->sectHeader[i].sh_entsize);

        if (ELF->sectHeader[i].sh_flags & 1) {
            printf("W\t");
        }
        if (ELF->sectHeader[i].sh_flags & 2) {
            printf("A\t");
        }
        if (ELF->sectHeader[i].sh_flags & 4) {
            printf("X\t");
        }
        if (ELF->sectHeader[i].sh_flags & 16) {
            printf("M\t");
        }
        if (ELF->sectHeader[i].sh_flags & 32) {
            printf("S\t");
        }
        if (ELF->sectHeader[i].sh_flags & 64) {
            printf("I\t");
        }
        if (ELF->sectHeader[i].sh_flags & 128) {
            printf("L\t");
        }

        
        printf("%d\t", ELF->sectHeader[i].sh_link);
        printf("%d\t", ELF->sectHeader[i].sh_info);
        printf("%x\n", ELF->sectHeader[i].sh_addralign);
    }
}