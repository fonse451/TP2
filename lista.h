#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una estructura compuesta por nodos. */

struct lista;
typedef struct lista lista_t;

/* el iterador esta planteado como una estructura compuesta por una lista y dos nodos */
struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía. Si no se puede crear devuelve NULL.
lista_t *lista_crear(void);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

// Devuelve verdadero o falso, según si la lista no tienen o tiene elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la cola no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el largo de la lista.
//Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Crea un iterador de lista.
// Pre: recibe una lista que fue creada.
// Post: devuelve el iterador si se puedo crear. De caso contrario devuelve NULL.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Pre: el iterador fue creado.
// Post: devuelve true o false, si se pudo avanzar o no.
bool lista_iter_avanzar(lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: devuelve el actual del iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: devuelve true o false, depende si el iterador esta al final de la lista o no.
bool lista_iter_al_final(const lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: destruye el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: devuelve true o false, si puedo inserta un elemento en la posicion actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Pre: el iterador fue creado.
// Post: Devuelve el elemento borrado. Si esta vacia o al final, devuelve NULL.
void *lista_iter_borrar(lista_iter_t *iter);

// Pre: se pasa por parametro una lista creada, una funcion que devuelve un bool y un extra.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

#endif // LISTA_H
