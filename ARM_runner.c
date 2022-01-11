#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "arm_simulator_interface.h"
#include "Functions/readSectionHeader.h"
#include "Functions/readSectionContent.h"

#include "Functions/readElfHeader.h"
#include "Functions/readSymTable.h"
#include "Functions/readRelocTable.h"
#include "Functions/writeFile.h"
#include "Functions/implantation.h"
#include "structure.h"


void usage(char *name) {
	fprintf(stderr, "Usage:\n"
		"%s [ --help ] [ --host hostname ] [ --service servicename ] [ --debug file ] file\n\n"
		"Loads a sample ARM code to a remote simulator. The --debug flag enables the output produced by "
		"calls to the debug function in the named source file.\n" 
		, name);
}


int main(int argc, char *argv[]) {
    int opt;
    	struct option longopts[] = {
		{ "header", 0, NULL, 'h' },
		{ "section", required_argument, NULL, 'S' },
		{ "symtab", required_argument, NULL, 's' },
		{ "reloc", required_argument, NULL, 'r' },
		{ "all", required_argument, NULL, 'a' },
		{ "text", required_argument, NULL, 't' },
		{ "data", required_argument, NULL, 'j' },
		{ NULL, 0, NULL, 0 }
	};

    FILE * f_source = fopen(argv[1], "rb");
	if (f_source == NULL) {
		printf("Impossible d'ouvrir le fichier source\n");
		return 1;
	}
	FILE * f_dest;
	if(argc>=3) {
		f_dest = fopen(argv[2], "w");
		if (f_dest == NULL) {
			printf("Impossible d'ouvrir le fichier destination\n");
			return 1;
		}
	} 


    Elf32_Main ELF;
    ELF.header = loadHeader(f_source);
    ELF.sectHeader = loadTabSectionHeader(f_source, &ELF);
	ELF.symTable = loadSymTable(f_source, &ELF);
    TableauSectionReloc tabSecRel;
    tabSecRel.nbSections = 0;
    ELF.tabSecRel = tabSecRel;
	ELF.relTable = loadRelocTable(f_source, &ELF);
	ELF.sectionContent = loadSectionContent(f_source, &ELF);

	

	while ((opt = getopt_long(argc, argv, "h:S:s:r:x:e:a:w:d", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			printHeader(&ELF);
			exit(0);
		case 'S':
			printSectionHeader(f_source, &ELF);
			exit(0);
		case 's':
			printSymTable(f_source, &ELF);
			exit(0);
		case 'r':
			printRelocTable(f_source, &ELF);
			exit(0);
		case 'x':
			printSectionContent(&ELF);
			exit(0);
		case 'w':
			if(argc>=3){
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
			}
			exit(0);
		case 'd':
			if(argc>=3){
				deleteRel(&ELF);
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
			}
			exit(0);
		case 'a':
			printHeader(&ELF);
			printf("\n");
			printSectionHeader(f_source, &ELF);
			printf("\n");
			printSymTable(f_source, &ELF);
			printf("\n");
			printRelocTable(f_source, &ELF);
			printf("\n");
			exit(0);
		case 't':
			deleteRel(&ELF);
			correctSymTable(f_source,&ELF, strtol(optarg,NULL,16), 0);
			correctABSReloc(&ELF);
			if(argc>=3){
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
			}
			break;
		case 'j':
			correctSymTable(f_source, &ELF, 0, strtol(optarg,NULL,16));
			if(argc>=3){
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
			}
			break;
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}

    fclose(f_source);
	if(argc>=3){
		fclose(f_dest);
	}

	free(ELF.sectHeader);
	free(ELF.symTable);
    for(int i = 0; i < tabSecRel.nbSections; i++) {
        free(ELF.relTable[i]);
    }
	free(ELF.relTable);
	
	
	return 0;
}
