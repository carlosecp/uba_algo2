#include <stdio.h>
#include <stdlib.h>

#include "pa2mm.h"
#include "src/cola.h"
#include "src/lista.h"
#include "src/pila.h"

/* Lista: Creacion */

void
puedoCrearYDestruirUnaLista()
{
    lista_t* lista = lista_crear();

    pa2m_afirmar(lista != NULL, "Crear una lista devuelve la lista");
    pa2m_afirmar(lista_primero(lista) == NULL,
                 "El nodo inicial de la lista apunta a NULL");
    pa2m_afirmar(lista_ultimo(lista) == NULL,
                 "El nodo final de la lista apunta a NULL");
    pa2m_afirmar(lista_vacia(lista), "La lista se crea vacia");

    lista_destruir(lista);
}

/* Lista: Insercion */

void
dadaUnaListaVacia_alInsertarUnElementoExitosamente_devuelveLaLista()
{
    lista_t* lista = lista_crear();

    pa2m_afirmar(lista_insertar(lista, NULL) != NULL,
                 "Insertar en una lista devuelve la lista");

    lista_destruir(lista);
}

void
dadaUnaListaNULL_alIntentarInsertarUnElemento_devuelveNULL()
{
    lista_t* lista = NULL;

    pa2m_afirmar(lista_insertar(lista, NULL) == NULL,
                 "Insertar en una lista NULL devuelve NULL");
}

void
dadaUnaListaVacia_alInsertar10ElementosExitosamente_laListaQuedaConTamanio10()
{
    lista_t* lista = lista_crear();

    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);

    pa2m_afirmar(lista_tamanio(lista) == 10,
                 "Para una lista con 10 elementos el tamanio es 10")

        lista_destruir(lista);
}

void
dadaUnaListaVacia_alInsertarMultiplesElementosExitosamente_losElementosQuedanEnElOrdenCorrecto()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);

    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 0) == a,
                 "El primer elemento insertado, cuyo valor es 'a', queda en la "
                 "posicion 0");
    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 1) == b,
                 "El segundo elemento insertado, cuyo valor es 'b', queda en "
                 "la posicion 1");
    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 2) == c,
                 "El tercer elemento insertado, cuyo valor es 'c', queda en la "
                 "posicion 2");

    lista_destruir(lista);
}

/* Lista: Insercion en Posicion Especifica */

void
dadaUnaListaNULL_alIntentarInsertarUnElementoEnUnaPosicionEspecifica_devuelveNULL()
{
    lista_t* lista = NULL;

    pa2m_afirmar(lista_insertar_en_posicion(lista, NULL, 0) == NULL,
                 "Insertar un elemento en una posicion especifica en una lista "
                 "NULL devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alInsertarUnElementoEnLaPosicion0Exitosamente_elElementoSeInsertaAlInicio()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd';
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar_en_posicion(lista, &a, 0);

    pa2m_afirmar(*(char*)lista_primero(lista) == a,
                 "Insertar el elemento 'a' en la posicion 0, ubica el elemento "
                 "al inicio de la lista");
    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 1) == b,
                 "Tras insertar 'a' en la posicion 0, el elemento 'b', que "
                 "antes estaba de primero, ahora esta en la posicion 1");

    lista_destruir(lista);
}

void
dadaUnaLista_alIntentarInsertarUnElementoEnUnaPosicionInvalida_elElementoSeInsertaAlFinal()
{
    lista_t* lista = lista_crear();

    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);

    char a = 'a';
    lista = lista_insertar_en_posicion(lista, &a, 10);

    pa2m_afirmar(*(char*)lista_ultimo(lista) == a,
                 "Intentar insertar un elemento en una posicion mayor a la "
                 "posible, lo inserta al final de la lista");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alInsertarUnElementoEnUnaPosicionEspecificaExitosamente_losElementosQuedanEnElOrdenCorrecta()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', e = 'e', f = 'f';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &e);
    lista = lista_insertar(lista, &f);

    char d = 'd';
    lista = lista_insertar_en_posicion(lista, &d, 3);

    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 3) == d,
                 "El elemento insertado en la posicion 3, cuyo valor es 'd', "
                 "queda en esa posicion");
    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 4) == e,
                 "Tras insertar 'd' en la posicion 3, el elemento 'e', que "
                 "antes estaba en la posicion 3, ahora esta en la posicion 4");

    lista_destruir(lista);
}

