#include "writeFile.h"

void writeHeader(Elf32_Main * ELF, FILE * fdest){
    fseek(fdest, 0, SEEK_SET);

    uint16_t temp16;
    uint32_t temp32;

    if (fwrite(&ELF->header.e_ident[EI_MAG0],1,1,fdest) != 1) {exit(1);}
    if (fwrite(&ELF->header.e_ident[EI_MAG1],1,1,fdest) != 1) {exit(1);}
    if (fwrite(&ELF->header.e_ident[EI_MAG2],1,1,fdest) != 1) {exit(1);}
    if (fwrite(&ELF->header.e_ident[EI_MAG3],1,1,fdest) != 1) {exit(1);}

    //32 or 64 bits
    if (fwrite(&ELF->header.e_ident[EI_CLASS],1,1,fdest) != 1) {exit(1);}

    //Endianess
    if (fwrite(&ELF->header.e_ident[EI_DATA],1,1,fdest) != 1) {exit(1);}

    //Version
    if (fwrite(&ELF->header.e_ident[EI_VERSION],1,1,fdest) != 1) {exit(1);}

    //ABI
    if (fwrite(&ELF->header.e_ident[EI_OSABI],1,1,fdest) != 1) {exit(1);}

    //ABI version
    if (fwrite(&ELF->header.e_ident[EI_ABIVERSION],1,1,fdest) != 1) {exit(1);}

    //7 useless bytes
    if (fwrite(&ELF->header.e_ident[EI_PAD],7,1,fdest) != 1) {exit(1);}

    //File type
    temp16 = bswap_16(ELF->header.e_type);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}
    

    //Target machine
    temp16 = bswap_16(ELF->header.e_machine);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}
    

    //Version
    temp32 = bswap_32(ELF->header.e_version);
    if (fwrite(&temp32,4,1,fdest) != 1) {exit(1);}
    

    //Entry point
    if (ELF->header.e_ident[EI_CLASS] == 1){
        
        temp32 = bswap_32(ELF->header.e_entry);
        if (fwrite(&temp32,4,1,fdest) != 1) {exit(1);}
        
    }else{
        if (fwrite(&ELF->header.e_entry,8,1,fdest) != 1) {exit(1);}
    }

    //Program headers offset
    if (ELF->header.e_ident[EI_CLASS] == 1){
        temp32 = bswap_32(ELF->header.e_phoff);
        if (fwrite(&temp32,4,1,fdest) != 1) {exit(1);}
    }else{
        if (fwrite(&ELF->header.e_phoff,8,1,fdest) != 1) {exit(1);}
    }

    //Section headers offset
    if (ELF->header.e_ident[EI_CLASS] == 1){
        temp32 = bswap_32(ELF->header.e_shoff);
        if (fwrite(&temp32,4,1,fdest) != 1) {exit(1);}
    }else{
        if (fwrite(&ELF->header.e_shoff,8,1,fdest) != 1) {exit(1);}
    }

    //Flags
    temp32 = bswap_32(ELF->header.e_flags);
    if (fwrite(&temp32,4,1,fdest) != 1) {exit(1);}
    
    //Size of headers
    temp16 = bswap_16(ELF->header.e_ehsize);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}
    
    //Size of programm headers
    temp16 = bswap_16(ELF->header.e_phentsize);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}

    //Number of programm headers
    temp16 = bswap_16(ELF->header.e_phnum);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}

    //Size of section headers
    temp16 = bswap_16(ELF->header.e_shentsize);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}

    //Number of section headers
    temp16 = bswap_16(ELF->header.e_shnum);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}

    //Section header string table index
    temp16 = bswap_16(ELF->header.e_shstrndx);
    if (fwrite(&temp16,2,1,fdest) != 1) {exit(1);}

}

int compare( const void* a, const void* b)
{
    tempTab e1 = * ( (tempTab*) a );
    tempTab e2 = * ( (tempTab*) b );

    if ( e1.offset == e2.offset ) return 0;
    else if ( e1.offset < e2.offset ) return -1;
    else return 1;
}

void writeSectionContent(Elf32_Main * ELF, FILE * fdest){
    uint16_t temp16;
    uint32_t temp32;

    tempTab * tab = malloc(sizeof(tempTab)*ELF->header.e_shnum);

    for (int i=0; i < ELF->header.e_shnum; i++){
        tempTab elem;
        elem.indice =  i;
        elem.offset = ELF->sectHeader[i].sh_offset;
        tab[i] = elem;
    }

    qsort(tab, ELF->header.e_shnum, sizeof(tempTab), compare);

    for (int i=0; i < ELF->header.e_shnum; i++){
        if(ELF->sectHeader[tab[i].indice].sh_type==SHT_SYMTAB){
            for (int j = 0; j < ELF->nb_symboles; j++) {
                temp32 = bswap_32(ELF->symTable[j].st_name);
                if (fwrite(&temp32, 4, 1, fdest) != 1) {exit(1);}

                temp32 = bswap_32(ELF->symTable[j].st_value);
                if (fwrite(&temp32, 4, 1, fdest) != 1) {exit(1);}
            
                temp32 = bswap_32(ELF->symTable[j].st_size);
                if (fwrite(&temp32, 4, 1, fdest) != 1) {exit(1);}

                if (fwrite(&ELF->symTable[j].st_info, 1, 1, fdest) != 1) {exit(1);}
                if (fwrite(&ELF->symTable[j].st_other, 1, 1, fdest) != 1) {exit(1);}

                temp16 = bswap_16(ELF->symTable[j].st_shndx);
                if (fwrite(&temp16, 2, 1, fdest) != 1) {exit(1);}
            }
        } else{
            if (ELF->sectionContent[tab[i].indice].size != 0) {
                if (fwrite(ELF->sectionContent[tab[i].indice].section, ELF->sectionContent[tab[i].indice].size, 1, fdest) != 1) {exit(1);}
            }
        }
    }
    free(tab);
}

void writeSectionHeader(Elf32_Main * ELF, FILE * fdest) {
    Elf32_Word * temp = (Elf32_Word *) ELF->sectHeader;    // temp est l'adresse de l'attribut j de la section i
    for(int i = 0; i < ELF->header.e_shnum; i++) {   // Parcours de chaque section
        for(int j = 0; j < 10; j++){ 
            Elf32_Word temp2 = *temp;                                // Pour chaque mot (32 bits)
            temp2 = bswap_32(temp2);                           // Inversion du boutisme
            fwrite(&temp2, 4, 1, fdest);
            temp++;
        }
    }
}

