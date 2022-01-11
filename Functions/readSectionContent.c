#include "readSectionContent.h"

SectionContent * loadSectionContent(FILE * file, Elf32_Main * ELF){
    SectionContent  * content = malloc(sizeof(SectionContent));
    if (content == NULL) {
        printf("Error: Unable to allocate section content memory\n");
        exit(1);
    }
    fseek(file, ELF->header.e_ehsize, SEEK_SET);    // On se place au début des contenus de sections dans le fichier
    for(int i = 0; i < ELF->header.e_shnum; i++) {  // Pour chaque section
        content = realloc(content, sizeof(SectionContent) * (i+1)); // On ajoute une structure de contenu de section au tableau de structures
        content[i].size = 0;
        if (ELF->sectHeader[i].sh_size != 0){
            fseek(file, ELF->sectHeader[i].sh_offset, SEEK_SET);    // On se place au début du contenu de la section courante
            if (i < ELF->header.e_shnum - 1){                       // On n'est pas à la dernière section
                int val = i+1;
                for (int j=0; j<ELF->header.e_shnum; j++){          // A la sortie de la boucle, val contient l'indice de la prochaine section (car les sections ne sont pas toutes dans le bon ordre)
                    if (ELF->sectHeader[j].sh_offset>ELF->sectHeader[i].sh_offset && ELF->sectHeader[j].sh_offset < ELF->sectHeader[val].sh_offset){
                        val = j;
                    }
                }
                // Etant donné qu'il y à des données inactives entres les sections, la taille de la section ne suffit pas et il faut tout prendre jusq'a la prochaine section
                int size = (ELF->sectHeader[i].sh_type == SHT_REL) ? ELF->sectHeader[i].sh_size : ELF->sectHeader[val].sh_offset - ELF->sectHeader[i].sh_offset;
                content[i].section = malloc(size);
                if (content[i].section == NULL) {
                    printf("Error: Unable to allocate one section content memory\n");
                    exit(1);
                }
                content[i].size = size;
                if(fread(content[i].section, size, 1, file) != 1) {exit(1);}           // On lit le bloc dans le fichier source
            }else {                                                 // On est à la dernière section
                content[i].section = malloc(ELF->header.e_shoff - ELF->sectHeader[i].sh_offset);
                if (content[i].section == NULL) {
                    printf("Error: Unable to allocate one section content memory\n");
                    exit(1);
                }
                content[i].size = ELF->header.e_shoff - ELF->sectHeader[i].sh_offset;
                if(fread(content[i].section, ELF->header.e_shoff - ELF->sectHeader[i].sh_offset, 1, file) != 1) {exit(1);}           // On lit le bloc dans le fichier source
            }
        }
    }
    return content;
}

void printSectionContent(Elf32_Main * ELF){
    for (int i=0; i < ELF->header.e_shnum; i++){
        printf("Section %d: %d\n",i,ELF->sectionContent[i].size);
        for (int j=0; j < ELF->sectionContent[i].size; j++){
            if (j%16 == 0 && j!=0){
                printf("\n");
            } else if (j%4 == 0 && j!=0){
                printf(" ");
            }
            printf("%02x",ELF->sectionContent[i].section[j]);
        }
        printf("\n\n");
    }
}