/* Lista: Eliminacion */

void
dadaUnaListaNULL_alIntentarQuitarUnElemento_devuelveNULL()
{
    lista_t* lista = NULL;

    pa2m_afirmar(lista_quitar(lista) == NULL,
                 "Quitar elemento de lista NULL devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaListaVacia_alIntentarQuitarUnElemento_devuelveNULL()
{
    lista_t* lista = lista_crear();

    pa2m_afirmar(lista_quitar(lista) == NULL,
                 "Quitar elemento de lista vacia devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alQuitarElUltimoElementoExitosamente_devuelveElElemento()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);

    pa2m_afirmar(*(char*)lista_quitar(lista) == c,
                 "Quitar un elemento devuelve el elemento");
    pa2m_afirmar(*(char*)lista_ultimo(lista) == b,
                 "Al quitar un elemento, el elemento que antes estaba de "
                 "penultimo, ahora es el ultimo");

    lista_destruir(lista);
}

void
dadaUnaListaCon1Elemento_alQuitarElElemento_elNodoInicialYFinalApuntanANULL()
{
    lista_t* lista = lista_crear();

    char a = 'a';
    lista = lista_insertar(lista, &a);
    lista_quitar(lista);

    pa2m_afirmar(
        lista_primero(lista) == NULL,
        "Para una lista con 1 elemento, al quitar el elemento, el nodo "
        "inicial apunta a NULL");
    pa2m_afirmar(
        lista_ultimo(lista) == NULL,
        "Para una lista con 1 elemento, al quitar el elemento, el nodo "
        "final apunta a NULL");

    lista_destruir(lista);
}

void
dadaUnaListaCon5Elementos_alQuitarLos2UltimosElementosExitosamente_laListaQuedaConTamanio3()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);
    lista_quitar(lista);
    lista_quitar(lista);

    pa2m_afirmar(lista_tamanio(lista) == 3,
                 "Para una lista con 5 elementos, al quitar los 2 ultimos "
                 "elementos el tamanio es 3");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alQuitarTodosElementosExitosamente_laListaQuedaVacia()
{
    lista_t* lista = lista_crear();

    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, NULL);
    lista_quitar(lista);
    lista_quitar(lista);
    lista_quitar(lista);
    lista_quitar(lista);
    lista_quitar(lista);

    pa2m_afirmar(lista_vacia(lista),
                 "Al quitar todos los elementos de la lista, esta queda vacia");

    lista_destruir(lista);
}

/* Lista: Eliminacion en Posicion Especifica */

void
dadaUnaListaNULL_alIntentarQuitarUnElementoEnUnaPosicionEspecifica_devuelveNULL()
{
    lista_t* lista = NULL;

    pa2m_afirmar(lista_quitar_de_posicion(lista, 10) == NULL,
                 "Quitar un elemento en una posicion especifica en una lista "
                 "NULL, devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaListaVacia_alIntentarQuitarUnElementoEnUnaPosicionEspecifica_devuelveNULL()
{
    lista_t* lista = lista_crear();

    pa2m_afirmar(lista_quitar_de_posicion(lista, 10) == NULL,
                 "Quitar un elemento en una posicion especifica en una lista "
                 "vacia, devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alQuitarUnElementoEnUnaPosicionEspecificaExitosamente_devuelveElElemento()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);

    pa2m_afirmar(*(char*)lista_quitar_de_posicion(lista, 2) == c,
                 "Quitar un elemento en una posicion especifica en una lista "
                 "devuelve ese elemento");
    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 2) == d,
                 "Al quitar el elemento 'd' de la posicion 2, el elemento que "
                 "le seguia, cuyo valor es 'e', ahora ocupa la posicion 2");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alQuitarElElementoEnLaPosicion0Exitosamente_seEliminaElPrimerElemento()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);

    pa2m_afirmar(*(char*)lista_quitar_de_posicion(lista, 0) == a,
                 "Quitar un elemento en la posicion 0 efectivamente quita el "
                 "primer elemento");

    lista_destruir(lista);
}

void
dadaUnaListaCon5Elementos_alQuitarUnElementoEnUnaPosicionEspecificaExitosamente_laListaQuedaConTamanio4()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);
    lista_quitar_de_posicion(lista, 1);

    pa2m_afirmar(lista_tamanio(lista) == 4,
                 "Para una lista con 5 elementos, al quitar un elemento "
                 "especifico la lista queda con tamanio 4");

    lista_destruir(lista);
}

