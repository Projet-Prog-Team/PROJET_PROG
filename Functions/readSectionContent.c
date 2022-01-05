#include "readSectionContent.h"

void readRawSectionContent(const char * f_source)
{
    Elf32_Ehdr header = *readHeader(f_source);
    FILE* file = fopen(f_source,"r");
    Elf32_Shdr* sect_header = loadTabSectionHeader(file, header);
    unsigned char temp[2];
    char * nom_section = malloc(sizeof(char) * 512);
    for(int i=0;i<header.e_shnum;i++)
    {
        if(sect_header[i].sh_type!=SHT_NOBITS)
        {
            fseek(file,sect_header[header.e_shstrndx].sh_offset + sect_header[i].sh_name, SEEK_SET); // set to the section name offset
            if (fscanf(file, "%s", nom_section) == EOF) {                   // name of the section
                printf("Err : no section name\n");
            };
            fseek(file, sect_header[i].sh_offset, SEEK_SET); //section offset
            printf("%s\n",nom_section);
            printf("->%07x\n",sect_header[i].sh_offset);    //offset + last digit is the offset of the byte in actual row
            for(int j=0;j<sect_header[i].sh_size;j++)
            {
                fread(temp,sizeof(unsigned char),1,file);  
                printf("%02x",temp[0]);
                if((j%8==0 || j%2==0)&&j!=0)    //adding some space
                    printf(" ");
                if((j%16==0)&&j!=0)
                    printf("\n");
            }
            printf("\n\n");
        }
    }
    free(nom_section);
    fclose(file);
}
