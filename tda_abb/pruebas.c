#include "src/abb.h"
#include "pa2mm.h"
#include "string.h"

// Auxiliares para pruebas

typedef struct estudiante_fiuba {
	unsigned padron;
	char* nombre;
} estudiante_fiuba_t;

estudiante_fiuba_t*
crear_estudiante_fiuba(unsigned padron, char* nombre) {
	estudiante_fiuba_t* est = malloc(sizeof(estudiante_fiuba_t));
	est -> padron = padron;
	est -> nombre = nombre;
	return est;
}

bool
a_cada_estudiante_fiuba(void* _est, void* aux) {
	return true;
}

bool
a_cada_estudiante_hasta_padron_22(void* _est, void* aux) {
	estudiante_fiuba_t* est = _est;
	return !(est -> padron == 22);
}

void  
destructor_estudiante_fiuba(void* _est) {
	free(_est);
}

int
comparador_estudiantes_fiuba(void* _est_1, void* _est_2)
{
	estudiante_fiuba_t* est_1 = _est_1;
	estudiante_fiuba_t* est_2 = _est_2;

	if (est_1 -> padron == est_2 -> padron)
		return 0;

	return est_1 -> padron < est_2 -> padron ? -1 : 1;
}

// Pruebas: Creacion y Destruccion

void
puedoCrearYDestruirUnABB()
{
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	pa2m_afirmar(arbol -> nodo_raiz == NULL, "Para un arbol nuevo, la raiz es NULL");
	pa2m_afirmar(arbol -> comparador == comparador_estudiantes_fiuba, "Para un arbol nuevo, el comparador es el correcto");
	pa2m_afirmar(abb_vacio(arbol), "Un arbol se crea vacio");

	abb_destruir(arbol);
}

void
dadoUnComparadorNULL_noPuedoCrearUnABB() {
	abb_t* arbol = abb_crear(NULL);

	pa2m_afirmar(arbol == NULL, "No puedo crear un arbol con un comparador NULL");

	abb_destruir(arbol);
}

void
dadoUnABB_puedoDestruirTodosLosElementos() {
    abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
	estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
	estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
	estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
	estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
	estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
	estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
	estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	arbol = abb_insertar(arbol, est_3);
	arbol = abb_insertar(arbol, est_4);
	arbol = abb_insertar(arbol, est_5);
	arbol = abb_insertar(arbol, est_6);
	arbol = abb_insertar(arbol, est_7);
	arbol = abb_insertar(arbol, est_8);
	arbol = abb_insertar(arbol, est_9);

    abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

// Pruebas: Insertar

void
dadoUnABBNULL_alInsertarUnElemento_seRetornaNULL() {
	abb_t* arbol = NULL;

	pa2m_afirmar(abb_insertar(arbol, NULL) == NULL, "Al insertar un elemento en un arbol NULL, se retorna NULL");

	abb_destruir(arbol);
}

void
dadoUnABBVacio_alInsertarElPrimerElemento_eseElementoPasaASerLaRaiz()
{
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");

	arbol = abb_insertar(arbol, est_0);

	pa2m_afirmar(arbol -> nodo_raiz -> elemento == est_0, "Al insertar el primer elemento en un arbol vacio, este pasa a ser la raiz");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alInsetarUnElemento_seRetornaElABB() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	pa2m_afirmar(abb_insertar(arbol, NULL), "Al insertar un elemento se retorna el arbol");

	abb_destruir(arbol);
}

void
dadoUnABB_alInsertarVariosElementos_elTamanioDelABBIncrementaCorrectamente() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");

	arbol = abb_insertar(arbol, est_0);
	pa2m_afirmar(abb_tamanio(arbol) == 1, "Luego de insertar 1 elemento, el tamanio del ABB es 1");

	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	pa2m_afirmar(abb_tamanio(arbol) == 3, "Luego de insertar 2 elementos mas, el tamanio del ABB es 3");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

// Pruebas: Busqueda

void
dadoUnABBNULL_alBuscarUnElementoDelABB_seRetornaNULL() {
	abb_t* arbol = NULL;

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	arbol = abb_insertar(arbol, est_0);

	pa2m_afirmar(abb_buscar(arbol, est_0) == NULL, "Al buscar un elemento en un ABB NULL, se retorna NULL");

	destructor_estudiante_fiuba(est_0);
	abb_destruir(arbol);
}

void
dadoUnABB_alBuscarUnElementoDelABB_seRetornaElElemento() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	arbol = abb_insertar(arbol, est_0);

	pa2m_afirmar(abb_buscar(arbol, est_0) == est_0, "Al buscar y encontrar un elemento en el ABB, se retorna ese elemento");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alBuscarUnElementoQueNoEstaEnElABB_seRetornaNULL() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");

	pa2m_afirmar(abb_buscar(arbol, est_0) == NULL, "Al buscar un elemento que no esta en el ABB, se retorna NULL")

	destructor_estudiante_fiuba(est_0);
	abb_destruir(arbol);
}

