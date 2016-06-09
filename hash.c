#include "hash.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define TAM_INI 37
#define FACTOR_DE_CARGA 0.6
const size_t primos[]={37,79,131,211,419,827,1601,2741};
typedef struct nodo_hash nodo_t;
typedef enum{VACIO, OCUPADO, BORRADO} estado_t;

//Las funciones auxuliares también las tienen que documentar!!!!!

/* ESTRUCTURAS */
struct nodo_hash{
    char* clave;
    void* dato;
    estado_t estado;
};

struct hash{
    nodo_t** datos;
    size_t cantidad;
    size_t largo;
    hash_destruir_dato_t destruir_dato;
};
struct hash_iter{
    const hash_t* hash;
    nodo_t* actual;
    size_t posicion;
};

/* FUNCIONES AUXILIARES*/

char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}
//pre: recibe una clave y el hash.
//post: devuelve un size_t.
size_t hashing(char *str, const hash_t* hash){
    size_t hash_1 = 5381;
    size_t c;
    
    while ((c = *str++)){
        hash_1 = ((hash_1 << 5) + hash_1) + c;
    }
    size_t hash_2 = hash_1 % hash->largo;
    return hash_2;
}
// devuelve un size_t que representa el largo del arreglo.
size_t buscar_largo(size_t largo_actual){
    
    for(size_t i = 0; i < 8; i++){
        if( (primos[i] == largo_actual) && (i+1 < 8)){
            return primos[i+1];
        }
    }
    return 5*largo_actual;
}
//crear un nodo_t* y lo incializa.
//post: devuelve null si no se pudo crear el nodo sino devuelve un nodo_t*.
nodo_t* crear_nodo_t(void){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo){
        return NULL;
    }
    nodo->clave = NULL;
    nodo->dato = NULL;
    nodo->estado = VACIO;
    return nodo;
}


//pre: recibe el hash y un size_t largo que se utiliza para el tamaño del arreglo a devolver.
//post: devuelve true si se creo el arreglo y sus nodos. False en caso de que no se pudiera crear el arreglo o los nodos.
bool hash_crear_datos(hash_t* hash, size_t largo){
    nodo_t** datos = malloc(largo * sizeof(nodo_t*));
    if (!datos){
        return false;
    }
    hash->largo = largo;
    hash->cantidad = 0;
    hash->datos = datos;
    for (size_t i = 0; i < hash->largo ; i++){
        nodo_t* nodo = crear_nodo_t();
        if (!nodo){
            return false;
        }
        hash->datos[i] = nodo;
    }
    return true;
}


//pre: recibe un size_t incial, otro final, un char* que es la clave y el hash.
//post: devuelve true si encuentra el nodo con la clave o false si no se encuentra el nodo.
bool buscar_clave(size_t* inicial,size_t final,char* clave, const hash_t* hash){
//No me parece que el nombre de la función sea adecuado. El nombre de una función te tiene //YA ESTA
//que indicar que es lo que hace, tengan eso en cuenta a la hora de nombrar las funciones.
    for (size_t i = *inicial;i<final;i++){
        if(hash->datos[i]->estado == OCUPADO && !strcmp(hash->datos[i]->clave, clave)) {
            *inicial=i;
            free(clave);
            return true;
        }
        else if(hash->datos[i]->estado == VACIO){
            *inicial=i;
            return false;
        }
    }
    return false;
}



size_t buscar_posicion(size_t* posicion,hash_t* hash,char* clave){
	//buscar_posicion deberia recibir solamente el hash y la clave. Y después devolver el size_t posicion.
    size_t posicion_limite =*posicion;
    char* clave_auxiliar = strdup((const char*) clave);
	//El if y else que siguen son exactamente iguales a los dos de abajo. Cuando ven que tienen
	//que repetir exactamente el mismo código dos veces en una misma función o en dos funciones 
	//diferentes significa que tienen que hacer una función auxiliar para esa operación. Este if y 
	//else es el caso.
    if (buscar_clave(posicion,hash->largo , clave_auxiliar, (const hash_t*) hash)){
        if (hash->destruir_dato != NULL){
            hash->destruir_dato(hash->datos[*posicion]->dato);
        }
        hash->cantidad --;
        return *posicion;//No deberian modificar el puntero y devolver su valor.
    }
    else if (hash->datos[*posicion]->estado == VACIO){
        free(clave_auxiliar);
        return *posicion;
    }
    *posicion = 0;
    if (buscar_clave(posicion,posicion_limite, clave_auxiliar, (const hash_t*) hash)){
        if (hash->destruir_dato != NULL){
            hash->destruir_dato(hash->datos[*posicion]->dato);
        }
        hash->cantidad --;
        return *posicion;
    }
    else{
        free(clave_auxiliar);
        return *posicion;
    }
}

//pre: recibe el hash, char* clave y un void* dato.
void hash_asignar(hash_t *hash, char *clave, void *dato){
    size_t x = hashing(clave, hash);
    size_t y = buscar_posicion(&x,hash,clave);//Nombres claros!!!!!
    if (!hash_pertenece(hash,clave)){
        hash->datos[y]->clave = clave;
    }
    
    else{
        free(clave);
    }
    hash->datos[y]->dato = dato;
    hash->datos[y]->estado = OCUPADO;
    hash->cantidad++;
}
//pre: recibe el hash.
//post: devuelve false si hay que redimensionar o true si todavia no.
bool verificar_redimensionar(hash_t* hash){ //Tienen que cambiar el nombre de la función. Fijense la // YA ESTA
	//corrección anterior
    float carga =(float) hash->cantidad/(float)hash->largo;
    return FACTOR_DE_CARGA > carga;
}

