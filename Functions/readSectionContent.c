#include "readSectionContent.h"

void printRawSectionContent(FILE *file, Elf32_Main * ELF)
{
    unsigned char temp[2];
    char * nom_section = malloc(sizeof(char) * 512);
    for(int i=0;i<ELF->header.e_shnum;i++)
    {
        if(ELF->sectHeader[i].sh_type!=SHT_NOBITS)
        {
            fseek(file,ELF->sectHeader[ELF->header.e_shstrndx].sh_offset + ELF->sectHeader[i].sh_name, SEEK_SET); // set to the section name offset
            if (fscanf(file, "%s", nom_section) == EOF) {                   // name of the section
                printf("Err : no section name\n");
            };
            fseek(file, ELF->sectHeader[i].sh_offset, SEEK_SET); //section offset
            printf("%s\n",nom_section);
            printf("->%07x\n",ELF->sectHeader[i].sh_offset);    //offset + last digit is the offset of the byte in actual row
            for(int j=0;j<ELF->sectHeader[i].sh_size;j++)
            {
                fread(temp,sizeof(unsigned char),1,file);  
                printf("%02x",temp[0]);
                if(j%2==0&&j!=0)    //adding some space
                    printf(" ");
                if((j%16==0)&&j!=0)
                    printf("\n");
            }
            printf("\n\n");
        }
    }
    free(nom_section);
}
