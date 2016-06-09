#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "strutil.h"

void destruir_dato(void* dato){
    free(dato);
}


void join(int argc, char* argv_1, argv_2){
    if (!(argc == 3)){
        printf("Uso: ./join <ARCHIVO_1> <ARCHIVO_2> \n");
        return;
    }
    FILE* fp_1;
    //crear un arreglo que contenga los nombres y el orden en que tienen que ser impresos
    fp_1 = fopen(argv_1,"r");
    if (fp_1 == NULL){
        printf("No se puedo abrir el archivo\n");
        return;
    }
    hash_t* hash_lineas = hash_crear(destruir_dato);
    char *linea = NULL;
    size_t capacidad = 0;
    
    while(getline(&linea, &capacidad, fp_1) >= 0) {
        if(!hash_pertenece(hash_lineas,linea)){
            char** cadena = split(linea, " ");
            hash_guardar(hash_lineas,cadena[0],cadena);
        }
        free(linea);
        linea = NULL;
    }
     free(linea);
    fp_2 = fopen(argv_2,"r");
    if (fp_2 == NULL){
        printf("No se puedo abrir el archivo\n");
        hash_destruir(hash_lineas);
        fclose(fp_1);
        return;
    }
    
    hash_t* hash_lineas_1 = hash_crear(destruir_dato);
    char *linea_1 = NULL;
    size_t capacidad_1 = 0;
    while(getline(&linea_1, &capacidad_1, fp_2) >= 0) {
        if(hash_pertenece(hash_lineas,linea_1)){
            char** cadena_1 = split(linea_1, " ");
            hash_guardar(hash_lineas_1,cadena_1[0],cadena_1);
        }
        free(linea_1);
        linea_1 = NULL;
    }
    free(linea_1);

    //ciclo que se vaya fijando si pertenece en los dos hash y lo imprima con el join

    hash_destruir(hash_lineas);
    hash_destruir(hash_lineas_1);
    fclose(fp_2);
}

int main(int argc, char** argv){
    uniq(argc,argv[1],argv[2]);
    return 0;
}
