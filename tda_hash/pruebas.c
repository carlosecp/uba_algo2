#include "pa2mm.h"
#include "src/hash.h"
#include "./src/casilla.h"
#include <string.h>

#define EXITO 0
#define ERROR -1

struct hash {
	casilla_t** casillas;
	size_t cantidad_casillas;
	size_t cantidad_elementos;
	hash_destruir_dato_t destruir_elemento;
};

						 /* Auxiliares para pruebas */

typedef struct {
	size_t padron;
	char* nombre;
} estudiante_t;

estudiante_t* crear_estudiante(size_t padron, char* nombre) {
	estudiante_t* est = malloc(sizeof(estudiante_t));
	est->padron = padron;
	est->nombre = nombre;
	return est;
}

void destruir_estudiante(void* est) {
	free(est);
}

bool reprobar_estudiante(hash_t* hash, const char* clave, void* aux) {
	return false;
}

bool reprobar_hasta_septimo_estudiante(hash_t* hash, const char* clave, void* aux) {
	return (strcmp(clave, "septimo") == 0);
}

// === BORRAR ===

void imprimir_lista(casilla_t* lista) {
	if (!lista)
		return;

	printf("%i\n", *(int*)lista->elemento);
	imprimir_lista(lista->siguiente);
}

void imprimir_casilla(casilla_t* casilla) {
	if (!casilla)
		return;

	estudiante_t est = *(estudiante_t*)casilla->elemento;
	printf(" {%s: {%s, %li}} ", casilla->clave, est.nombre, est.padron);
	imprimir_casilla(casilla->siguiente);
}

void imprimir_hash(hash_t* hash) {
	for (size_t i = 0; i < hash->cantidad_casillas; i++) {
		if (hash->casillas[i] == NULL) {
			printf("\t%li\t---\n", i);
		} else {
			printf("\t%li\t", i);
			imprimir_casilla(hash->casillas[i]);
			printf("\n");
		}
	}
}

						  /* Pruebas Hash: Creacion */

void dadoUnDestructorYUnValorInicialValido_alCrearUnHash_seRetornaUnHashCon0Elementos() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);
	pa2m_afirmar(hash != NULL, "Al crear un hash con un destructor y valor inicial valido, se retorna un hash no NULL");
	pa2m_afirmar(hash_cantidad(hash) == 0, "Al crear un hash la cantidad de elementos es 0");
	hash_destruir(hash);
}

void dadoUnDestructorNULL_alDestruirUnHash_seDestruyeCorrectamenteSinDestruirLosElementos() {
	hash_t* hash = hash_crear(NULL, 5);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	hash_insertar(hash, "primero", est0);
	hash_destruir(hash);

	pa2m_afirmar(est0 != NULL, "Al destruir un hash con un destructor NULL, solo se destruye el hash, no los elementos");

	if (est0)
		destruir_estudiante(est0);
}

						  /* Pruebas Hash: Insertar */

void dadoUnHashNULL_alInsertarUnElemento_seRetornaError() {
	hash_t* hash = NULL;
	pa2m_afirmar(hash_insertar(hash, "primero", NULL) == ERROR, "Al insertar en un hash NULl se retorna ERROR (-1)");
	hash_destruir(hash);
}

void dadaUnaClaveNULL_alInsertarUnElemento_seRetornaError() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);
	pa2m_afirmar(hash_insertar(hash, NULL, NULL) == ERROR, "Al insertar un elemento con clave NULL se retorna ERROR (-1)");
	hash_destruir(hash);
}

