#ifndef __readSectionContent_h__
#define __readSectionContent_h__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include "readSectionHeader.h"
#include "readElfHeader.h"
#include <elf.h>

void readRawSectionContent(const char * f_char);

#endif