/* Lista: Acceso Elementos */

void
dadaUnaListaNULL_alAccederAUnElemento_devuelveNULL()
{
    lista_t* lista = NULL;

    pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == NULL,
                 "Acceder a lista NULL devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaListaVacia_alAccederAUnElemento_devuelveNULL()
{
    lista_t* lista = lista_crear();

    pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == NULL,
                 "Acceder a lista vacia devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaLista_alAccederAUnElementoEnUnaPosicionInvalida_devuelveNULL()
{
    lista_t* lista = lista_crear();

    pa2m_afirmar(lista_elemento_en_posicion(lista, 5) == NULL,
                 "Acceder a una posicion invalida devuelve NULL");

    lista_destruir(lista);
}

void
dadaUnaLista_alAccederAlElementoEnLaPoscion0Exitosamente_devuelveElPrimerElemento()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);

    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 0) == a,
                 "Acceder al elemento en la posicion 0 devuelve el primer "
                 "elemento de la lista");

    lista_destruir(lista);
}

void
dadaUnaLista_alAccederAlElementoEnLaPoscionFinalExitosamente_devuelveElUltimoElemento()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);

    pa2m_afirmar(
        *(char*)lista_elemento_en_posicion(lista, lista_tamanio(lista)) == e,
        "Acceder al elemento en la posicion igual al tamanio de la lista "
        "devuelve el ultimo elemento de la lista");

    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alAccederAlElementoEnUnaPosicionEspecificaExitosamente_devuelveElElemento()
{
    lista_t* lista = lista_crear();

    char a = 'a';
    lista = lista_insertar(lista, NULL);
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, NULL);

    pa2m_afirmar(*(char*)lista_elemento_en_posicion(lista, 1) == a,
                 "Acceder a la posicion de un elemento de la lista, "
                 "efecticamente devuelve el elemento en esa posicion");

    lista_destruir(lista);
}

/* Lista: Iterador */

void
puedoCrearYDestruirUnIterador()
{
    lista_t* lista = NULL;
    lista_iterador_t* it = lista_iterador_crear(lista);

    pa2m_afirmar(it == NULL, "Para una lista NULL, el iterador se crea NULL");

    lista_iterador_destruir(it);
}

void
dadaUnaListaVacia_puedoCrearYDestruirUnIterador()
{
    lista_t* lista = lista_crear();
    lista_iterador_t* it = lista_iterador_crear(lista);

    pa2m_afirmar(lista_iterador_elemento_actual(it) == NULL,
                 "Para una lista vacia, el iterador se crea con elemento "
                 "corriente apuntando a NULL");

    lista_iterador_destruir(it);
    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_puedoCrearYDestruirUnIterador()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);

    lista_iterador_t* it = lista_iterador_crear(lista);

    pa2m_afirmar(*(char*)lista_iterador_elemento_actual(it) == a,
                 "Para una lista con varios elementos, el iterador se crea con "
                 "elemento corriente apuntando al primer elemento de la lista");

    lista_iterador_destruir(it);
    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alIterarConUnIteradorExterno_elIteradorAvanzaSobreLosElementosCorrectamente()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);

    lista_iterador_t* it = lista_iterador_crear(lista);

    bool iterador_avanza = lista_iterador_avanzar(it);
    pa2m_afirmar(
        iterador_avanza && (*(char*)lista_iterador_elemento_actual(it) == b),
        "Para un iterador que esta sobre el primer elemento de la lista 'a', "
        "al avanzar el iterador, se posiciona sobre el segundo elemento de la "
        "lista 'b'");

    iterador_avanza = lista_iterador_avanzar(it);
    pa2m_afirmar(
        iterador_avanza && (*(char*)lista_iterador_elemento_actual(it) == c),
        "Para un iterador que esta sobre el segundo elemento de la lista 'b', "
        "al avanzar el iterador, se posiciona sobre el tercer elemento de la "
        "lista 'c'");
    
    lista_iterador_destruir(it);
    lista_destruir(lista);
}

