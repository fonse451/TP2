#include "lista.h"
#include <stdlib.h>

typedef struct nodo nodo_t;

struct nodo{
    void* dato;
    nodo_t* siguiente;
};

nodo_t* crear_nodo(void* dato, nodo_t* siguiente){
    nodo_t* nodo_aux = malloc(sizeof(nodo_t));
    if (nodo_aux == NULL){
        return NULL;
    }
    nodo_aux->dato = dato;
    nodo_aux->siguiente = siguiente;
    return nodo_aux;
}


struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

lista_t *lista_crear(void){
    lista_t* lista_creada = malloc(sizeof(lista_t));
    
    if (lista_creada == NULL){
        return NULL;
    }
    
    lista_creada->primero = NULL;
    lista_creada->ultimo = NULL;
    lista_creada->largo = 0;
    return lista_creada;
}


size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

bool lista_esta_vacia(const lista_t *lista){
    return lista_largo(lista) == 0;
}

bool insertar_si_esta_vacia(lista_t *lista, nodo_t* nodo){
    if (lista_esta_vacia(lista)){
        lista->primero = nodo;
        lista->ultimo = nodo;
        lista->largo++;
        return true;
    }
    return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato, lista->primero);
    if (nodo == NULL){
        return false;
    }
    if (insertar_si_esta_vacia(lista, nodo)){
        return true;
    }
    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato, NULL);
    if (nodo == NULL){
        return false;
    }
    if (insertar_si_esta_vacia(lista, nodo)){
        return true;
    }
    lista->ultimo->siguiente = nodo;
    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void* lista_ver_primero(const lista_t *lista){
    if(lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->primero->dato;
}

void* lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    nodo_t* primero = lista->primero;
    lista->primero = lista->primero->siguiente;
    lista->largo--;
    if (lista_esta_vacia(lista)){
        lista->ultimo = NULL;
    }
    void* dato = primero->dato;
    free(primero);
    return dato;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
    while (!lista_esta_vacia(lista)){
        void* eliminado = lista_borrar_primero(lista);
        if (destruir_dato != NULL){
            destruir_dato(eliminado);
        }
    }
    free(lista);
}

struct lista_iter{
    lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior;
};
    
lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (iter == NULL){
        return NULL;
    }
    iter->lista = lista;
    iter->actual = lista->primero;
    iter->anterior= NULL;
    return iter;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if(lista_iter_al_final(iter)){
        return NULL;
    }
    return iter->actual->dato;
}
    
bool lista_iter_al_final(const lista_iter_t *iter){
    return (iter->lista->ultimo == iter->anterior && iter->actual == NULL);
}
    
bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)){
        return false;
    }
    iter->anterior = iter->actual;
    iter->actual = iter->actual->siguiente;
    return true;
}

bool lista_iter_al_principio(lista_iter_t* iter){
    return iter->anterior == NULL;
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    
    if (lista_iter_al_principio(iter)){
        if(lista_insertar_primero(iter->lista, dato)){
            iter->actual = iter->lista->primero;
            return true;
        }
        return false;
        
    }
    else if (lista_iter_al_final(iter)){
       
        if(lista_insertar_ultimo(iter->lista, dato)){
            iter->actual = iter->lista->ultimo;
            return true;
        }
        return false;
    }
    else{
        nodo_t* nodo = crear_nodo(dato, iter->actual);
        if (nodo == NULL){
            return false;
        }
        iter->anterior->siguiente = nodo;
        iter->actual = nodo;
        iter->lista->largo++;
        return true;
    }
}

void *lista_iter_borrar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)){/*elimana si esta alfinal, agarrar caso que este vacia o que este en el final de la lista*/
        return NULL;
    }
    nodo_t* actual = iter->actual;
    void* dato = actual->dato;
    if (lista_iter_al_principio(iter)){/*agarrar el caso de que este al principio y el caso que el largo sea 1*/
        if(lista_largo(iter->lista) == 1){
            iter->lista->primero = NULL;
            iter->actual = NULL;
            iter->lista->ultimo = iter->anterior;
        }
        else{
            iter->actual = iter->actual->siguiente;
            iter->lista->primero = iter->actual;
        }
    }
    else if (actual->siguiente == NULL){ /*agarrar el caso de que este en el ante ultimo */
        iter->lista->ultimo = iter->anterior;
        iter->actual = NULL;
        iter->anterior->siguiente = NULL;
    }
    else{
        iter->actual = iter->actual->siguiente;/*todos los demas casos*/
        iter->anterior->siguiente = iter->actual;
    }
    iter->lista->largo--;
    free(actual);
    return dato;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra){
    for(nodo_t* nodo=lista->primero; nodo; nodo=nodo->siguiente){
        if(!visitar(nodo->dato, extra)){
            return;
        }
    }
}





