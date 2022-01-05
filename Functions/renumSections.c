#include "renumSections.h"

void writeSectionContentFile(const char * f_source, const char * f_dest) { // A modif : mettre les structures en parametres
    Elf32_Ehdr header = *readHeader(f_source);
    FILE* fsource = fopen(f_source,"r");
    FILE* fdest = fopen(f_dest,"w");
    Elf32_Shdr* sect_header = loadTabSectionHeader(fsource, header);
    for(int i = 0; i < header.e_shnum; i++) {   // Parcours de chaque section
        fseek(fsource, sect_header[i].sh_offset, SEEK_SET);
        if(sect_header[i].sh_type!=SHT_NOBITS) {
            int size = sect_header[i].sh_size;
            for(int j = 0; j < size; j++) {
                char car[1] = {fgetc(fsource)};
                fwrite(car,1,1,fdest);
            }
        }
    }
    fclose(fdest);
    fclose(fsource);
}

/*void (const char * file, Elf32_Shdr * sectionHeader, Elf32_Ehdr header) {
    FILE *f = fopen(file, "r");
    FILE *fdest = fopen("sortie.o", "w");
    if ((f == NULL) || (fdest == NULL)) {
        printf("Erreur : impossible d'ouvrir le(s) fichier(s)\n");
        return NULL;
    }
    



    long int size;
    int i = 0;
    while (i < header.e_shnum) {
        // Si c'est une section de réimplantation
        if (sectionHeader[i].sh_type == 9) {    
            fseek(f, 0L, SEEK_END);   // Calcul de la taille du fichier


            size = ftell(f) - (sectionHeader[i].sh_offset + sectionHeader[i].sh_size);


            fseek(f, sectionHeader[i].sh_offset + sectionHeader[i].sh_size, SEEK_SET);
            char *buffer = malloc(size);
            fread(&buffer, size, 1, f);
            fseek(fdest, sectionHeader[i].sh_offset, SEEK_SET);
            fwrite(&buffer,size, 1, fdest);
            header.e_shnum--;
        }
        i++;
    }
}*/