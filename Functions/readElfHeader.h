#ifndef __readElfHeader_h__
#define __readElfHeader_h__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>

Elf32_Ehdr * readHeaderAffichage(const char * file);

Elf32_Ehdr * readHeader(const char * file);

#endif