void dadoUnHash_alInsertarVariosElementos_seInsertanCorrectamente() {
	hash_t* hash = hash_crear(destruir_estudiante, 10);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	estudiante_t* est1 = crear_estudiante(20, "Cami Fiorotto");
	estudiante_t* est2 = crear_estudiante(36, "Carolina Aramay");
	estudiante_t* est3 = crear_estudiante(10, "Facundo Sanso");
	estudiante_t* est4 = crear_estudiante(22, "Joaquin Dopazo");

	pa2m_afirmar(hash_insertar(hash, "primero", est0) == EXITO, "Al insertar un elemento correctamente se retorna EXITO (0)");

	hash_insertar(hash, "segundo", est1);
	hash_insertar(hash, "tercero", est2);
	hash_insertar(hash, "cuarto",  est3);
	hash_insertar(hash, "quinto",  est4);

	pa2m_afirmar(hash_cantidad(hash) == 5, "Al insertar 5 elementos en un hash la cantidad de elementos es 5");

	estudiante_t* est5 = crear_estudiante(30, "Julian Calderon");
	estudiante_t* est6 = crear_estudiante(40, "Julian Stiefkens");
	estudiante_t* est7 = crear_estudiante(5,  "Manuel Sanchez");
	estudiante_t* est8 = crear_estudiante(12, "Nicolas Celano");
	estudiante_t* est9 = crear_estudiante(28, "Nicolas Tonizzo");

	hash_insertar(hash, "sexto",   est5);
	hash_insertar(hash, "septimo", est6);
	hash_insertar(hash, "octavo",  est7);
	hash_insertar(hash, "noveno",  est8);
	hash_insertar(hash, "decimo",  est9);
	
	pa2m_afirmar(hash_cantidad(hash) == 10, "Luego de insertar varios elementos y hacer rehash, la cantidad de elementos siguie siendo la correcta");

	hash_destruir(hash);
}

void dadoUnHash_alInsertarUnElementoConUnaClaveRepetida_seInsertaCorrectamente() {
	hash_t* hash = hash_crear(destruir_estudiante, 10);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	estudiante_t* est1 = crear_estudiante(20, "Cami Fiorotto");

	hash_insertar(hash, "primero", est0);

	pa2m_afirmar(hash_insertar(hash, "primero", est1) == EXITO, "Al insertar un elemento con clave repetida se retorna EXITO (0)");
	pa2m_afirmar(hash_cantidad(hash) == 1, "Al insertar un elemento con clave repetida la cantidad de elementos en el hash se mantiene igual");

	hash_destruir(hash);
}

                           /* Pruebas Hash: Quitar */

void dadoUnHashNULL_alQuitarUnElemento_seRetornaError() {
	hash_t* hash = NULL;
	pa2m_afirmar(hash_quitar(hash, "clave"), "Al quitar en un hash NULL se devuelve ERROR (-1)");
	hash_destruir(hash);
}

void dadaUnaClaveNULL_alQuitarUnElemento_seRetornaError() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);
	pa2m_afirmar(hash_quitar(hash, NULL) == ERROR, "Al quitar un elemento con clave NULL se retorna ERROR (-1)");
	hash_destruir(hash);
}

void dadoUnHash_alQuitarVariosElementos_seQuitanCorrectamente() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	estudiante_t* est1 = crear_estudiante(20, "Cami Fiorotto");
	estudiante_t* est2 = crear_estudiante(36, "Carolina Aramay");
	estudiante_t* est3 = crear_estudiante(10, "Facundo Sanso");
	estudiante_t* est4 = crear_estudiante(22, "Joaquin Dopazo");
	estudiante_t* est5 = crear_estudiante(22, "Joaquin Dopazo");

	hash_insertar(hash, "primero", est0);

	pa2m_afirmar(hash_quitar(hash, "primero") == EXITO, "Al quitar un elemento correctamente se retorna EXITO (0)");
	pa2m_afirmar(hash_cantidad(hash) == 0, "Al quitar un elemento la cantidad de elementos disminuye");
	pa2m_afirmar(hash_quitar(hash, "no_existe") == ERROR, "Al quitar un elemento con clave que no existe se retorna ERROR (-1)");
	pa2m_afirmar(hash_cantidad(hash) == 0, "Al quitar un elemento con clave que no existe la cantidad no se disminuye");

	hash_insertar(hash, "segundo", est1);
	hash_insertar(hash, "tercero", est2);
	hash_insertar(hash, "cuarto",  est3);
	hash_insertar(hash, "quinto",  est4);
	hash_insertar(hash, "sexto",   est5);

	hash_quitar(hash, "segundo");
	hash_quitar(hash, "tercero");
	hash_quitar(hash, "cuarto");
	hash_quitar(hash, "quinto");
	hash_quitar(hash, "sexto");

	pa2m_afirmar(hash_cantidad(hash) == 0, "Al quitar todos los elementos la cantidad de elementos es 0");

	hash_destruir(hash);
}

						  /* Pruebas Hash: Obtener */

