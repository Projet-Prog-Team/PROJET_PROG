#include "renumSections.h"

void writeHeader(const char * f_source, const char * f_dest, Elf32_Ehdr * header) {   // Pour pouvoir écrire le header en "un coup" il nous faut un pointeur vers la structure header
    FILE* fsource = fopen(f_source,"r");
    FILE* fdest = fopen(f_dest,"w");
    
    //char * headerTab[header.e_ehsize];
    // fread(headerTab,header.e_ehsize,1,fsource)
    //fwrite(header,header->e_ehsize,1,fdest);
    uint16_t temp16;
    uint32_t temp32;

    fwrite(&header->e_ident[EI_MAG0],1,1,fdest);
    fwrite(&header->e_ident[EI_MAG1],1,1,fdest);
    fwrite(&header->e_ident[EI_MAG2],1,1,fdest);
    fwrite(&header->e_ident[EI_MAG3],1,1,fdest);

    //32 or 64 bits
    fwrite(&header->e_ident[EI_CLASS],1,1,fdest);

    //Endianess
    fwrite(&header->e_ident[EI_DATA],1,1,fdest);

    //Version
    fwrite(&header->e_ident[EI_VERSION],1,1,fdest);

    //ABI
    fwrite(&header->e_ident[EI_OSABI],1,1,fdest);

    //ABI version
    fwrite(&header->e_ident[EI_ABIVERSION],1,1,fdest);

    //7 useless bytes
    fwrite(&header->e_ident[EI_PAD],7,1,fdest);

    //File type
    temp16 = bswap_16(header->e_type);
    fwrite(&temp16,2,1,fdest);
    

    //Target machine
    temp16 = bswap_16(header->e_machine);
    fwrite(&temp16,2,1,fdest);
    

    //Version
    temp32 = bswap_32(header->e_version);
    fwrite(&temp32,4,1,fdest);
    

    //Entry point
    if (header->e_ident[EI_CLASS] == 1){
        
        temp32 = bswap_32(header->e_entry);
        fwrite(&temp32,4,1,fdest);
        
    }else{
        fwrite(&header->e_entry,8,1,fdest);
    }

    //Program headers offset
    if (header->e_ident[EI_CLASS] == 1){
        temp32 = bswap_32(header->e_phoff);
        fwrite(&temp32,4,1,fdest); 
    }else{
        fwrite(&header->e_phoff,8,1,fdest);
    }

    //Section headers offset
    if (header->e_ident[EI_CLASS] == 1){
        temp32 = bswap_32(header->e_shoff);
        fwrite(&temp32,4,1,fdest);
    }else{
        fwrite(&header->e_shoff,8,1,fdest);
    }

    //Flags
    temp32 = bswap_32(header->e_flags);
    fwrite(&temp32,4,1,fdest);
    
    //Size of headers
    temp16 = bswap_16(header->e_ehsize);
    fwrite(&temp16,2,1,fdest);
    
    //Size of programm headers
    temp16 = bswap_16(header->e_phentsize);
    fwrite(&temp16,2,1,fdest);

    //Number of programm headers
    temp16 = bswap_16(header->e_phnum);
    fwrite(&temp16,2,1,fdest);

    //Size of section headers
    temp16 = bswap_16(header->e_shentsize);
    fwrite(&temp16,2,1,fdest);

    //Number of section headers
    temp16 = bswap_16(header->e_shnum);
    fwrite(&temp16,2,1,fdest);

    //Section header string table index
    temp16 = bswap_16(header->e_shstrndx);
    fwrite(&temp16,2,1,fdest);

    fclose(fdest);
    fclose(fsource);

}

void writeSectionContentFile(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* sect_header) { 
    FILE* fsource = fopen(f_source,"r");
    FILE* fdest = fopen(f_dest,"a");
    Elf32_Word * buffer = malloc(sect_header[0].sh_size);             // Taille quelconque
    //char c = 0;
    for(int i = 0; i < header.e_shnum; i++) {                        // Parcours de chaque section
        fseek(fsource, sect_header[i].sh_offset, SEEK_SET);
        buffer = realloc(buffer, sect_header[i].sh_size);            // On alloue un bloc de la taille de la section a ecrire
        fread(buffer, sect_header[i].sh_size, 1, fsource);           // On lit le bloc dans le fichier source
        fwrite(buffer, sect_header[i].sh_size, 1, fdest);            // On écrit le bloc dans le fichier destination
        /*if (i < header.e_shnum - 1) {
            while(ftell(fdest) < sect_header[i+1].sh_offset) {     
                fputc(c, fdest);
            }
        }*/
    }
    free(buffer);
    fclose(fdest);
    fclose(fsource);
}

void writeSectionHeader(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* sect_header) {
    FILE* fsource = fopen(f_source,"r");
    FILE* fdest = fopen(f_dest,"a");
    Elf32_Word * temp = (Elf32_Word *) sect_header;    // temp est l'adresse de l'attribut j de la section i
    for(int i = 0; i < header.e_shnum; i++) {   // Parcours de chaque section
        for(int j = 0; j < 10; j++){                                 // Pour chaque mot (32 bits)
            *temp = bswap_32(*temp);                           // Inversion du boutisme
            fwrite(temp, 4, 1, fdest);
            temp++;
        }
    }
    fclose(fdest);
    fclose(fsource);
}

void deleteRel(Elf32_Ehdr * header, Elf32_Shdr* sect_header){
    int i=0;
    while(i < header->e_shnum){
        if (sect_header[i].sh_type==SHT_REL){
            header->e_shstrndx--;
            header->e_shoff -= header->e_shentsize + sect_header[i].sh_size;
            int sizeRel = sect_header[i].sh_size;
            int offsetRel = sect_header[i].sh_offset;
            for(int j=i+1; j<header->e_shnum; j++){
                sect_header[j-1] = sect_header[j];
            }
            for(int k=0; k < header->e_shnum; k++){
                if (sect_header[k].sh_offset > offsetRel){
                    sect_header[k].sh_offset -= sizeRel;
                }
            }
            header->e_shnum--;
        }else{
            // printf("\nStruct %d : \n", i);
            // printf("name : %d\n", sect_header[i].sh_name);
            // printf("type : %d\n", sect_header[i].sh_type);
            // printf("flags : %d\n", sect_header[i].sh_flags);
            // printf("addr : %d\n", sect_header[i].sh_addr);
            // printf("offset : %d\n", sect_header[i].sh_offset);
            // printf("size : %d\n", sect_header[i].sh_size);
            // printf("link : %d\n", sect_header[i].sh_link);
            // printf("info : %d\n", sect_header[i].sh_info);
            // printf("addralign : %d\n", sect_header[i].sh_addralign);
            // printf("entsize : %d\n", sect_header[i].sh_entsize);
            // printf("\n\n");
            i++;
        }
    }
}

void renumSect(const char * f_source, const char * f_dest) {
    Elf32_Ehdr header = *readHeader(f_source);
    Elf32_Shdr* sect_header = readSectionsHeader(f_source, 0);
    
    deleteRel(&header, sect_header);
    writeHeader(f_source, f_dest, &header);
    writeSectionContentFile(f_source, f_dest, header, sect_header);
    writeSectionHeader(f_source, f_dest, header, sect_header);
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