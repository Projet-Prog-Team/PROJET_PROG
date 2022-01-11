#include "readSectionContent.h"

// void printRawSectionContent(FILE *file, Elf32_Main * ELF)
// {
//     unsigned char temp[2];
//     char * nom_section = malloc(sizeof(char) * 512);
//     for(int i=0;i<ELF->header.e_shnum;i++)
//     {
//         if(ELF->sectHeader[i].sh_type!=SHT_NOBITS)
//         {
//             fseek(file,ELF->sectHeader[ELF->header.e_shstrndx].sh_offset + ELF->sectHeader[i].sh_name, SEEK_SET); // set to the section name offset
//             if (fscanf(file, "%s", nom_section) == EOF) {                   // name of the section
//                 printf("Err : no section name\n");
//             };
//             fseek(file, ELF->sectHeader[i].sh_offset, SEEK_SET); //section offset
//             printf("%s\n",nom_section);
//             printf("->%07x\n",ELF->sectHeader[i].sh_offset);    //offset + last digit is the offset of the byte in actual row
//             for(int j=0;j<ELF->sectHeader[i].sh_size;j++)
//             {
//                 fread(temp,sizeof(unsigned char),1,file);  
//                 printf("%02x",temp[0]);
//                 if((j%8==0 || j%2==0)&&j!=0)    //adding some space
//                     printf(" ");
//                 if((j%16==0)&&j!=0)
//                     printf("\n");
//             }
//             printf("\n\n");
//         }
//     }
//     free(nom_section);
// }

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