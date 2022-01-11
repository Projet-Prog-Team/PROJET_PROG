#include "readElfHeader.h"

Elf32_Ehdr loadHeader(FILE * elfFile){

    Elf32_Ehdr header;
    //First 4 bytes are magic number
    if (fread(&header.e_ident[EI_MAG0],1,1,elfFile) != 1) {exit(1);}
    if (fread(&header.e_ident[EI_MAG1],1,1,elfFile) !=1) {exit(1);}
    if (fread(&header.e_ident[EI_MAG2],1,1,elfFile) !=1) {exit(1);}
    if (fread(&header.e_ident[EI_MAG3],1,1,elfFile) !=1) {exit(1);}

    if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 || header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3) {
        printf("Error: Not an ELF file - it has the wrong magic bytes at the start\n");
        exit(1);
    }

    //32 or 64 bits
    if (fread(&header.e_ident[EI_CLASS],1,1,elfFile) !=1) {exit(1);}

    //Endianess
    if (fread(&header.e_ident[EI_DATA],1,1,elfFile) !=1) {exit(1);}

    //Version
    if (fread(&header.e_ident[EI_VERSION],1,1,elfFile) !=1) {exit(1);}

    //ABI
    if (fread(&header.e_ident[EI_OSABI],1,1,elfFile) !=1) {exit(1);}

    //ABI version
    if (fread(&header.e_ident[EI_ABIVERSION],1,1,elfFile) !=1) {exit(1);}

    //7 useless bytes
    if (fread(&header.e_ident[EI_PAD],7,1,elfFile) !=1) {exit(1);}

    //File type
    if (fread(&header.e_type,2,1,elfFile) !=1) {exit(1);}
    header.e_type=bswap_16(header.e_type);

    //Target machine
    if (fread(&header.e_machine,2,1,elfFile) !=1) {exit(1);}
    header.e_machine = bswap_16(header.e_machine);

    //Version
    if (fread(&header.e_version,4,1,elfFile) !=1) {exit(1);}
    header.e_version = bswap_32(header.e_version);

    //Entry point
    if (header.e_ident[EI_CLASS] == 1){
        if (fread(&header.e_entry,4,1,elfFile) !=1) {exit(1);}
        header.e_entry = bswap_32(header.e_entry);
    }else{
        if (fread(&header.e_entry,8,1,elfFile) !=1) {exit(1);}
        header.e_entry = bswap_64(header.e_entry);
    }

    //Program headers offset
    if (header.e_ident[EI_CLASS] == 1){
        if (fread(&header.e_phoff,4,1,elfFile) !=1) {exit(1);}
        header.e_phoff = bswap_32(header.e_phoff);
    }else{
        if (fread(&header.e_phoff,8,1,elfFile) !=1) {exit(1);}
        header.e_phoff = bswap_64(header.e_phoff);
    }

    //Section headers offset
    if (header.e_ident[EI_CLASS] == 1){
        if (fread(&header.e_shoff,4,1,elfFile) !=1) {exit(1);}
        header.e_shoff = bswap_32(header.e_shoff);
    }else{
        if (fread(&header.e_shoff,8,1,elfFile) !=1) {exit(1);}
        header.e_shoff = bswap_64(header.e_shoff);
    }

    //TODO Flags
    if (fread(&header.e_flags,4,1,elfFile) !=1) {exit(1);}
    header.e_flags = bswap_32(header.e_flags);

    //Size of headers
    if (fread(&header.e_ehsize,2,1,elfFile) !=1) {exit(1);}
    header.e_ehsize = bswap_16(header.e_ehsize);

    //Size of programm headers
    if (fread(&header.e_phentsize,2,1,elfFile) !=1) {exit(1);}
    header.e_phentsize = bswap_16(header.e_phentsize);

    //Number of programm headers
    if (fread(&header.e_phnum,2,1,elfFile) !=1) {exit(1);}
    header.e_phnum = bswap_16(header.e_phnum);

    //Size of section headers
    if (fread(&header.e_shentsize,2,1,elfFile) !=1) {exit(1);}
    header.e_shentsize = bswap_16(header.e_shentsize);

    //Number of section headers
    if (fread(&header.e_shnum,2,1,elfFile) !=1) {exit(1);}
    header.e_shnum = bswap_16(header.e_shnum);

    //Section header string table index
    if (fread(&header.e_shstrndx,2,1,elfFile) !=1) {exit(1);}
    header.e_shstrndx = bswap_16(header.e_shstrndx);

    return header;
}


void printHeader(Elf32_Main * ELF){
    printf("ELF Header:\n");

    //32 or 64 bits
    if(ELF->header.e_ident[EI_CLASS]==1){
        printf("\tClass:\tELF32\n");
    }else if(ELF->header.e_ident[EI_CLASS]==2){
        printf("\tClass:\tELF64\n");
    }

    //Endianess
    if(ELF->header.e_ident[EI_DATA]==1){
        printf("\tData:\t2's complement, little endian\n");
    }else if(ELF->header.e_ident[EI_DATA]==2){
        printf("\tData:\t2's complement, big endian\n");
    }else {
        printf("\tData:\tUnknown\n");
    }

    //Version
    printf("\tVersion:\t%d (current)\n",ELF->header.e_ident[EI_VERSION]);

    //ABI
    printf("\tOS/ABI:\t");
    switch(ELF->header.e_ident[EI_OSABI]){
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
    printf("\tABI Version:\t%d\n",ELF->header.e_ident[EI_ABIVERSION]);

    //File type
    switch (ELF->header.e_type){
        case 0:
            printf("\tType:\tNONE");
            break;
        case 1:
            printf("\tType:\tREL (Relocatable file)");
            break;
        case 2:
            printf("\tType:\tEXEC (Executable file)");
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
    switch (ELF->header.e_machine){
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
    if (ELF->header.e_version==0){
        printf("\tVersion:\t0x0");
    }else if (ELF->header.e_version==1){
        printf("\tVersion:\t0x%d",ELF->header.e_version);
    }else {
        printf("\tVersion:\tUnknown");
    }
    printf("\n");

    //Entry point
    printf("\tEntry point address:\t0x%x\n",ELF->header.e_entry);

    //Program headers offset
    printf("\tStart of program headers:\t%d (bytes into file)\n",ELF->header.e_phoff);

    //Section headers offset
    printf("\tStart of section headers:\t%d (bytes into file)\n",ELF->header.e_shoff);

    //TODO Flags
    //printf("\tFlags:\t%d\n",ELF->header.e_flags);

    //Size of headers
    printf("\tSize of this header:\t%d (bytes)\n",ELF->header.e_ehsize);

    //Size of programm headers
    printf("\tSize of program headers:\t%d (bytes)\n",ELF->header.e_phentsize);

    //Number of programm headers
    printf("\tNumber of program headers:\t%d\n",ELF->header.e_phnum);

    //Size of section headers
    printf("\tSize of section headers:\t%d (bytes)\n",ELF->header.e_shentsize);

    //Number of section headers
    printf("\tNumber of section headers:\t%d\n",ELF->header.e_shnum);

    //Section header string table index
    printf("\tSection header string table index:\t%d\n",ELF->header.e_shstrndx);
}








