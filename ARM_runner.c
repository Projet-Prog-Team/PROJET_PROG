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
//#include "Functions/renumSections.h"
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
		{ "header", required_argument, NULL, 'h' },
		{ "section", required_argument, NULL, 'S' },
		{ "symtab", required_argument, NULL, 's' },
		{ "reloc", required_argument, NULL, 'r' },
		{ "all", required_argument, NULL, 'a' },
		{ NULL, 0, NULL, 0 }
	};
    // TO DO : vérifier que les fichiers sont bien ouverts
    FILE * f_source = fopen(argv[2], "rb");
    // TO DO : vérifier que argv 3 existe ou pas avant d'ouvrir f_dest
    //FILE * f_dest = fopen(argv[3], "w");

    Elf32_Main ELF;
    ELF.header = loadHeader(f_source);
    rewind(f_source);
    ELF.sectHeader = loadTabSectionHeader(f_source, &ELF);
	ELF.symTable = loadSymTable(f_source, &ELF);
    TableauSectionReloc tabSecRel;
    tabSecRel.nbSections = 0;
    ELF.tabSecRel = tabSecRel;
	ELF.relTable = loadRelocTable(f_source, &ELF);
	
	while ((opt = getopt_long(argc, argv, "h:S:s:r:x:e:a", longopts, NULL)) != -1) {
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
			printRawSectionContent(f_source, &ELF);
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
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}

    fclose(f_source);

	free(ELF.sectHeader);
	free(ELF.symTable);
    for(int i = 0; i < tabSecRel.nbSections; i++) {
        free(ELF.relTable[i]);
    }
	free(ELF.relTable);
	
	
	return 0;
}
