#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *substr(const char *str, size_t n){
    size_t largo = n+1;
    char* cadena=malloc(sizeof(char) * largo);
    if(cadena == NULL){
        return NULL;
    }
    strncpy(cadena,str,n);
    cadena[n] = '\0';
    return cadena;
}


char **split(const char *str, char sep){
    if(sep == atoi("\0")){
        return NULL;
    }
    
    size_t cont = 2;
    for(size_t i = 0; i<strlen(str); i++){
        if(str[i] == sep){
            cont++;
        }
    }
 
    char** cadenas = malloc(cont * sizeof(char*));
    if (cadenas == NULL){
        return  NULL;
    }
    
    size_t n_cadena = 0;
    size_t indice_aux = 0;
    
    for(size_t indice = 0; indice<strlen(str); indice++){
        if(str[indice] == sep){
            cadenas[n_cadena] = substr(&str[indice_aux],indice-indice_aux);
            indice_aux = indice+1;
            n_cadena++;
        }
        
    }
    cadenas[n_cadena] = substr(&str[indice_aux],strlen(str));
    cadenas[n_cadena+1] = NULL;
    
    return cadenas;
}

char *join_1(char **strv, char sep){
    if (!strv[0]){
        char* cadena_vacia;
        cadena_vacia = strdup("");
        if(!cadena_vacia){
            return NULL;
        }
        return cadena_vacia;
    }
    size_t largo_cadena = 0;
    for (size_t i = 0; strv[i] != NULL; i++){
        
        largo_cadena+=strlen(strv[i]);
        largo_cadena++;
    }
  
    char* cadena_join = malloc(sizeof(char) * largo_cadena);
    if(cadena_join == NULL){
        return NULL;
    }

    size_t x = 0;
    for(size_t ind = 0 ; strv[ind] != NULL; ind++){
        strncpy(&cadena_join[x], strv[ind],strlen(strv[ind]));
        x += strlen(strv[ind]);
        if (strv[ind+1] == NULL){
            cadena_join[x] = '\0';
            break;
        }
        cadena_join[x] = sep;
        x++;
    }
    return cadena_join;
}

void free_strv(char *strv[]){
    for( size_t indice = 0; strv[indice] != NULL; indice++ ){
        free(strv[indice]);
    }
    free(strv);
}
    

