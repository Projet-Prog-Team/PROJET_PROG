#include "implantation.h"


void deleteRel(Elf32_Main * ELF){
    int i=0;
    while(i < ELF->header.e_shnum){
        if (ELF->sectHeader[i].sh_type==SHT_REL){
            if (ELF->header.e_shstrndx>i){
                ELF->header.e_shstrndx--;
            }
            ELF->header.e_shoff -= ELF->sectHeader[i].sh_size;
            int sizeRel = ELF->sectHeader[i].sh_size;
            int offsetRel = ELF->sectHeader[i].sh_offset;
            //DELETE REL from sectionHeader
            for(int j=i+1; j < ELF->header.e_shnum; j++){
                ELF->sectHeader[j-1] = ELF->sectHeader[j];
            }
            //DELETE REL from sectionContent
            for(int j=i+1; j < ELF->header.e_shnum; j++){
                ELF->sectionContent[j-1].size = ELF->sectionContent[j].size;     
                ELF->sectionContent[j-1].section = realloc(ELF->sectionContent[j-1].section, ELF->sectionContent[j].size); 
                memmove(ELF->sectionContent[j-1].section, ELF->sectionContent[j].section, ELF->sectionContent[j-1].size); 
            }
            free(ELF->sectionContent[ELF->header.e_shnum-1].section);

            for(int k=0; k < ELF->header.e_shnum; k++){
                //OFFSET modif
                if (ELF->sectHeader[k].sh_offset > offsetRel){
                    ELF->sectHeader[k].sh_offset -= sizeRel;
                }
                //LINK modif
                int linkCondition = ELF->sectHeader[k].sh_type == SHT_HASH || ELF->sectHeader[k].sh_type == SHT_DYNAMIC || ELF->sectHeader[k].sh_type == SHT_SYMTAB;

                if (ELF->sectHeader[k].sh_link > i && linkCondition){
                    ELF->sectHeader[k].sh_link--;
                }
            }
            //STEP 7
            // NDX modif 
            for (int l=0; l< ELF->nb_symboles ;l++){    
                if (ELF->symTable[l].st_shndx >= i){
                    ELF->symTable[l].st_shndx--;
                }
            }
            ELF->header.e_shnum--;
        }else{
            i++;
        }
    }
}


void correctSymTable(FILE * f, Elf32_Main * ELF, uint32_t text, uint32_t data) {
    int scan, compteur;
    char nom_section[512];
    char c;
    // Modification de la table des symboles
    for (int j = 0; j < ELF->nb_symboles; j++) {
        fseek(f, ELF->sectHeader[ELF->header.e_shstrndx].sh_offset + ELF->sectHeader[ELF->symTable[j].st_shndx].sh_name, SEEK_SET); // On se rend à la position du nom de la section dans le fichier
        compteur = 0;
        scan = fscanf(f, "%c", &c);  
        while ((scan != EOF) && (c != '\0')) {   // Lecture du nom de la section dans la string table
            nom_section[compteur] = c;
            scan = fscanf(f, "%c", &c);
            compteur++;
        }
        nom_section[compteur] = '\0';              // Sans oublier d'ajouter le \0 de fin de séquence
        //printf("Symbole %d : %s\n",j,nom_section);
        //printf("valeur avant: %d\n",ELF->symTable[j].st_value);
        if (!strcmp(nom_section, ".text")) {
            ELF->symTable[j].st_value += text;
        } else if (!strcmp(nom_section, ".data")) {
            ELF->symTable[j].st_value += data;
        }
        //printf("valeur apres: %d\n\n",ELF->symTable[j].st_value);
    }
}


