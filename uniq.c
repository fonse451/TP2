#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"

void destruir_dato(void* dato){
    free(dato);
}


void uniq(int argc, char* argv){
    if (!(argc == 2)){
        printf("Uso: ./uniq <ARCHIVO> \n");
        return;
    }
    FILE* fp;

    fp = fopen(argv,"r");
    if (fp == NULL){
        printf("No se puedo abrir el archivo\n");
        return;
    }
    hash_t* hash_lineas = hash_crear(destruir_dato);
    char *linea = NULL;
    size_t capacidad = 0;

    while(getline(&linea, &capacidad, fp) >= 0) {
        if(!hash_pertenece(hash_lineas,linea)){
            printf("%s",linea);
            hash_guardar(hash_lineas,linea,NULL);
        }
        free(linea);
        linea = NULL;
    }
    free(linea);
    hash_destruir(hash_lineas);
    fclose(fp);
}

int main(int argc, char** argv){
    uniq(argc,argv[1]);
    return 0;
}
