#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "strutil.h"
#include "lista.h"

void destruir_dato(void* dato){
    free(dato);
}


void join(int argc, char* argv_1, argv_2){
    if (!(argc == 3)){
        printf("Uso: ./join <ARCHIVO_1> <ARCHIVO_2> \n");
        return;
    }
    FILE* fp_1;
    lista_t* nombres = lista_crear();

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
            lista_insertar_ultimo(cadena[0]);
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
        lista_destruir(nombres,NULL);
        fclose(fp_1);
        return;
    }
    
    hash_t* hash_lineas_1 = hash_crear(destruir_dato);
    char *linea_1 = NULL;
    size_t capacidad_1 = 0;
    while(getline(&linea_1, &capacidad_1, fp_2) >= 0) {
        char** cadena_1 = split(linea_1, " ");
        if(hash_pertenece(hash_lineas,cadena_1[0])){
            hash_guardar(hash_lineas_1,cadena_1[0],cadena_1);
        }
        free(linea_1);
        linea_1 = NULL;
    }
    free(linea_1);

    //ciclo que se vaya fijando si pertenece en los dos hash y lo imprima con el join
    for(size_t i = 0 ; i<lista_largo(nombres); i++){
        char* cad_1 = join_1(hash_obtener(hash_lineas,nombres[i]), " ");
        char* cad_2 = join_1(hash_obtener(hash_lineas_1,nombres[i])[1], " ");
        printf("%s %s",cad_1,cad_2);
    }
    hash_destruir(hash_lineas);
    hash_destruir(hash_lineas_1);
    lista_destruir(nombres,NULL);
    fclose(fp_2);
    fclose(fp_1);
}

int main(int argc, char** argv){
    uniq(argc,argv[1],argv[2]);
    return 0;
}
