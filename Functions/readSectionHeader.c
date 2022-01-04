#include "readSectionHeader.h"

Elf32_Shdr *loadTabSectionHeader(FILE *f, Elf32_Ehdr header) {
    Elf32_Shdr *Tab = malloc(header.e_shentsize * header.e_shnum);   // Allocation d'un tableau de structure (16 * 40 octets)
    if (Tab == NULL){
        printf("Erreur d'allocation mémoire du tableau\n");
        return NULL;
    }
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

void printSectionHeader(Elf32_Shdr * Tab, Elf32_Ehdr header, FILE *f) {
    int scan, compteur;
    char nom_section[512];
    char c;
    for(int i = 0; i < header.e_shnum; i++) {                        // Pour chaque section
        printf("[%d]", i);
        
        fseek(f, Tab[header.e_shstrndx].sh_offset + Tab[i].sh_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier

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

int readSections(const char * file, int affichage) {
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    Elf32_Ehdr * header = readHeader(file);
    if (affichage) {
        printSectionHeader(loadTabSectionHeader(f, *header), *header, f);
    } else {
        loadTabSectionHeader(f, *header);
    }

    fclose(f);
    return 0;
}