void dadoUnHashNULL_alObtenerUnElemento_seRetornaNULL() {
	hash_t* hash = NULL;
	pa2m_afirmar(hash_obtener(hash, "no_existe") == false, "Al obtener un elemento en un hash NULL se retorna NULL");
	hash_destruir(hash);
}

void dadaUnaClaveNULL_alObtenerUnElemento_seRetornaNULL() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);
	pa2m_afirmar(hash_obtener(hash, NULL) == false, "Al obtener un elemento con clave NULL se retorna NULL");
	hash_destruir(hash);
}

void dadoUnHash_alObtenerUnElemento_seObtieneCorrectamente() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	hash_insertar(hash, "primero", est0);

	pa2m_afirmar(hash_obtener(hash, "primero") == est0, "Al obtener un elemento por su clave se retorna el elemento correcto");
	pa2m_afirmar(hash_obtener(hash, "no_existe") == NULL, "Al obtener un elemento que no existe se retorna NULL");

	hash_destruir(hash);
}

						  /* Pruebas Hash: Contiene */

void dadoUnHashNULL_alVerificarSiContieneUnElemento_seRetornaNULL() {
	hash_t* hash = NULL;
	pa2m_afirmar(hash_contiene(hash, "no_existe") == false, "Al verificar si un hash NULL contiene un elemento se retorna false");
	hash_destruir(hash);
}

void dadaUnaClaveNULL_alVerificarSiElHashContieneUnElemento_seRetornaNULL() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);
	pa2m_afirmar(hash_contiene(hash, NULL) == false, "Al verificar si un hash contiene un elemento con clave NULL se retorna false");
	hash_destruir(hash);
}

void dadoUnHash_alVerificarSiContieneUnElemento_seObtieneCorrectamente() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	hash_insertar(hash, "primero", est0);

	pa2m_afirmar(hash_obtener(hash, "primero") == est0, "Al verificar si un hash contiene un elemento insertado previamente se retorna true");
	pa2m_afirmar(hash_obtener(hash, "no_existe") == NULL, "Al verificar si un hash contiene un elemento que no existe se retorna false");

	hash_destruir(hash);
}

						  /* Pruebas Hash: Contiene */

void dadoUnHashNULL_alRecorrerLosElementos_seRetorna0() {
	hash_t* hash = NULL;
	pa2m_afirmar(hash_con_cada_clave(hash, reprobar_estudiante, NULL) == 0, "Al recorrer un hash NULL se retorna 0");
	hash_destruir(hash);
}

