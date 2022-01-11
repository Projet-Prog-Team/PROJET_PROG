#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	fprintf(stderr, "\nUsage:\n"
		"%s --source file [-h -S -s -r -x -a] [--dest file [-w -d --text addr --data addr]]\n\n"
		"Produce a display similar to arm-none-eabi-readelf and use same options\n"
		"-w copy source file to dest file\n"
		"-d copy source file to dest file with REL sections deleted\n"
		"--text addr --data addr change symbol table to match new addresses\n\n"
		, name);
}


int main(int argc, char *argv[]) {
    int opt;
    	struct option longopts[] = {
	    { "source", required_argument, 0, 200 },
		{ "dest", required_argument, 0, 201},
		{ "text", required_argument, 0, 202 },
		{ "data", required_argument, 0, 203 },
		{ NULL, 0, NULL, 0 }
	};

	FILE * f_source = NULL;
	FILE * f_dest = NULL;

	int i = 1;
	while (i < argc){
		if (strcmp(argv[i],"--dest")==0 && i+1<argc) {
			//printf("dest : %s\n",argv[i+1]);
			f_dest = fopen(argv[i+1],"w");
			i++;
		}else if (strcmp(argv[i],"--source")==0 && i+1<argc && access(argv[i+1], F_OK) == 0){
			//printf("source : %s\n",argv[i+1]);
			f_source = fopen(argv[i+1],"rb");
			i++;
		}
		i++;
	}

	if (f_source == NULL){
		printf("Le fichier source n'a pas été trouvé ou n'a pas pu être ouvert\n");
		usage(argv[0]);
		exit(1);
	}

    Elf32_Main ELF;
	TableauSectionReloc tabSecRel;
	tabSecRel.nbSections = 0;

	ELF.header = loadHeader(f_source);
	ELF.sectHeader = loadTabSectionHeader(f_source, &ELF);
	ELF.symTable = loadSymTable(f_source, &ELF);
	ELF.tabSecRel = tabSecRel;
	ELF.relTable = loadRelocTable(f_source, &ELF);
	ELF.sectionContent = loadSectionContent(f_source, &ELF);

	while ((opt = getopt_long(argc, argv, "hSsrxawd", longopts, NULL)) != -1) {
		switch(opt) {
			case 200:
				// f_source = fopen(optarg, "rb");
				// if (f_source == NULL) {
				// 	printf("Impossible d'ouvrir le fichier source\n");
				// 	exit(1);
				// }
				break;
			case 201:
				// f_dest = fopen(optarg, "w");
				// if (f_dest == NULL) {
				// 	printf("Impossible d'ouvrir le fichier destination\n");
				// 	exit(1);
				// }
				break;
			case 'h':
				printHeader(&ELF);
				break;
			case 'S':
				printSectionHeader(f_source, &ELF);
				break;
			case 's':
				printSymTable(f_source, &ELF);
				break;
			case 'r':
				printRelocTable(f_source, &ELF);
				break;
			case 'x':
				printSectionContent(&ELF);
				break;
			case 'a':
				printHeader(&ELF);
				printf("\n");
				printSectionHeader(f_source, &ELF);
				printf("\n");
				printSymTable(f_source, &ELF);
				printf("\n");
				printRelocTable(f_source, &ELF);
				printf("\n");
				break;
			case 'w':
				if (f_dest==NULL){
					printf("L'option -w nécessite un fichier destination\n");
					exit(1);
				}
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
				break;
			case 'd':
				if (f_dest==NULL){
					printf("L'option -d nécessite un fichier destination\n");
					exit(1);
				}
				deleteRel(&ELF);
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
				break;
			case 202:
				if (f_dest==NULL){
					printf("L'option --text addr nécessite un fichier destination");
					exit(1);
				}
				correctSymTable(f_source,&ELF, strtol(optarg,NULL,16), 0);
				correctABSReloc(&ELF);
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
				break;
			case 203:
				if (f_dest==NULL){
					printf("L'option --data addr nécessite un fichier destination");
					exit(1);
				}
				correctSymTable(f_source, &ELF, 0, strtol(optarg,NULL,16));
				correctABSReloc(&ELF);
				writeHeader(&ELF, f_dest);
				writeSectionContent(&ELF, f_dest);
				writeSectionHeader(&ELF, f_dest);
				break;
			default:
				fprintf(stderr, "Unrecognized option %c\n", opt);
				usage(argv[0]);
				exit(1);
		}
	}

	if (f_source!=NULL){
    	fclose(f_source);
	}

	if (f_dest!=NULL){
		fclose(f_dest);
	}

	for(int i = 0; i < ELF.header.e_shnum; i++) {
		free(ELF.sectionContent[i].section);
	}
	free(ELF.sectionContent);

	for(int i = 0; i < ELF.tabSecRel.nbSections; i++) {
		free(ELF.relTable[i]);
	}
	free(ELF.relTable);

	free(ELF.sectHeader);

	free(ELF.symTable);


	return 0;
}