void
dadaUnaListaConVariosElementos_alIterarConUnIteradorExternoHastaElFinalDeLaListaEIntentarIterador_elIteradorNoAvanza()
{
    lista_t* lista = lista_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    lista = lista_insertar(lista, &a);
    lista = lista_insertar(lista, &b);
    lista = lista_insertar(lista, &c);
    lista = lista_insertar(lista, &d);
    lista = lista_insertar(lista, &e);

    lista_iterador_t* it = lista_iterador_crear(lista);

    bool iterador_avanza = lista_iterador_avanzar(it);
    iterador_avanza = lista_iterador_avanzar(it);
    iterador_avanza = lista_iterador_avanzar(it);
    iterador_avanza = lista_iterador_avanzar(it);
    iterador_avanza = lista_iterador_avanzar(it);

    pa2m_afirmar(!iterador_avanza,
                 "Cuando el iterador llega al final de la lista, al intentar "
                 "avanzar devuelve false");

    iterador_avanza = lista_iterador_avanzar(it);
    pa2m_afirmar(lista_iterador_elemento_actual(it) == NULL,
                 "Cuando el iterador llega al final de la lista el elemento "
                 "actual apunta a NULL");

    lista_iterador_destruir(it);
    lista_destruir(lista);
}

/* Cola: Creacion */

void
puedoCrearYDestruirUnaCola()
{
    cola_t* cola = cola_crear();

    pa2m_afirmar(cola != NULL, "Crear una cola devuelve la cola");
    pa2m_afirmar(cola_frente(cola) == NULL,
                 "El frente de la cola se crea NULL");
    pa2m_afirmar(cola_vacia(cola), "La cola se crea vacia");

    cola_destruir(cola);
}

/* Cola: Encolar */

void
dadaUnaColaNULL_alIntentarEncolarUnElemento_devuelveNULL()
{
    cola_t* cola = NULL;

    pa2m_afirmar(cola_encolar(cola, NULL) == NULL,
                 "Encolar en una cola NULL devuelve NULL");

    cola_destruir(cola);
}

void
dadaUnaColaVacia_alEncolar10ElementosExitosamente_laColaQuedaConTamanio10()
{
    cola_t* cola = cola_crear();

    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);

    pa2m_afirmar(cola_tamanio(cola) == 10,
                 "Para una cola con 10 elementos el tamanio es 10");

    cola_destruir(cola);
}

void
dadaUnaColaCon1ElementoEnElFrente_alEncolarMultiplesElementosExitosamente_elPrimerElementoSigueEnElFrente()
{
    cola_t* cola = cola_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
    cola = cola_encolar(cola, &a);
    cola = cola_encolar(cola, &b);
    cola = cola_encolar(cola, &c);
    cola = cola_encolar(cola, &d);
    cola = cola_encolar(cola, &e);

    pa2m_afirmar(*(char*)cola_desencolar(cola) == a,
                 "Luego de encolar multiples elementos, el primer elemento "
                 "encolado 'a', sigue estando en el frente de la cola");

    cola_destruir(cola);
}

/* Cola: Desencolar */

void
dadaUnaColaNULL_alIntentarDesencolarUnElemento_devuelveNULL()
{
    cola_t* cola = NULL;

    pa2m_afirmar(cola_desencolar(cola) == NULL,
                 "Desencolar en una cola NULL, devuelve NULL");

    cola_destruir(cola);
}

void
dadaUnaColaVacia_alIntentarDesencolarUnElemento_devuelveNULL()
{
    cola_t* cola = cola_crear();

    pa2m_afirmar(cola_desencolar(cola) == NULL,
                 "Desencolar en una cola vacia, devuelve NULL");

    cola_destruir(cola);
}