// Pruebas: Recorrer

void
dadoUnABBNULL_alRecorrerElABB_seRecorrenCeroElementos() {
	abb_t* arbol = NULL;

	const size_t MAX_ELEMENTOS = 10;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	void* recorridos[MAX_ELEMENTOS];
	size_t cantidad_recorridos = abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

	pa2m_afirmar(cantidad_recorridos == 0, "Al recorrer un ABB NULL, la cantidad de elementos recorridos es 0");

	abb_destruir(arbol);
}

void
dadoUnABBVacio_alRecorrerElABB_seRecorrenCeroElementos() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	const size_t MAX_ELEMENTOS = 10;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	void* recorridos[MAX_ELEMENTOS];
	size_t cantidad_recorridos = abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

	pa2m_afirmar(cantidad_recorridos == 0, "Al recorrer un ABB vacio, la cantidad de elementos recorridos es 0");

	abb_destruir(arbol);
}

void
dadoUnABBYUnVectorNULL_alRecorrerElABB_seRecorrenCeroElementos() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);

	const size_t MAX_ELEMENTOS = 3;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	size_t cantidad_recorridos = abb_recorrer(arbol, PREORDEN, NULL, MAX_RECORRIDOS);

	pa2m_afirmar(cantidad_recorridos == 0, "Al hacer el recorrido de un ABB con un vector NULL, la cantidad de elementos recorridos es 0");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alRecorrerTodosLosElementos_laCantidadDeElementosRecorridosEsIgualAlTamanioDelABB() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);

	const size_t MAX_ELEMENTOS = 3;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	void* recorridos[MAX_ELEMENTOS];
	size_t cantidad_recorridos = abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

	pa2m_afirmar(cantidad_recorridos == 3, "Al recorrer todos los elementos de un ABB, la cantidad de elementos es igual al tamanio del ABB");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alRecorrerUnaCantidadMenorAlTamanioDelABB_seRecorrenSoloEsosElementos() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
	estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
	estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
	estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
	estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
	estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
	estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
	estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	arbol = abb_insertar(arbol, est_3);
	arbol = abb_insertar(arbol, est_4);
	arbol = abb_insertar(arbol, est_5);
	arbol = abb_insertar(arbol, est_6);
	arbol = abb_insertar(arbol, est_7);
	arbol = abb_insertar(arbol, est_8);
	arbol = abb_insertar(arbol, est_9);

	const size_t MAX_ELEMENTOS = 10;
	size_t MAX_RECORRIDOS = 5;

	void* recorridos[MAX_ELEMENTOS];
	size_t cantidad_recorridos = abb_recorrer(arbol, POSTORDEN, recorridos, MAX_RECORRIDOS);

	pa2m_afirmar(cantidad_recorridos == MAX_RECORRIDOS, "Al recorrer solo 5 elementos de un ABB de 10 elementos, la cantidad de elementos recorridos es 5");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alRecorrerUnaCantidadMayorAlTamanioDelABB_laCantidadDeElementosRecorridosEsIgualAlTamanioDelABB() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
	estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
	estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	arbol = abb_insertar(arbol, est_3);
	arbol = abb_insertar(arbol, est_4);

	const size_t MAX_ELEMENTOS = 5;
	size_t MAX_RECORRIDOS = 10;

	void* recorridos[MAX_ELEMENTOS];
	size_t cantidad_recorridos = abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

	pa2m_afirmar(cantidad_recorridos == MAX_ELEMENTOS, "Al intentar recorrer 10 elementos de un ABB de solo 5 elementos, la cantidad de elementos recorridos es 5");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alRecorrerElABB_seObtienenLosRecorridosEsperados() {
    abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

    estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
    estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
    estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
    estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
    estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
    estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
    estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
    estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
    estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
    estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

    arbol = abb_insertar(arbol, est_0);
    arbol = abb_insertar(arbol, est_1);
    arbol = abb_insertar(arbol, est_2);
    arbol = abb_insertar(arbol, est_3);
    arbol = abb_insertar(arbol, est_4);
    arbol = abb_insertar(arbol, est_5);
    arbol = abb_insertar(arbol, est_6);
    arbol = abb_insertar(arbol, est_7);
    arbol = abb_insertar(arbol, est_8);
    arbol = abb_insertar(arbol, est_9);

    const size_t MAX_ELEMENTOS = 10;
    size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

    void* recorridos[MAX_ELEMENTOS];
	abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

    bool recorrido_correcto =
        (recorridos[0] == est_0) && (recorridos[1] == est_1) &&
        (recorridos[2] == est_3) && (recorridos[3] == est_7) &&
        (recorridos[4] == est_8) && (recorridos[5] == est_4) &&
        (recorridos[6] == est_2) && (recorridos[7] == est_5) &&
        (recorridos[8] == est_9) && (recorridos[9] == est_6);

    pa2m_afirmar(recorrido_correcto, "Los elementos se recorren en el orden PREORDEN correctamente");

	abb_recorrer(arbol, INORDEN, recorridos, MAX_RECORRIDOS);

	recorrido_correcto =
		(recorridos[0] == est_7) && (recorridos[1] == est_3) &&
		(recorridos[2] == est_8) && (recorridos[3] == est_1) &&
		(recorridos[4] == est_4) && (recorridos[5] == est_0) &&
		(recorridos[6] == est_9) && (recorridos[7] == est_5) &&
		(recorridos[8] == est_2) && (recorridos[9] == est_6);

    pa2m_afirmar(recorrido_correcto, "Los elementos se recorren en el orden INORDEN correctamente");

	abb_recorrer(arbol, POSTORDEN, recorridos, MAX_RECORRIDOS);

	recorrido_correcto =
		(recorridos[0] == est_7) && (recorridos[1] == est_8) &&
		(recorridos[2] == est_3) && (recorridos[3] == est_4) &&
		(recorridos[4] == est_1) && (recorridos[5] == est_9) &&
		(recorridos[6] == est_5) && (recorridos[7] == est_6) &&
		(recorridos[8] == est_2) && (recorridos[9] == est_0);

    pa2m_afirmar(recorrido_correcto, "Los elementos se recorren en el orden POSTORDEN correctamente");

    abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

// Pruebas: Con Cada Elemento

void
dadoUnABBNULL_alAplicarUnaFuncionACadaElemento_seRecorrenCeroElementos() {
	abb_t* arbol = NULL;

	size_t cantidad_recorridos = abb_con_cada_elemento(arbol, INORDEN, a_cada_estudiante_fiuba, NULL);

	pa2m_afirmar(cantidad_recorridos == 0, "Al aplicar un funcion a cada elemento de un ABB NULL, la cantidad de elementos recorridos es 0");

	abb_destruir(arbol);
}

void
dadoUnABB_alAplicarUnaFuncionACadaElemento_seAplicaCorrectamente() {
    abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

    estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
    estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
    estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
    estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
    estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
    estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
    estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
    estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
    estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
    estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

    arbol = abb_insertar(arbol, est_0);
    arbol = abb_insertar(arbol, est_1);
    arbol = abb_insertar(arbol, est_2);
    arbol = abb_insertar(arbol, est_3);
    arbol = abb_insertar(arbol, est_4);
    arbol = abb_insertar(arbol, est_5);
    arbol = abb_insertar(arbol, est_6);
    arbol = abb_insertar(arbol, est_7);
    arbol = abb_insertar(arbol, est_8);
    arbol = abb_insertar(arbol, est_9);

	const size_t MAX_RECORRIDOS = 10;

	size_t cantidad_recorridos = abb_con_cada_elemento(arbol, PREORDEN, a_cada_estudiante_fiuba, NULL);
	pa2m_afirmar(cantidad_recorridos == MAX_RECORRIDOS, "Al aplicar una funcion en un recorrido PREORDEN, la cantidad de elementos a los que se les aplica la funcion es igual al total de elementos");

	cantidad_recorridos = abb_con_cada_elemento(arbol, INORDEN, a_cada_estudiante_fiuba, NULL);
	pa2m_afirmar(cantidad_recorridos == MAX_RECORRIDOS, "Al aplicar una funcion en un recorrido INORDEN, la cantidad de elementos a los que se les aplica la funcion es igual al total de elementos");

	cantidad_recorridos = abb_con_cada_elemento(arbol, POSTORDEN, a_cada_estudiante_fiuba, NULL);
	pa2m_afirmar(cantidad_recorridos == MAX_RECORRIDOS, "Al aplicar una funcion en un recorrido POSTORDEN, la cantidad de elementos a los que se les aplica la funcion es igual al total de elementos");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alAplicarUnaFuncionQueRetornaFalseAUnElemento_elRecorridoSeDetieneEnLaCantidadDeRecorridosCorrecta() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

    estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
    estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
    estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
    estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
    estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
    estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
    estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
    estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
    estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
    estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

    arbol = abb_insertar(arbol, est_0);
    arbol = abb_insertar(arbol, est_1);
    arbol = abb_insertar(arbol, est_2);
    arbol = abb_insertar(arbol, est_3);
    arbol = abb_insertar(arbol, est_4);
    arbol = abb_insertar(arbol, est_5);
    arbol = abb_insertar(arbol, est_6);
    arbol = abb_insertar(arbol, est_7);
    arbol = abb_insertar(arbol, est_8);
    arbol = abb_insertar(arbol, est_9);

	size_t cantidad_recorridos = abb_con_cada_elemento(arbol, PREORDEN, a_cada_estudiante_hasta_padron_22, NULL);
	pa2m_afirmar(cantidad_recorridos == 6, "Al aplicar una funcion en un recorrido PREORDEN hasta cierto elemento, el recorrido se detiene en la cantidad de elementos correcta");

	cantidad_recorridos = abb_con_cada_elemento(arbol, INORDEN, a_cada_estudiante_hasta_padron_22, NULL);
	pa2m_afirmar(cantidad_recorridos == 5, "Al aplicar una funcion en un recorrido INORDEN hasta cierto elemento, el recorrido se detiene en la cantidad de elementos correcta");

	cantidad_recorridos = abb_con_cada_elemento(arbol, POSTORDEN, a_cada_estudiante_hasta_padron_22, NULL);
	pa2m_afirmar(cantidad_recorridos == 4, "Al aplicar una funcion en un recorrido POSTORDEN hasta cierto elemento, el recorrido se detiene en la cantidad de elementos correcta");

	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

// Pruebas: Quitar

void
dadoUnABBNULL_alQuitarUnElemento_seRetornaNULL() {
	abb_t* arbol = NULL;

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");

	pa2m_afirmar(abb_quitar(arbol, est_0) == NULL, "Al quitar un elemento de un ABB NULL, se retorna NULL");

	destructor_estudiante_fiuba(est_0);
	abb_destruir(arbol);
}

void
dadoUnABB_alQuitarUnElementoDelABB_seQuitaCorrectamente() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);

	size_t tamanio_original = abb_tamanio(arbol);

	pa2m_afirmar(abb_quitar(arbol, est_2) == est_2, "Al quitar un elemento que del ABB, se retorna ese elemento");
	pa2m_afirmar(abb_tamanio(arbol) == tamanio_original - 1, "Al quitar un elemento el tamanio del ABB se reduce en 1");
	pa2m_afirmar(abb_buscar(arbol, est_2) == NULL, "Al buscar un elemento que se quito del ABB, ya no se encuentra");

	destructor_estudiante_fiuba(est_2);
	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alQuitarUnElementoQueNoEstaEnElABB_seRetornaNULL() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);

	pa2m_afirmar(abb_quitar(arbol, est_2) == NULL, "Al intentar quitar un elemento que no esta en el ABB, se retorna NULL");

	destructor_estudiante_fiuba(est_2);
	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alQuitarUnElementoEnUnNodoHoja_seQuitaCorrectamente() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);

	abb_quitar(arbol, est_2);

	const size_t MAX_ELEMENTOS = 3;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	void* recorridos[MAX_ELEMENTOS];
	abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

    bool recorrido_correcto = (recorridos[0] == est_0) && (recorridos[1] == est_1);

	pa2m_afirmar(recorrido_correcto, "Al quitar un elemento de un nodo hoja, se elimina correctamente y el ABB sigue siendo un ABB");

	destructor_estudiante_fiuba(est_2);
	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alQuitarUnElementoEnUnNodoConUnHijo_seQuitaCorrectamente() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
	estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
	estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
	estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
	estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
	estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
	estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
	estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	arbol = abb_insertar(arbol, est_3);
	arbol = abb_insertar(arbol, est_4);
	arbol = abb_insertar(arbol, est_5);
	arbol = abb_insertar(arbol, est_6);
	arbol = abb_insertar(arbol, est_7);
	arbol = abb_insertar(arbol, est_8);
	arbol = abb_insertar(arbol, est_9);

	abb_quitar(arbol, est_5);

	const size_t MAX_ELEMENTOS = 10;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	void* recorridos[MAX_ELEMENTOS];
	abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

    bool recorrido_correcto =
        (recorridos[0] == est_0) && (recorridos[1] == est_1) &&
        (recorridos[2] == est_3) && (recorridos[3] == est_7) &&
        (recorridos[4] == est_8) && (recorridos[5] == est_4) &&
        (recorridos[6] == est_2) && (recorridos[7] == est_9) &&
		(recorridos[8] == est_6);

	pa2m_afirmar(recorrido_correcto, "Al quitar un elemento de un nodo con 1 hijo, se elimina correctamente y el ABB sigue siendo un ABB");

	destructor_estudiante_fiuba(est_5);
	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alQuitarUnElementoEnUnNodoConDosHijos_seQuitaCorrectamente() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
	estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
	estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
	estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
	estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
	estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
	estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
	estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	arbol = abb_insertar(arbol, est_3);
	arbol = abb_insertar(arbol, est_4);
	arbol = abb_insertar(arbol, est_5);
	arbol = abb_insertar(arbol, est_6);
	arbol = abb_insertar(arbol, est_7);
	arbol = abb_insertar(arbol, est_8);
	arbol = abb_insertar(arbol, est_9);

	abb_quitar(arbol, est_2);

	const size_t MAX_ELEMENTOS = 10;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;
 
	void* recorridos[MAX_ELEMENTOS];
	abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);
 
    bool recorrido_correcto =
        (recorridos[0] == est_0) && (recorridos[1] == est_1) &&
        (recorridos[2] == est_3) && (recorridos[3] == est_7) &&
        (recorridos[4] == est_8) && (recorridos[5] == est_4) &&
        (recorridos[6] == est_5) && (recorridos[7] == est_9) &&
		(recorridos[8] == est_6);
 
	pa2m_afirmar(recorrido_correcto, "Al quitar un elemento de un nodo con 2 hijos, se elimina correctamente y el ABB sigue siendo un ABB");

	destructor_estudiante_fiuba(est_2);
	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