void nodos_destruir(hash_t* hash, size_t largo ,nodo_t** datos){
//Revisen las correcciones de la vez pasada que no hicieron las modificaciones. //  YA ESTA
    for(size_t i = 0; i<largo; i++){
        nodo_t* nodo = datos[i];
        if (nodo->estado == OCUPADO){
            if (hash->destruir_dato != NULL){
                hash->destruir_dato(nodo->dato);
            }
            free(nodo->clave);
        }
        free(nodo);
        
    }
}

//pre: recibe el hash.
//post: devuelve false de no poder crear los datos. True si se pudo redimensionar.
bool hash_redimensionar(hash_t* hash){
    nodo_t** datos_viejo = hash->datos;
    size_t largo_viejo = hash->largo;
    size_t largo_nuevo = buscar_largo(hash->largo);//Fijense esto en las correcciones anteriores. //PREGUNTAR
    
    if(!hash_crear_datos(hash,largo_nuevo)){
        return false;
    }
    for(size_t i = 0; i < largo_viejo ;i++){
        if (datos_viejo[i]->estado == OCUPADO){
            hash_guardar(hash,datos_viejo[i]->clave,datos_viejo[i]->dato);
        }
        free(datos_viejo[i]->clave);
        free(datos_viejo[i]);
    }
    free(datos_viejo);
    return true;
}

/*PRIMITIVAS DE HASH*/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash){
        return NULL;
    }
    if (hash_crear_datos(hash, TAM_INI)){
        hash->destruir_dato = destruir_dato;
        return hash;
    }
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    char* clave_aux = strdup(clave);
    size_t posicion = hashing(clave_aux,hash);
	size_t posicion_limite = posicion;
	bool respuesta = true;
	if (!buscar_clave(&posicion,hash->largo,clave_aux,hash)){
		posicion = 0;
		respuesta = buscar_clave(&posicion,posicion_limite,clave_aux,hash);
	}
	if (respuesta != true){
		free(clave_aux);
	}
	return respuesta;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    char* clave_aux = strdup(clave);
	//Revisen las correcciones que les hice la vez anterior
    if (!clave_aux){
        return false;
    }
    if (verificar_redimensionar(hash)){
        hash_asignar(hash, clave_aux,dato);
        return true;
    }
    else{
        if (hash_redimensionar(hash)){
            hash_asignar(hash,clave_aux,dato);
            return true;
        }
        return false;
    }
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void *hash_borrar(hash_t *hash, const char *clave){
    char* clave_aux = strdup(clave);//No necesitan copiar la clave!
    
    size_t posicion = hashing(clave_aux,hash);//Haganlo en el buscar_posicion
    size_t posicion_aux = posicion;

	if(!buscar_clave(&posicion,hash->largo,clave_aux,(const hash_t*)hash)){
    	posicion = 0;
		if(!buscar_clave(&posicion,posicion_aux,clave_aux,(const hash_t*) hash)){
			free(clave_aux);
   			return NULL;
		}
	}	
    void* dato = hash->datos[posicion]->dato;
   	free(hash->datos[posicion]->clave);
   	hash->datos[posicion]->estado=BORRADO;
    hash->cantidad--;
    return  dato;
}

void hash_destruir(hash_t *hash){
    nodos_destruir(hash, hash->largo, hash->datos);
    free(hash->datos);
    free(hash);
}

void *hash_obtener(const hash_t *hash, const char *clave){
    char* clave_aux = strdup(clave);
    size_t posicion = hashing(clave_aux,hash);//Haganlo en el buscar_posicion
    size_t posicion_aux = posicion;
    
    if(!buscar_clave(&posicion,hash->largo,clave_aux,(const hash_t*)hash)){
    	posicion = 0;
		if(!buscar_clave(&posicion,posicion_aux,clave_aux,(const hash_t*) hash)){
			free(clave_aux);
   			return NULL;
 		}
	}
    return hash->datos[posicion]->dato;
}

/*PRIMITIVAS DE ITERADOR DE HASH*/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iterador = malloc(sizeof(hash_iter_t));
    if (!iterador){
        return NULL;
    }
    iterador->hash = hash;
    iterador->posicion = 0;
    nodo_t* actual = iterador->hash->datos[iterador->posicion];
    while(actual->estado == VACIO){ 
        if (iterador->posicion+1>= iterador->hash->largo-1){
            iterador->actual = NULL;
            return iterador;
        }
        iterador->posicion ++;
        actual = iterador->hash->datos[iterador->posicion];
    }
    iterador->actual = actual;
    return iterador;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    if (iter->hash->cantidad == 0){
        return true;
    }
    return (iter->posicion == iter->hash->largo);
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if (hash_iter_al_final(iter)){
        return false;
    }
    if(iter->posicion+1 == iter->hash->largo){
        iter->posicion++;
        return true;
    }
    nodo_t* actual = iter->hash->datos[iter->posicion+1];
	iter->posicion ++;
    while(actual->estado == VACIO){
        if (iter->posicion+1 >= iter->hash->largo){
            iter->posicion++;
            return true;
        }
        iter->posicion ++;
        actual = iter->hash->datos[iter->posicion];
    }
    iter->actual = actual;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_al_final(iter)){
        return NULL;
    }
    const char* valor = iter->actual->clave;
    return valor;
}

void hash_iter_destruir(hash_iter_t* iter){
    free(iter);
}