void dadoUnHash_alRecorrerLosElementos_seRetornaLaCantidadEsperada() {
	hash_t* hash = hash_crear(destruir_estudiante, 20);

	pa2m_afirmar(hash_con_cada_clave(hash, reprobar_estudiante, NULL) == 0, "Al recorrer un hash vacio se recorren 0 elementos");

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	estudiante_t* est1 = crear_estudiante(20, "Cami Fiorotto");
	estudiante_t* est2 = crear_estudiante(36, "Carolina Aramay");
	estudiante_t* est3 = crear_estudiante(10, "Facundo Sanso");
	estudiante_t* est4 = crear_estudiante(22, "Joaquin Dopazo");
	estudiante_t* est5 = crear_estudiante(30, "Julian Calderon");
	estudiante_t* est6 = crear_estudiante(40, "Julian Stiefkens");
	estudiante_t* est7 = crear_estudiante(5,  "Manuel Sanchez");
	estudiante_t* est8 = crear_estudiante(12, "Nicolas Celano");
	estudiante_t* est9 = crear_estudiante(28, "Nicolas Tonizzo");

	hash_insertar(hash, "primero", est0);
	hash_insertar(hash, "segundo", est1);
	hash_insertar(hash, "tercero", est2);
	hash_insertar(hash, "cuarto",  est3);
	hash_insertar(hash, "quinto",  est4);
	hash_insertar(hash, "sexto",   est5);
	hash_insertar(hash, "septimo", est6);
	hash_insertar(hash, "octavo",  est7);
	hash_insertar(hash, "noveno",  est8);
	hash_insertar(hash, "decimo",  est9);

	pa2m_afirmar(hash_con_cada_clave(hash, reprobar_estudiante, NULL) == 10, "Al recorrer un hash ininterrumpidamente se recorren todos los elementos");
	pa2m_afirmar(hash_con_cada_clave(hash, reprobar_hasta_septimo_estudiante, NULL) == 5, "Al recorrer un hash hasta llegar a cierta clave e interrumpir el recorrido, se recorren la cantidad de elementos correctos");
	pa2m_afirmar(hash_con_cada_clave(hash, NULL, NULL) == 10, "Al recorrer un hash con una funcion NULL se recorren todos los elementos");

	hash_destruir(hash);
}

					  /* Pruebas Hash: Copia de Claves */

void dadoUnHash_alInsertarUnaClaveQueLuegoPuedeSerModificada_seHaceUnaCopiaDeLaClave() {
	hash_t* hash = hash_crear(destruir_estudiante, 3);

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	estudiante_t* est1 = crear_estudiante(20, "Cami Fiorotto");
	estudiante_t* est2 = crear_estudiante(36, "Carolina Aramay");
	estudiante_t* est3 = crear_estudiante(10, "Facundo Sanso");

	hash_insertar(hash, "primero", est0);
	hash_insertar(hash, "segundo", est1);

	char tercera_clave[] = "tercero";
	hash_insertar(hash, tercera_clave, est2);
	tercera_clave[0] = 'T';
	tercera_clave[1] = 'E';
	tercera_clave[2] = 'R';
	tercera_clave[3] = 'C';
	tercera_clave[4] = 'E';
	tercera_clave[5] = 'R';
	tercera_clave[6] = '0';

	pa2m_afirmar(hash_insertar(hash, "cuarto",  est3) == 0, "Al modificar una clave previamente insertada, puedo seguir insertando elementos al hash");
	pa2m_afirmar(hash_obtener(hash, "tercero") == est2, "Al modificar una clave previamente insertada, puedo seguir obteniendo el elemento en la clave anterior");

	hash_destruir(hash);
}

					/* Pruebas Auxiliares Lista: Insertar */

void dadaUnaLista_alInsertarVariosElementos_seInsertanCorrectamente() {
	casilla_t* lista = casilla_crear();

	size_t cantidad_elementos = 0;

	pa2m_afirmar(casilla_insertar(NULL, NULL, NULL, NULL, NULL) == NULL, "Al insertar en lista NULL se retorna NULL");
	pa2m_afirmar(casilla_insertar(lista, NULL, NULL, NULL, &cantidad_elementos) == NULL, "Al insertar clave NULL en una lista se retorna NULL");
	pa2m_afirmar(casilla_insertar(lista, "primero", NULL, NULL, NULL) == NULL, "Al insertar en lista con una cantidad de elementos NULL se retorna NULL");

	lista = casilla_insertar(lista, "primero", NULL, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "segundo", NULL, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "tercero", NULL, NULL, &cantidad_elementos);

	pa2m_afirmar(cantidad_elementos == 3, "Al insertar 3 elementos en una lista la cantidad de elementos es 3");

	casilla_destruir(lista, NULL);
}

					 /* Pruebas Auxiliares Lista: Quitar */

