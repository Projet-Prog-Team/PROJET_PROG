#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "debug.h"

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
        printf("\n\nStructure numéro %d : \n", i);

        fseek(f, Tab[header.e_shstrndx].sh_offset + Tab[i].sh_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier
        if (fscanf(f, "%s", nom_section) == EOF) {                   // On lit le nom de la section et le place dans nom_section
            printf("Erreur : impossible de lire le nom de section\n");
            return;
        };
        printf("sh_name = %s\n", nom_section);                       // Affichages...
        printf("sh_type = %d\n", Tab[i].sh_type);
        printf("sh_flags = %d\n", Tab[i].sh_flags);
        printf("sh_addr = %d\n", Tab[i].sh_addr);
        printf("sh_offset = %d\n", Tab[i].sh_offset);
        printf("sh_size = %d\n", Tab[i].sh_size);
        printf("sh_link = %d\n", Tab[i].sh_link);
        printf("sh_info = %d\n", Tab[i].sh_info);
        printf("sh_addralign = %d\n", Tab[i].sh_addralign);
        printf("sh_entsize = %d\n", Tab[i].sh_entsize);
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