#include "writeFile.h"

void writeHeader(Elf32_Main * ELF, FILE * fdest){
    fseek(fdest, 0, SEEK_SET);

    uint16_t temp16;
    uint32_t temp32;

    fwrite(&ELF->header.e_ident[EI_MAG0],1,1,fdest);
    fwrite(&ELF->header.e_ident[EI_MAG1],1,1,fdest);
    fwrite(&ELF->header.e_ident[EI_MAG2],1,1,fdest);
    fwrite(&ELF->header.e_ident[EI_MAG3],1,1,fdest);

    //32 or 64 bits
    fwrite(&ELF->header.e_ident[EI_CLASS],1,1,fdest);

    //Endianess
    fwrite(&ELF->header.e_ident[EI_DATA],1,1,fdest);

    //Version
    fwrite(&ELF->header.e_ident[EI_VERSION],1,1,fdest);

    //ABI
    fwrite(&ELF->header.e_ident[EI_OSABI],1,1,fdest);

    //ABI version
    fwrite(&ELF->header.e_ident[EI_ABIVERSION],1,1,fdest);

    //7 useless bytes
    fwrite(&ELF->header.e_ident[EI_PAD],7,1,fdest);

    //File type
    temp16 = bswap_16(ELF->header.e_type);
    fwrite(&temp16,2,1,fdest);
    

    //Target machine
    temp16 = bswap_16(ELF->header.e_machine);
    fwrite(&temp16,2,1,fdest);
    

    //Version
    temp32 = bswap_32(ELF->header.e_version);
    fwrite(&temp32,4,1,fdest);
    

    //Entry point
    if (ELF->header.e_ident[EI_CLASS] == 1){
        
        temp32 = bswap_32(ELF->header.e_entry);
        fwrite(&temp32,4,1,fdest);
        
    }else{
        fwrite(&ELF->header.e_entry,8,1,fdest);
    }

    //Program headers offset
    if (ELF->header.e_ident[EI_CLASS] == 1){
        temp32 = bswap_32(ELF->header.e_phoff);
        fwrite(&temp32,4,1,fdest); 
    }else{
        fwrite(&ELF->header.e_phoff,8,1,fdest);
    }

    //Section headers offset
    if (ELF->header.e_ident[EI_CLASS] == 1){
        temp32 = bswap_32(ELF->header.e_shoff);
        fwrite(&temp32,4,1,fdest);
    }else{
        fwrite(&ELF->header.e_shoff,8,1,fdest);
    }

    //Flags
    temp32 = bswap_32(ELF->header.e_flags);
    fwrite(&temp32,4,1,fdest);
    
    //Size of headers
    temp16 = bswap_16(ELF->header.e_ehsize);
    fwrite(&temp16,2,1,fdest);
    
    //Size of programm headers
    temp16 = bswap_16(ELF->header.e_phentsize);
    fwrite(&temp16,2,1,fdest);

    //Number of programm headers
    temp16 = bswap_16(ELF->header.e_phnum);
    fwrite(&temp16,2,1,fdest);

    //Size of section headers
    temp16 = bswap_16(ELF->header.e_shentsize);
    fwrite(&temp16,2,1,fdest);

    //Number of section headers
    temp16 = bswap_16(ELF->header.e_shnum);
    fwrite(&temp16,2,1,fdest);

    //Section header string table index
    temp16 = bswap_16(ELF->header.e_shstrndx);
    fwrite(&temp16,2,1,fdest);

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
                fwrite(&temp32, 4, 1, fdest);

                temp32 = bswap_32(ELF->symTable[j].st_value);
                fwrite(&temp32, 4, 1, fdest);
            
                temp32 = bswap_32(ELF->symTable[j].st_size);
                fwrite(&temp32, 4, 1, fdest);

                fwrite(&ELF->symTable[j].st_info, 1, 1, fdest);
                fwrite(&ELF->symTable[j].st_other, 1, 1, fdest);

                temp16 = bswap_16(ELF->symTable[j].st_shndx);
                fwrite(&temp16, 2, 1, fdest);
            }
        }else{
            fwrite(ELF->sectionContent[tab[i].indice].section,ELF->sectionContent[tab[i].indice].size,1,fdest);
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