void
dadaUnaColaConVariosElementos_alDesencolarUnElementoExitosamente_devuelveElElemento()
{
    cola_t* cola = cola_crear();

    char a = 'a', b = 'b', c = 'c';
    cola = cola_encolar(cola, &a);
    cola = cola_encolar(cola, &b);
    cola = cola_encolar(cola, &c);

    pa2m_afirmar(*(char*)cola_desencolar(cola) == a,
                 "Desencolar un elemento devuelve el elemento");

    cola_destruir(cola);
}

void
dadaUnaColaConVariosElementos_alDesencolarUnElementoExitosamente_elSegundoElementoEncoladoAhoraEsElFrente()
{
    cola_t* cola = cola_crear();

    char a = 'a', b = 'b';
    cola = cola_encolar(cola, &a);
    cola = cola_encolar(cola, &b);
    cola_desencolar(cola);

    pa2m_afirmar(*(char*)cola_frente(cola) == b,
                 "Al desencolar el primer elemento 'a', el segundo elemento "
                 "encolado 'b', pasa a estar en el frente de la cola");

    cola_destruir(cola);
}

void
dadaUnaColaCon5Elementos_alDesencolar1ElementoExitosamente_laColaQuedaConTamanio4()
{
    cola_t* cola = cola_crear();

    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola_desencolar(cola);

    pa2m_afirmar(cola_tamanio(cola) == 4,
                 "Tras desencolar una cola con 5 elementos, el tamanio es 4");

    cola_destruir(cola);
}

void
dadaUnaColaConVariosElementos_alDesencolarTodosElementosExitosamente_laColaQuedaVacia()
{
    cola_t* cola = cola_crear();

    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola = cola_encolar(cola, NULL);
    cola_desencolar(cola);
    cola_desencolar(cola);
    cola_desencolar(cola);
    cola_desencolar(cola);
    cola_desencolar(cola);

    pa2m_afirmar(
        cola_vacia(cola),
        "Al desencolar todos los elementos de una cola, esta queda vacia");

    cola_destruir(cola);
}

/* Cola: Acceso Frente */

void
dadaUnaColaNULL_alAccederAlElementoDelFrente_devuelveNULL()
{
    cola_t* cola = NULL;

    pa2m_afirmar(cola_frente(cola) == NULL,
                 "Acceder al frente de una cola NULL devuelve NULL");

    cola_destruir(cola);
}

void
dadaUnaColaVacia_alAccederAlElementoDelFrente_devuelveNULL()
{
    cola_t* cola = cola_crear();

    pa2m_afirmar(cola_frente(cola) == NULL,
                 "Acceder al frente de una cola vacia devuelve NULL");

    cola_destruir(cola);
}

void
dadaUnaColaConVariosElementos_alAccederAlElementoDelFrente_devuelveElElemento()
{
    cola_t* cola = cola_crear();

    char a = 'a', b = 'b', c = 'c';
    cola = cola_encolar(cola, &a);
    cola = cola_encolar(cola, &b);
    cola = cola_encolar(cola, &c);

    pa2m_afirmar(
        *(char*)cola_frente(cola) == a,
        "Acceder al frente de una cola, efectivamente devuelve ese elemento");

    cola_destruir(cola);
}

/* Pila: Creacion */

void
puedoCrearYDestruirUnaPila()
{
    pila_t* pila = pila_crear();

    pa2m_afirmar(pila != NULL, "Crear una pila devuelve la pila");
    pa2m_afirmar(pila_tope(pila) == NULL,
                 "El tope de la pila se crea NULL");
    pa2m_afirmar(pila_vacia(pila), "La pila se crea vacia");

    pila_destruir(pila);
}

/* Pila: Apilado */

void
dadaUnaPilaNULL_alIntentarApilarUnElemento_devuelveNULL()
{
    pila_t* pila = NULL;

    pa2m_afirmar(pila_apilar(pila, NULL) == NULL,
                 "Apilar en una pila NULL devuelve NULL");

    pila_destruir(pila);
}