void
dadoUnABB_alQuitarElElementoEnElNodoRaiz_seQuitaCorrectamente() {
	abb_t* arbol = abb_crear(comparador_estudiantes_fiuba);

	estudiante_fiuba_t* est_0 = crear_estudiante_fiuba(25, "Alejandro Schamun");
	estudiante_fiuba_t* est_1 = crear_estudiante_fiuba(20, "Cami Fiorotto");
	estudiante_fiuba_t* est_2 = crear_estudiante_fiuba(36, "Carolina Aramay");
	estudiante_fiuba_t* est_3 = crear_estudiante_fiuba(10, "Facundo Sanso");
	estudiante_fiuba_t* est_4 = crear_estudiante_fiuba(22, "Joaquin Dopazo");
	estudiante_fiuba_t* est_5 = crear_estudiante_fiuba(30, "Julian Calderon");
	estudiante_fiuba_t* est_6 = crear_estudiante_fiuba(40, "Julian Stiefkens");
	estudiante_fiuba_t* est_7 = crear_estudiante_fiuba(5, "Manuel Sanchez");
	estudiante_fiuba_t* est_8 = crear_estudiante_fiuba(12, "Nicolas Celano");
	estudiante_fiuba_t* est_9 = crear_estudiante_fiuba(28, "Nicolas Tonizzo");

	arbol = abb_insertar(arbol, est_0);
	arbol = abb_insertar(arbol, est_1);
	arbol = abb_insertar(arbol, est_2);
	arbol = abb_insertar(arbol, est_3);
	arbol = abb_insertar(arbol, est_4);
	arbol = abb_insertar(arbol, est_5);
	arbol = abb_insertar(arbol, est_6);
	arbol = abb_insertar(arbol, est_7);
	arbol = abb_insertar(arbol, est_8);
	arbol = abb_insertar(arbol, est_9);

	abb_quitar(arbol, est_5);

	const size_t MAX_ELEMENTOS = 10;
	size_t MAX_RECORRIDOS = MAX_ELEMENTOS;

	void* recorridos[MAX_ELEMENTOS];
	abb_recorrer(arbol, PREORDEN, recorridos, MAX_RECORRIDOS);

	bool recorrido_correcto =
		(recorridos[0] == est_0) && (recorridos[1] == est_1) &&
		(recorridos[2] == est_3) && (recorridos[3] == est_7) &&
		(recorridos[4] == est_8) && (recorridos[5] == est_4) &&
		(recorridos[6] == est_2) && (recorridos[7] == est_9) &&
		(recorridos[8] == est_6);

	pa2m_afirmar(recorrido_correcto, "Al quitar un elemento de un nodo con 1 hijo, se elimina correctamente y el ABB sigue siendo un ABB");

	destructor_estudiante_fiuba(est_5);
	abb_destruir_todo(arbol, destructor_estudiante_fiuba);
}

