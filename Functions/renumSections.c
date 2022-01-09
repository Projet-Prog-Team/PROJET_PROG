#include "renumSections.h"

void writeHeader(const char * f_source, const char * f_dest, Elf32_Ehdr * header) {   // Pour pouvoir écrire le header en "un coup" il nous faut un pointeur vers la structure header
    FILE* fsource = fopen(f_source,"r");
    FILE* fdest = fopen(f_dest,"w");
    
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

// void writeSectionContentFile(const char * f_source, const char * f_dest, Elf32_Ehdr header, Elf32_Shdr* sect_header, Elf32_Sym * symTab, int nb_symboles, int offset) { 
//     FILE* fsource = fopen(f_source,"r");
//     FILE* fdest = fopen(f_dest,"a");
//     char * buffer = malloc(sect_header[0].sh_size);             // Taille quelconque
//     uint16_t temp16;
//     uint32_t temp32;
//     fseek(fsource, header.e_ehsize, SEEK_SET);
//     for(int i = 0; i < header.e_shnum; i++) {                   // Parcours de chaque section
//         if (sect_header[i].sh_size != 0){
//             if (i < header.e_shnum - 1){
//                 int val = i+1;
//                 for (int j=0; j<header.e_shnum; j++){
//                     if (sect_header[j].sh_offset>sect_header[i].sh_offset && sect_header[j].sh_offset < sect_header[val].sh_offset){
//                         val = j;
//                     }
//                 }
//                 if (sect_header[i].sh_type == SHT_SYMTAB){
//                     printf("Test\n");
//                     for (int k = 0; k < nb_symboles; k++) {
//                         temp32 = bswap_32(symTab[k].st_name);
//                         fwrite(&temp32, 4, 1, fdest);
//                         temp32 = bswap_32(symTab[k].st_value);
//                         fwrite(&temp32, 4, 1, fdest);
//                         temp32 = bswap_32(symTab[k].st_size);
//                         fwrite(&temp32, 4, 1, fdest);
//                         fwrite(&(symTab[k].st_info), 1, 1, fdest);
//                         fwrite(&(symTab[k].st_other), 1, 1, fdest);
//                         temp16 = bswap_16(symTab[k].st_shndx);
//                         fwrite(&temp16, 2, 1, fdest);
//                     }
//                     int size = sect_header[val].sh_offset - (nb_symboles * 16 + sect_header[i].sh_offset);
//                     printf("size : %d\n",size);
//                     buffer = realloc(buffer, size);
//                     fread(buffer, size, 1, fsource);           // On lit le bloc dans le fichier source
//                     fwrite(buffer, size, 1, fdest);  
//                     fseek(fsource, sect_header[val].sh_offset, SEEK_SET);
//                 }else {
//                     int size = sect_header[i].sh_type == SHT_REL ? sect_header[i].sh_size : sect_header[val].sh_offset - sect_header[i].sh_offset;
//                     buffer = realloc(buffer, size);
//                     fread(buffer, size, 1, fsource);           // On lit le bloc dans le fichier source
//                     fwrite(buffer, size, 1, fdest);           // On écrit le bloc dans le fichier destination
//                 }
//             }else {
//                 buffer = realloc(buffer, offset - sect_header[i].sh_offset);
//                 fread(buffer, offset - sect_header[i].sh_offset, 1, fsource);           // On lit le bloc dans le fichier source
//                 fwrite(buffer, offset - sect_header[i].sh_offset, 1, fdest);           // On écrit le bloc dans le fichier destination
//             }

//         }
//     }
//     free(buffer);
//     fclose(fdest);
//     fclose(fsource);
// }

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

Elf32_Shdr * deleteRel(Elf32_Ehdr * header, Elf32_Shdr* sect_header, Elf32_Sym * symTab, int nb_symboles, int * offset){
    int i=0;
    *offset = header->e_shoff;
    Elf32_Shdr * new_sect_header = malloc(header->e_shnum*40);
    memmove(new_sect_header, sect_header, header->e_shnum*40);
    FILE* fsource = fopen("Examples_loader/example2.o","r");
    uint32_t sh_offset = header->e_shoff
    printSectionHeader(sect_header, *header, fsource);
    while(i < header->e_shnum){
        if (new_sect_header[i].sh_type==SHT_REL){
            if (header->e_shstrndx > i ) {
                header->e_shstrndx--;
            }
            header->e_shoff -= new_sect_header[i].sh_size;
            int sizeRel = new_sect_header[i].sh_size;
            int offsetRel = new_sect_header[i].sh_offset;
            for(int j=i+1; j<header->e_shnum; j++){
                new_sect_header[j-1] = new_sect_header[j];
            }
            for(int k=0; k < header->e_shnum; k++){
                if (new_sect_header[k].sh_offset > offsetRel){
                    new_sect_header[k].sh_offset -= sizeRel;
                    printf("%d Offset : %d\n",i,new_sect_header[k].sh_offset);
                }
                int linkCondition = new_sect_header[k].sh_type == SHT_HASH;
                linkCondition ||= new_sect_header[k].sh_type == SHT_DYNAMIC;
                // LINK modif
                if (linkCondition && new_sect_header[k].sh_link > i){
                    new_sect_header[k].sh_link--;
                }
            }
            //NDX modif 
            // for (int l=0; l< nb_symboles ;l++){
            //     if (symTab[l].st_shndx >= i){
            //         symTab[l].st_shndx--;
            //     }
            // }
            header->e_shnum--;
        }else{
            i++;
        }
    }
    printf("\n\n\n\n");
    printSectionHeader(new_sect_header, *header, fsource);
    return new_sect_header;
}

void renumSect(const char * f_source, const char * f_dest) {
    Elf32_Ehdr* header = readHeader(f_source);
    Elf32_Shdr* sect_header = readSectionsHeader(f_source, 0);
    int nb_symboles = 0;
    FILE *fsource = fopen(f_source, "rb");
    if (fsource == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return;
    }
    Elf32_Sym* symTab = loadSymTable(fsource, sect_header, *header, &nb_symboles);
    int val;
    writeHeader(f_source, f_dest, header);
    writeSectionContentFile(f_source, f_dest, *header, sect_header, symTab, nb_symboles, val);
    writeSectionHeader(f_source, f_dest, *header, new_sect_header);
    fclose(fsource);
}