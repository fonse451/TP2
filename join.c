#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "strutil.h"
#include "lista.h"

void destruir_dato(void* dato){
    free_strv(dato);
}


char* arma_cadena(char* cad_1, char* cad_2, char* nombre){
    char** linea = malloc(sizeof(char*)*3);
    linea[0] = cad_1;
    linea[3] = NULL;
    size_t largo = strlen(cad_2) - strlen(nombre);
    char* cad_aux = malloc(sizeof(char*) * largo);
    size_t j,i;
    for(j = 0,i = strlen(nombre)+1; i<strlen(cad_2); i++, j++){
        cad_aux[j] = cad_2[i];
    }
    linea[1] = cad_aux;
    char* linea_devolver = join_1(linea,' ');
    free(cad_aux);
    free(linea);
    return linea_devolver;
}

void join(int argc, char* argv_1,char* argv_2){
   
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
    //lee el primer archivo guarada en un hash las lineas y en una lista primera palabra de esa linea.
    while(getline(&linea, &capacidad, fp_1) >= 0) {
        if(!hash_pertenece(hash_lineas,linea)){
            char** cadena = split(linea, ' ');
            lista_insertar_ultimo(nombres, cadena[0]);
            hash_guardar(hash_lineas,cadena[0],cadena);
        }
        free(linea);
        linea = NULL;
    }
    free(linea);
    
    FILE* fp_2;
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
    //lee el segundo archivo y hace lo mismo que el ciclo anterior.
    while(getline(&linea_1, &capacidad_1, fp_2) >= 0) {
        char** cadena_1 = split(linea_1, ' ');
        if(hash_pertenece(hash_lineas,cadena_1[0])){
            hash_guardar(hash_lineas_1,cadena_1[0],cadena_1);
        }
        free(linea_1);
        linea_1 = NULL;
    }
    
    free(linea_1);
    
    //ciclo que se vaya fijando si pertenece en los dos hash y lo imprima con el join
    lista_iter_t* iter = lista_iter_crear(nombres);
    while(!lista_iter_al_final(iter)){
       
        char* nombre = lista_iter_ver_actual(iter);
        if(hash_pertenece(hash_lineas,nombre) && hash_pertenece(hash_lineas_1,nombre)){
            char* cad_1 = join_1(hash_obtener(hash_lineas,nombre), ' ');
            char* cad_2 = join_1(hash_obtener(hash_lineas_1,nombre), ' ');
            char* cad_imp = arma_cadena(cad_1,cad_2, nombre);
            printf("%s \n",cad_imp);
            free(cad_imp);
            free(cad_1);
            free(cad_2);
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    hash_destruir(hash_lineas);
    hash_destruir(hash_lineas_1);
    lista_destruir(nombres,NULL);
    fclose(fp_2);
    fclose(fp_1);
}


int main(int argc, char** argv){
    join(argc,argv[1],argv[2]);
    return 0;
}
