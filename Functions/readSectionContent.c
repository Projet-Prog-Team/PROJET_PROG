#include "readSectionContent.h"

SectionContent * loadSectionContent(FILE * file, Elf32_Main * ELF){
    SectionContent  * content = malloc(sizeof(SectionContent));
    fseek(file, ELF->header.e_ehsize, SEEK_SET);
    for(int i = 0; i < ELF->header.e_shnum; i++) { 
        content = realloc(content, sizeof(SectionContent) * (i+1));
        content[i].size = 0;
        if (ELF->sectHeader[i].sh_size != 0){
            fseek(file, ELF->sectHeader[i].sh_offset, SEEK_SET);
            if (i < ELF->header.e_shnum - 1){
                int val = i+1;
                for (int j=0; j<ELF->header.e_shnum; j++){
                    if (ELF->sectHeader[j].sh_offset>ELF->sectHeader[i].sh_offset && ELF->sectHeader[j].sh_offset < ELF->sectHeader[val].sh_offset){
                        val = j;
                    }
                }
                int size = (ELF->sectHeader[i].sh_type == SHT_REL) ? ELF->sectHeader[i].sh_size : ELF->sectHeader[val].sh_offset - ELF->sectHeader[i].sh_offset;
                if (ELF->sectHeader[i].sh_type == SHT_STRTAB){
                }
                content[i].section = malloc(size);
                content[i].size = size;
                fread(content[i].section, size, 1, file);           // On lit le bloc dans le fichier source
            }else {
                content[i].section = malloc(ELF->header.e_shoff - ELF->sectHeader[i].sh_offset);
                content[i].size = ELF->header.e_shoff - ELF->sectHeader[i].sh_offset;
                fread(content[i].section, ELF->header.e_shoff - ELF->sectHeader[i].sh_offset, 1, file);           // On lit le bloc dans le fichier source
            }
        }
    }
    return content;
}



void printSectionContent(Elf32_Main * ELF){
    for (int i=0; i < ELF->header.e_shnum; i++){
        printf("Section %d: %d\n",i,ELF->sectionContent[i].size);
        for (int j=0; j < ELF->sectHeader[i].sh_size; j++){
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

void printSectionContentOne(Elf32_Main * ELF, int noSection){
    if (noSection >= ELF->header.e_shnum){
        printf("dump.");
    }else{
        printf("Section %d: %d\n",noSection,ELF->sectionContent[noSection].size);
        if (ELF->sectHeader[noSection].sh_size==0){
            printf("no data to dump.");
        }else{
            for (int j=0; j < ELF->sectHeader[noSection].sh_size; j++){
                if (j%16 == 0 && j!=0){
                    printf("\n");
                } else if (j%4 == 0 && j!=0){
                    printf(" ");
                }
                printf("%02x",ELF->sectionContent[noSection].section[j]);
            }
        }
        printf("\n\n"); 
    }
}