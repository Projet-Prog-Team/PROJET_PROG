#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>


Elf32_Ehdr * readHeader(const char * file){
    printf("ELF Header:\n");

    FILE * elfFile = fopen(file,"r");
    Elf32_Ehdr * header = malloc(sizeof(Elf32_Ehdr));

    //First 4 bytes are magic number
    fread(&header->e_ident[EI_MAG0],1,1,elfFile);
    fread(&header->e_ident[EI_MAG1],1,1,elfFile);
    fread(&header->e_ident[EI_MAG2],1,1,elfFile);
    fread(&header->e_ident[EI_MAG3],1,1,elfFile);

    //32 or 64 bits
    fread(&header->e_ident[EI_CLASS],1,1,elfFile);
    if(header->e_ident[EI_CLASS]==1){
        printf("\tClass:\tELF32\n");
    }else if(header->e_ident[EI_CLASS]==2){
        printf("\tClass:\tELF64\n");
    }

    //Endianess
    fread(&header->e_ident[EI_DATA],1,1,elfFile);
    if(header->e_ident[EI_DATA]==1){
        printf("\tData:\t2's complement, litle endian\n");
    }else if(header->e_ident[EI_DATA]==2){
        printf("\tData:\t2's complement, big endian\n");
    }else {
        printf("\tData:\tUnknown\n");
    }

    //Version
    fread(&header->e_ident[EI_VERSION],1,1,elfFile);
    printf("\tVersion:\t%d (current)\n",header->e_ident[EI_VERSION]);

    //ABI
    fread(&header->e_ident[EI_OSABI],1,1,elfFile);
    printf("\tOS/ABI:\t");
    switch(header->e_ident[EI_OSABI]){
        case 0:
            printf("UNIX - System V");
            break;
        case 1:
            printf("HP - UX");
            break;
        case 2:
            printf("NetBSD");
            break;
        case 3:
            printf("Linux");
            break;
        case 6:
            printf("Sun Solaris");
            break;
        case 7:
            printf("IBM AIX");
            break;
        case 8:
            printf("SGI Irix");
            break;
        case 9:
            printf("FreeBSD");
            break;
        case 10:
            printf("Compaq TRU64");
            break;
        case 11:
            printf("Novell Modesto");
            break;
        case 12:
            printf("OpenBSD");
            break;
        case 64:
            printf("ARM EABI");
            break;
        case 97:
            printf("ARM");
            break;
        case 255:
            printf("Standalone");
            break;
        default:
            printf("Unknown");
    }
    printf("\n");

    //ABI version
    fread(&header->e_ident[EI_ABIVERSION],1,1,elfFile);
    printf("\tABI Version:\t%d\n",header->e_ident[EI_ABIVERSION]);

    //7 useless bytes
    fread(&header->e_ident[EI_PAD],7,1,elfFile);

    //File type
    fread(&header->e_type,2,1,elfFile);
    header->e_type=bswap_16(header->e_type);
    switch (header->e_type){
        case 0:
            printf("\tType:\tNONE");
            break;
        case 1:
            printf("\tType:\tREL (Relocatable file)");
            break;
        case 2:
            printf("\tType:\tEXEC");
            break;
        case 3:
            printf("\tType:\tDYN");
            break;
        case 4:
            printf("\tType:\tCORE");
            break;
        default:
            printf("\tType:\tUnknown");
    }
    printf("\n");

    //Target machine
    fread(&header->e_machine,2,1,elfFile);
    header->e_machine = bswap_16(header->e_machine);
    switch (header->e_machine){
        case 0:
            printf("\tMachine:\tNone");
            break;
        case 2:
            printf("\tMachine:\tSPARC");
            break;
        case 3:
            printf("\tMachine:\tx86");
            break;
        case 4:
            printf("\tMachine:\tMotorola 68000");
            break;
        case 40:
            printf("\tMachine:\tARM");
            break;
        default:
            printf("\tMachine:\tUnknown");
    }
    printf("\n");

    //Version
    fread(&header->e_version,4,1,elfFile);
    header->e_version = bswap_32(header->e_version);
    if (header->e_version==0){
        printf("\tVersion:\t0x0");
    }else if (header->e_version==1){
        printf("\tVersion:\t0x%d",header->e_version);
    }else {
        printf("\tVersion:\tUnknown");
    }
    printf("\n");

    //Entry point
    if (header->e_ident[EI_CLASS] == 1){
        fread(&header->e_entry,4,1,elfFile);
        header->e_entry = bswap_32(header->e_entry);
    }else{
        fread(&header->e_entry,8,1,elfFile);
        header->e_entry = bswap_64(header->e_entry);
    }
    printf("\tEntry point address:\t0x%d\n",header->e_entry);

    //Program headers offset
    if (header->e_ident[EI_CLASS] == 1){
        fread(&header->e_phoff,4,1,elfFile);
        header->e_phoff = bswap_32(header->e_phoff);
    }else{
        fread(&header->e_phoff,8,1,elfFile);
        header->e_phoff = bswap_64(header->e_phoff);
    }
    printf("\tStart of program headers:\t%d (bytes into file)\n",header->e_phoff);

    //Section headers offset
    if (header->e_ident[EI_CLASS] == 1){
        fread(&header->e_shoff,4,1,elfFile);
        header->e_shoff = bswap_32(header->e_shoff);
    }else{
        fread(&header->e_shoff,8,1,elfFile);
        header->e_shoff = bswap_64(header->e_shoff);
    }
    printf("\tStart of section headers:\t%d (bytes into file)\n",header->e_shoff);

    //TODO Flags
    fread(&header->e_flags,4,1,elfFile);
    header->e_flags = bswap_32(header->e_flags);
    //printf("\tFlags:\t%d\n",header->e_flags);

    //Size of headers
    fread(&header->e_ehsize,2,1,elfFile);
    header->e_ehsize = bswap_16(header->e_ehsize);
    printf("\tSize of this header:\t%d (bytes)\n",header->e_ehsize);

    //Size of programm headers
    fread(&header->e_phentsize,2,1,elfFile);
    header->e_phentsize = bswap_16(header->e_phentsize);
    printf("\tSize of program headers:\t%d (bytes)\n",header->e_phentsize);

    //Number of programm headers
    fread(&header->e_phnum,2,1,elfFile);
    header->e_phnum = bswap_16(header->e_phnum);
    printf("\tNumber of program headers:\t%d\n",header->e_phnum);

    //Size of section headers
    fread(&header->e_shentsize,2,1,elfFile);
    header->e_shentsize = bswap_16(header->e_shentsize);
    printf("\tSize of section headers:\t%d (bytes)\n",header->e_shentsize);

    //Number of section headers
    fread(&header->e_shnum,2,1,elfFile);
    header->e_shnum = bswap_16(header->e_shnum);
    printf("\tNumber of section headers:\t%d\n",header->e_shnum);

    //Section header string table index
    fread(&header->e_shstrndx,2,1,elfFile);
    header->e_shstrndx = bswap_16(header->e_shstrndx);
    printf("\tSection header string table index:\t%d\n",header->e_shstrndx);

    fclose(elfFile);

    return header;
}