int
main() {
    pa2m_nuevo_grupo("Pruebas: Creacion y Destruccion");
	puedoCrearYDestruirUnABB();
	dadoUnComparadorNULL_noPuedoCrearUnABB();
	dadoUnABB_puedoDestruirTodosLosElementos();

    pa2m_nuevo_grupo("Pruebas: Insertar");
	dadoUnABBNULL_alInsertarUnElemento_seRetornaNULL();
	dadoUnABBVacio_alInsertarElPrimerElemento_eseElementoPasaASerLaRaiz();
	dadoUnABB_alInsetarUnElemento_seRetornaElABB();
	dadoUnABB_alInsertarVariosElementos_elTamanioDelABBIncrementaCorrectamente();

	pa2m_nuevo_grupo("Pruebas: Busqueda");
	dadoUnABBNULL_alBuscarUnElementoDelABB_seRetornaNULL();
	dadoUnABB_alBuscarUnElementoDelABB_seRetornaElElemento();
	dadoUnABB_alBuscarUnElementoQueNoEstaEnElABB_seRetornaNULL();

	pa2m_nuevo_grupo("Pruebas: Recorrer");
	dadoUnABBNULL_alRecorrerElABB_seRecorrenCeroElementos();
	dadoUnABBVacio_alRecorrerElABB_seRecorrenCeroElementos();
	dadoUnABBYUnVectorNULL_alRecorrerElABB_seRecorrenCeroElementos();
	dadoUnABB_alRecorrerTodosLosElementos_laCantidadDeElementosRecorridosEsIgualAlTamanioDelABB();
	dadoUnABB_alRecorrerUnaCantidadMenorAlTamanioDelABB_seRecorrenSoloEsosElementos();
	dadoUnABB_alRecorrerUnaCantidadMayorAlTamanioDelABB_laCantidadDeElementosRecorridosEsIgualAlTamanioDelABB();
	dadoUnABB_alRecorrerElABB_seObtienenLosRecorridosEsperados();

	pa2m_nuevo_grupo("Pruebas: Con Cada Elemento");
	dadoUnABBNULL_alAplicarUnaFuncionACadaElemento_seRecorrenCeroElementos();
	dadoUnABB_alAplicarUnaFuncionACadaElemento_seAplicaCorrectamente();
	dadoUnABB_alAplicarUnaFuncionQueRetornaFalseAUnElemento_elRecorridoSeDetieneEnLaCantidadDeRecorridosCorrecta();

	pa2m_nuevo_grupo("Pruebas: Quitar");
	dadoUnABBNULL_alQuitarUnElemento_seRetornaNULL();
	dadoUnABB_alQuitarUnElementoDelABB_seQuitaCorrectamente();
	dadoUnABB_alQuitarUnElementoQueNoEstaEnElABB_seRetornaNULL();
	dadoUnABB_alQuitarUnElementoEnUnNodoHoja_seQuitaCorrectamente();
	dadoUnABB_alQuitarUnElementoEnUnNodoConUnHijo_seQuitaCorrectamente();
	dadoUnABB_alQuitarUnElementoEnUnNodoConDosHijos_seQuitaCorrectamente();
	dadoUnABB_alQuitarElElementoEnElNodoRaiz_seQuitaCorrectamente();

    return pa2m_mostrar_reporte();
}