void dadaUnaLista_alQuitarVariosElementos_seQuitanCorrectamente() {
	casilla_t* lista = casilla_crear();

	size_t cantidad_elementos = 0;

	pa2m_afirmar(casilla_quitar(NULL, NULL, NULL, NULL) == -1, "Al quitar en una lista NULL se devuelve ERROR (-1)");
	pa2m_afirmar(casilla_quitar(&lista, NULL, NULL, NULL) == -1, "Al quitar un elemento con clave NULL en una lista se devuelve ERROR (-1)");

	lista = casilla_insertar(lista, "primero", NULL, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "segundo", NULL, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "tercero", NULL, NULL, &cantidad_elementos);

	casilla_quitar(&lista, "primero", NULL, &cantidad_elementos);
	pa2m_afirmar(cantidad_elementos == 2, "Al quitar un elemento en una lista la cantidad de elementos disminuye");

	casilla_quitar(&lista, "primero", NULL, &cantidad_elementos);
	pa2m_afirmar(cantidad_elementos == 2, "Al quitar un elemento que no existe en una lista la cantidad de elementos no disminuye");

	casilla_quitar(&lista, "segundo", NULL, &cantidad_elementos);
	casilla_quitar(&lista, "tercero", NULL, &cantidad_elementos);

	pa2m_afirmar(cantidad_elementos == 0, "Al quitar todos los elementos en una lista la cantidad de elementos es 0");

	casilla_destruir(lista, NULL);
}

					/* Pruebas Auxiliares Lista: Obtener */

void dadaUnaLista_alObtenerUnElemento_seObtieneCorrectamente() {
	casilla_t* lista = casilla_crear();

	pa2m_afirmar(casilla_obtener(NULL, "no_existe") == false, "Al obtener un elemento en una lista NULL se retorna NULL");

	size_t cantidad_elementos = 0;

	lista = casilla_insertar(lista, "primero", NULL, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "segundo", NULL, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "tercero", NULL, NULL, &cantidad_elementos);

	pa2m_afirmar(casilla_obtener(lista, "no_existe") == false, "Al obtener un elemento con clave NULL se retorna NULL");

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	lista = casilla_insertar(lista, "primero", est0, destruir_estudiante, &cantidad_elementos);

	pa2m_afirmar(casilla_obtener(lista, "primero") == est0, "Al obtener un elemento por su clave se retorna el elemento correcto");
	pa2m_afirmar(casilla_obtener(lista, "no_existe") == NULL, "Al obtener un elemento que no existe se retorna NULL");

	casilla_destruir(lista, destruir_estudiante);
}

				   /* Pruebas Auxiliares Lista: Recorrido */

void dadaUnaLista_alRecorrerLosElementos_seRecorrenCorrectamente() {
	hash_t* hash = hash_crear(NULL, 3);
	casilla_t* lista = casilla_crear();

	size_t cantidad_elementos = 0;
	size_t cantidad_recorridos = 0;

	casilla_con_cada_clave(NULL, hash, NULL, NULL, &cantidad_recorridos);
	pa2m_afirmar(cantidad_recorridos == 0, "Al recorrer una lista NULL se retorna 0");

	estudiante_t* est0 = crear_estudiante(25, "Alejandro Schamun");
	estudiante_t* est1 = crear_estudiante(20, "Cami Fiorotto");
	estudiante_t* est2 = crear_estudiante(36, "Carolina Aramay");
	estudiante_t* est3 = crear_estudiante(10, "Facundo Sanso");
	estudiante_t* est4 = crear_estudiante(22, "Joaquin Dopazo");
	estudiante_t* est5 = crear_estudiante(30, "Julian Calderon");
	estudiante_t* est6 = crear_estudiante(40, "Julian Stiefkens");
	estudiante_t* est7 = crear_estudiante(5,  "Manuel Sanchez");
	estudiante_t* est8 = crear_estudiante(12, "Nicolas Celano");
	estudiante_t* est9 = crear_estudiante(28, "Nicolas Tonizzo");

	lista = casilla_insertar(lista, "primero", est0, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "segundo", est1, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "tercero", est2, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "cuarto",  est3, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "quinto",  est4, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "sexto",   est5, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "septimo", est6, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "octavo",  est7, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "noveno",  est8, NULL, &cantidad_elementos);
	lista = casilla_insertar(lista, "decimo",  est9, NULL, &cantidad_elementos);

	casilla_con_cada_clave(lista, hash, NULL, NULL, &cantidad_recorridos);
	pa2m_afirmar(cantidad_recorridos == 10, "Al recorrer una lista ininterrumpidamente se recorren todos los elementos");

	cantidad_recorridos = 0;
	casilla_con_cada_clave(lista, hash, reprobar_hasta_septimo_estudiante, NULL, &cantidad_recorridos);
	pa2m_afirmar(cantidad_recorridos == 7, "Al recorrer una lista hasta llegar a cierta clave e interrumpir el recorrido, se recorren la cantidad de elementos correctos");

	cantidad_recorridos = 0;
	casilla_con_cada_clave(lista, hash, NULL, NULL, &cantidad_recorridos);
	pa2m_afirmar(cantidad_recorridos == 10, "Al recorrer una lista con una funcion NULL se recorren todos los elementos");

	hash_destruir(hash);
	casilla_destruir(lista, destruir_estudiante);
}