void
dadaUnaPilaVacia_alApilar10ElementosExitosamente_laPilaQuedaConTamanio10()
{
    pila_t* pila = pila_crear();

    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);

    pa2m_afirmar(pila_tamanio(pila) == 10,
                 "Para una cola con 10 elementos el tamanio es 10");

    pila_destruir(pila);
}

void
dadaUnaPilaConVariosElementos_alApilarUnElementoExitosamente_eseElementoPasaASerElTope()
{
    pila_t* pila = pila_crear();

    char a = 'a', b = 'b', c = 'c', d = 'd';
    pila = pila_apilar(pila, &a);
    pila = pila_apilar(pila, &b);
    pila = pila_apilar(pila, &c);

    pa2m_afirmar(*(char*)pila_tope(pila) == c,
                 "Para una pila de varios elementos, al apilar el elemento 'c', "
                 "ese elemento es ubicado en el tope de la pila");

    pila = pila_apilar(pila, &d);
    pa2m_afirmar(*(char*)pila_tope(pila) == d,
                 "Tras apilar el elemento 'd', ese elemento es ubicado en el "
                 "tope de la pila");

    pila_destruir(pila);
}

/* Pila: Desapilado */

void
dadaUnaPilaNULL_alIntentarDesapilarUnElemento_devuelveNULL()
{
    pila_t* pila = NULL;

    pa2m_afirmar(pila_desapilar(pila) == NULL,
                 "Desapilar en una pila NULL, devuelve NULL");

    pila_destruir(pila);
}

void
dadaUnaPilaVacia_alIntentarDesapilarUnElemento_devuelveNULL()
{
    pila_t* pila = pila_crear();

    pa2m_afirmar(pila_desapilar(pila) == NULL,
                 "Desapilar en una pila vacia, devuelve NULL");

    pila_destruir(pila);
}

void
dadaUnaPilaConVariosElementos_alDesapilarUnElementoExitosamente_devuelveElElemento()
{
    pila_t* pila = pila_crear();

    char a = 'a', b = 'b', c = 'c';
    pila = pila_apilar(pila, &a);
    pila = pila_apilar(pila, &b);
    pila = pila_apilar(pila, &c);

    pa2m_afirmar(*(char*)pila_desapilar(pila) == c,
                 "Desapilar un elemento devuelve el elemento");

    pila_destruir(pila);
}

void
dadaUnaPilaConVariosElementos_alDesapilarUnElementoExitosamente_elPenultimoElementoEncoladoAhoraEsElTope()
{
    pila_t* pila = pila_crear();

    char a = 'a', b = 'b';
    pila = pila_apilar(pila, &a);
    pila = pila_apilar(pila, &b);
    pila_desapilar(pila);

    pa2m_afirmar(*(char*)pila_tope(pila) == a,
                 "Al desapilar el ultimo elemento apilado 'b', el penultimo "
                 "elemento apilado 'a', pasa a estar en el tope de la pila");

    pila_destruir(pila);
}

void
dadaUnaPilaCon5Elementos_alDesapilar1ElementoExitosamente_laPilaQuedaConTamanio4()
{
    pila_t* pila = pila_crear();

    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila_desapilar(pila);
    
    pa2m_afirmar(pila_tamanio(pila) == 4,
                 "Tras desapilar una pila con 5 elementos, el tamanio es 4");

    pila_destruir(pila);
}

void
dadaUnaPilaConVariosElementos_alDesapilarTodosElementosExitosamente_laPilaQuedaVacia()
{
    pila_t* pila = pila_crear();

    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila = pila_apilar(pila, NULL);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);

    pa2m_afirmar(
        pila_vacia(pila),
        "Al desapilar todos los elementos de una pila, esta queda vacia");

    pila_destruir(pila);
}

/* Pila: Acceso Tope */

void
dadaUnaPilaNULL_alAccederAlElementoDelTope_devuelveNULL()
{
    pila_t* pila = NULL;

    pa2m_afirmar(pila_tope(pila) == NULL,
                 "Acceder al tope de una pila NULL devuelve NULL");

    pila_destruir(pila);
}

void
dadaUnaPilaVacia_alAccederAlElementoDelTope_devuelveNULL()
{
    pila_t* pila = pila_crear();

    pa2m_afirmar(pila_tope(pila) == NULL,
                 "Acceder al tope de una pila vacia devuelve NULL");

    pila_destruir(pila);
}

