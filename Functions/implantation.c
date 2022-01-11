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

uint32_t sign_extend16(uint16_t P) {
    uint32_t masque = 0;
    if (P>>15 == 1) { // Si P négatif
        return  ~masque & P;
    } else {          // P positif
        return masque | P;
    }
}

uint32_t sign_extend8(uint8_t P) {
    uint32_t masque = 0;
    if (P>>7 == 1) { // Si P négatif
        return  ~masque & P;
    } else {          // P positif
        return masque | P;
    }
}


void correctABSReloc(Elf32_Main * ELF) {
    uint32_t addend;
    uint16_t p16;
    uint8_t p8;
    uint32_t section_to_modify_index;
    unsigned char *to_modify;
    // Pour chaques tables de relocations
    for (int i = 0; i < ELF->tabSecRel.nbSections; i++) {
        // Pour chaques entrées
        for (int j = 0; j < ELF->tabSecRel.TabNb[i]; j++) {
            section_to_modify_index = ELF->sectHeader[ELF->symTable[ELF32_R_SYM(ELF->relTable[i][j].r_info)].st_shndx].sh_info;
            to_modify = ELF->sectionContent[section_to_modify_index].section;
            switch(ELF32_R_TYPE(ELF->relTable[i][j].r_info)) {
                case 2: //R_ARM_ABS32
                    // On fais rien car Addend vaut 0, T vaut 0 et S à déjà était mis à jour
                    // avec les bonnes addresses lors de l'étape 7 pour ce cas
                    break;
                case 5: //R_ARM_ABS16
                    p16 = ELF->sectHeader[ELF->symTable[ELF32_R_SYM(ELF->relTable[i][j].r_info)].st_shndx].sh_addr + ELF->relTable[i][j].r_offset;
                    addend = sign_extend16(p16);
                    *(to_modify + ELF->relTable[i][j].r_offset) = ELF->symTable[ELF32_R_SYM(ELF->relTable[i][j].r_info)].st_value + addend;
                    break;
                case 8: //R_ARM_ABS8
                    p8 = ELF->sectHeader[ELF->symTable[ELF32_R_SYM(ELF->relTable[i][j].r_info)].st_shndx].sh_addr + ELF->relTable[i][j].r_offset;
                    addend = sign_extend8(p8);
                    *(to_modify + ELF->relTable[i][j].r_offset) = ELF->symTable[ELF32_R_SYM(ELF->relTable[i][j].r_info)].st_value + addend;
                    break;
                case 28://R_ARM_CALL
                case 29://R_ARM_JUMP24
                    //all static data relocations have size 4, alignment 1 and write the full 32-bit result to the place
                    // S = ELF->symTable[ELF32_R_SYM(ELF->relTable[i][j].r_info)].st_value
                    // T = ?
                    // P = ?
                    // insn = ?
                    // *(to_modify + ELF->relTable[i][j].r_offset) = (((S + sign_extend (insn[23:0] << 2)) | T) - P )& 0x03FFFFFE
                    break;
                default:
                    break;
            }
        } 
    }
}