int main() {
	pa2m_nuevo_grupo("Pruebas Hash: Creacion");
	dadoUnDestructorYUnValorInicialValido_alCrearUnHash_seRetornaUnHashCon0Elementos();
	dadoUnDestructorNULL_alDestruirUnHash_seDestruyeCorrectamenteSinDestruirLosElementos();
	
	pa2m_nuevo_grupo("Pruebas Hash: Insertar");
	dadoUnHashNULL_alInsertarUnElemento_seRetornaError();
	dadaUnaClaveNULL_alInsertarUnElemento_seRetornaError();
	dadoUnHash_alInsertarVariosElementos_seInsertanCorrectamente();
	dadoUnHash_alInsertarUnElementoConUnaClaveRepetida_seInsertaCorrectamente();
	dadoUnHash_alInsertarUnaClaveQueLuegoPuedeSerModificada_seHaceUnaCopiaDeLaClave();

	pa2m_nuevo_grupo("Pruebas Hash: Quitar");
	dadoUnHashNULL_alQuitarUnElemento_seRetornaError();
	dadaUnaClaveNULL_alQuitarUnElemento_seRetornaError();
	dadoUnHash_alQuitarVariosElementos_seQuitanCorrectamente();

	pa2m_nuevo_grupo("Pruebas Hash: Obtener");
	dadoUnHashNULL_alObtenerUnElemento_seRetornaNULL();
	dadaUnaClaveNULL_alObtenerUnElemento_seRetornaNULL();
	dadoUnHash_alObtenerUnElemento_seObtieneCorrectamente();

	pa2m_nuevo_grupo("Pruebas Hash: Contiene");
	dadoUnHashNULL_alVerificarSiContieneUnElemento_seRetornaNULL();
	dadaUnaClaveNULL_alVerificarSiElHashContieneUnElemento_seRetornaNULL();
	dadoUnHash_alVerificarSiContieneUnElemento_seObtieneCorrectamente();

	pa2m_nuevo_grupo("Pruebas Hash: Recorrido");
	dadoUnHashNULL_alRecorrerLosElementos_seRetorna0();
	dadoUnHash_alRecorrerLosElementos_seRetornaLaCantidadEsperada();

	pa2m_nuevo_grupo("Pruebas Auxiliares Lista: Insertar");
	dadaUnaLista_alInsertarVariosElementos_seInsertanCorrectamente();

	pa2m_nuevo_grupo("Pruebas Auxiliares Lista: Quitar");
	dadaUnaLista_alQuitarVariosElementos_seQuitanCorrectamente();

	pa2m_nuevo_grupo("Pruebas Auxiliares Lista: Obtener");
	dadaUnaLista_alObtenerUnElemento_seObtieneCorrectamente();

	pa2m_nuevo_grupo("Pruebas Auxiliares Lista: Recorrido");
	dadaUnaLista_alRecorrerLosElementos_seRecorrenCorrectamente();

    return pa2m_mostrar_reporte();
}