void
dadaUnaPilaConVariosElementos_alAccederAlElementoDelTope_devuelveElElemento()
{
    pila_t* pila = pila_crear();

    char a = 'a', b = 'b', c = 'c';
    pila = pila_apilar(pila, &a);
    pila = pila_apilar(pila, &b);
    pila = pila_apilar(pila, &c);

    pa2m_afirmar(
        *(char*)pila_tope(pila) == c,
        "Acceder al tope de una pila, efectivamente devuelve ese elemento");

    pila_destruir(pila);
}

int
main()
{
    pa2m_nuevo_grupo("Prueba Lista: Creacion");
    puedoCrearYDestruirUnaLista();

    pa2m_nuevo_grupo("Prueba Lista: Insercion");
    dadaUnaListaNULL_alIntentarInsertarUnElemento_devuelveNULL();
    dadaUnaListaVacia_alInsertarUnElementoExitosamente_devuelveLaLista();
    dadaUnaListaVacia_alInsertar10ElementosExitosamente_laListaQuedaConTamanio10();
    dadaUnaListaVacia_alInsertarMultiplesElementosExitosamente_losElementosQuedanEnElOrdenCorrecto();

    pa2m_nuevo_grupo("Prueba Lista: Insercion en Posicion Especifica");
    dadaUnaListaNULL_alIntentarInsertarUnElementoEnUnaPosicionEspecifica_devuelveNULL();
    dadaUnaListaConVariosElementos_alInsertarUnElementoEnLaPosicion0Exitosamente_elElementoSeInsertaAlInicio();
    dadaUnaLista_alIntentarInsertarUnElementoEnUnaPosicionInvalida_elElementoSeInsertaAlFinal();
    dadaUnaListaConVariosElementos_alInsertarUnElementoEnUnaPosicionEspecificaExitosamente_losElementosQuedanEnElOrdenCorrecta();

    pa2m_nuevo_grupo("Prueba Lista: Eliminacion");
    dadaUnaListaNULL_alIntentarQuitarUnElemento_devuelveNULL();
    dadaUnaListaVacia_alIntentarQuitarUnElemento_devuelveNULL();
    dadaUnaListaConVariosElementos_alQuitarElUltimoElementoExitosamente_devuelveElElemento();
    dadaUnaListaCon1Elemento_alQuitarElElemento_elNodoInicialYFinalApuntanANULL();
    dadaUnaListaCon5Elementos_alQuitarLos2UltimosElementosExitosamente_laListaQuedaConTamanio3();
    dadaUnaListaConVariosElementos_alQuitarTodosElementosExitosamente_laListaQuedaVacia();

    pa2m_nuevo_grupo("Prueba Lista: Eliminacion en Posicion Especifica");
    dadaUnaListaNULL_alIntentarQuitarUnElementoEnUnaPosicionEspecifica_devuelveNULL();
    dadaUnaListaVacia_alIntentarQuitarUnElementoEnUnaPosicionEspecifica_devuelveNULL();
    dadaUnaListaConVariosElementos_alQuitarUnElementoEnUnaPosicionEspecificaExitosamente_devuelveElElemento();
    dadaUnaListaConVariosElementos_alQuitarElElementoEnLaPosicion0Exitosamente_seEliminaElPrimerElemento();
    dadaUnaListaCon5Elementos_alQuitarUnElementoEnUnaPosicionEspecificaExitosamente_laListaQuedaConTamanio4();

    pa2m_nuevo_grupo("Prueba Lista: Acceso Elementos");
    dadaUnaListaNULL_alAccederAUnElemento_devuelveNULL();
    dadaUnaListaVacia_alAccederAUnElemento_devuelveNULL();
    dadaUnaLista_alAccederAUnElementoEnUnaPosicionInvalida_devuelveNULL();
    dadaUnaLista_alAccederAlElementoEnLaPoscion0Exitosamente_devuelveElPrimerElemento();
    dadaUnaLista_alAccederAlElementoEnLaPoscionFinalExitosamente_devuelveElUltimoElemento();
    dadaUnaListaConVariosElementos_alAccederAlElementoEnUnaPosicionEspecificaExitosamente_devuelveElElemento();

    pa2m_nuevo_grupo("Prueba Lista: Iterador");
    puedoCrearYDestruirUnIterador();
    dadaUnaListaVacia_puedoCrearYDestruirUnIterador();
    dadaUnaListaConVariosElementos_puedoCrearYDestruirUnIterador();
    dadaUnaListaConVariosElementos_alIterarConUnIteradorExterno_elIteradorAvanzaSobreLosElementosCorrectamente();
    dadaUnaListaConVariosElementos_alIterarConUnIteradorExternoHastaElFinalDeLaListaEIntentarIterador_elIteradorNoAvanza();

    pa2m_nuevo_grupo("Prueba Cola: Creacion");
    puedoCrearYDestruirUnaCola();

    pa2m_nuevo_grupo("Prueba Cola: Encolado");
    dadaUnaColaNULL_alIntentarEncolarUnElemento_devuelveNULL();
    dadaUnaColaVacia_alEncolar10ElementosExitosamente_laColaQuedaConTamanio10();
    dadaUnaColaCon1ElementoEnElFrente_alEncolarMultiplesElementosExitosamente_elPrimerElementoSigueEnElFrente();

    pa2m_nuevo_grupo("Prueba Cola: Desencolado");
    dadaUnaColaNULL_alIntentarDesencolarUnElemento_devuelveNULL();
    dadaUnaColaVacia_alIntentarDesencolarUnElemento_devuelveNULL();
    dadaUnaColaConVariosElementos_alDesencolarUnElementoExitosamente_devuelveElElemento();
    dadaUnaColaConVariosElementos_alDesencolarUnElementoExitosamente_elSegundoElementoEncoladoAhoraEsElFrente();
    dadaUnaColaCon5Elementos_alDesencolar1ElementoExitosamente_laColaQuedaConTamanio4();
    dadaUnaColaConVariosElementos_alDesencolarTodosElementosExitosamente_laColaQuedaVacia();

    pa2m_nuevo_grupo("Prueba Cola: Acceso Frente");
    dadaUnaColaNULL_alAccederAlElementoDelFrente_devuelveNULL();
    dadaUnaColaVacia_alAccederAlElementoDelFrente_devuelveNULL();
    dadaUnaColaConVariosElementos_alAccederAlElementoDelFrente_devuelveElElemento();

    pa2m_nuevo_grupo("Prueba Pila: Creacion");
    puedoCrearYDestruirUnaPila();

    pa2m_nuevo_grupo("Prueba Pila: Apilado");
    dadaUnaPilaNULL_alIntentarApilarUnElemento_devuelveNULL();
    dadaUnaPilaVacia_alApilar10ElementosExitosamente_laPilaQuedaConTamanio10();
    dadaUnaPilaConVariosElementos_alApilarUnElementoExitosamente_eseElementoPasaASerElTope();

    pa2m_nuevo_grupo("Prueba Pila: Desapilado");
    dadaUnaPilaNULL_alIntentarDesapilarUnElemento_devuelveNULL();
    dadaUnaPilaVacia_alIntentarDesapilarUnElemento_devuelveNULL();
    dadaUnaPilaConVariosElementos_alDesapilarUnElementoExitosamente_devuelveElElemento();
    dadaUnaPilaConVariosElementos_alDesapilarUnElementoExitosamente_elPenultimoElementoEncoladoAhoraEsElTope();
    dadaUnaPilaCon5Elementos_alDesapilar1ElementoExitosamente_laPilaQuedaConTamanio4();
    dadaUnaPilaConVariosElementos_alDesapilarTodosElementosExitosamente_laPilaQuedaVacia();

    pa2m_nuevo_grupo("Prueba Pila: Acceso Tope");
    dadaUnaPilaNULL_alAccederAlElementoDelTope_devuelveNULL();
    dadaUnaPilaVacia_alAccederAlElementoDelTope_devuelveNULL();
    dadaUnaPilaConVariosElementos_alAccederAlElementoDelTope_devuelveElElemento();

    pa2m_mostrar_reporte();
}
