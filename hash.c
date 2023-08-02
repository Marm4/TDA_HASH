#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "lista.h"
#include "hash_iterador.h"

#define CAPACIDAD_MINIMA 3
#define ERROR -1
#define EXITO 0

typedef struct dato{
	void* elemento;
	const char* clave;
} dato_t;

struct hash{
	lista_t** lista_elementos;
	size_t capacidad;
	size_t cantidad_elementos;
	hash_destruir_dato_t destruir_elemento;
};

struct hash_iter{
	struct hash* hash;
	size_t indice;
	size_t indice_lista;
};

void no_destruye(void* elemento){
	return;
}

void borrar_dato(void* elemento, void* hash_datos){
	if(!elemento) return;

	hash_t* hash = hash_datos;
	dato_t* borrar = elemento;
	if(borrar->elemento)
		(*hash->destruir_elemento)(borrar->elemento);

	free(((char*)((dato_t*)(elemento))->clave));
	free(borrar);
}

int comparador(void* elemento1, void* elemento2){
	if(!elemento1 || !elemento2 || !(((char*)((dato_t*)(elemento1))->clave)))
		return ERROR;

	else if((strcmp(((char*)((dato_t*)(elemento1))->clave), ((char*)((dato_t*)(elemento2))->clave))) == 0)
		return EXITO;

	else
		return ERROR;
}

size_t valor(const char* clave, size_t capacidad){
	size_t largo = strlen(clave);
	size_t valor_hash = 0;
	int var;
	for(int i=0; i<largo; i++){
		var = clave[i];
		valor_hash += (size_t)((unsigned)var);
		valor_hash = (valor_hash*(size_t)((unsigned)var)) % capacidad;
	}
	return valor_hash;
}

static char* duplicar_string(const char* s){
  if(!s)
    return NULL;
  
  char* p = malloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){
	hash_t* nuevo_hash = calloc(1,sizeof(hash_t));
	if(!destruir_elemento)
		nuevo_hash->destruir_elemento = no_destruye;
	else
		nuevo_hash->destruir_elemento = destruir_elemento;
	if(!nuevo_hash)
		return NULL;

	if(capacidad < CAPACIDAD_MINIMA){
		nuevo_hash->lista_elementos = calloc(1,CAPACIDAD_MINIMA*sizeof(lista_t*));
		for(int i=0; i<CAPACIDAD_MINIMA; i++){
			nuevo_hash->lista_elementos[i] = lista_crear(comparador, no_destruye);
			nuevo_hash->capacidad = CAPACIDAD_MINIMA;
		}
	}
	else{
		nuevo_hash->lista_elementos = calloc(1,capacidad*sizeof(lista_t*));
		for(int i=0; i<capacidad; i++){
			nuevo_hash->lista_elementos [i] = lista_crear(comparador, no_destruye);
			}
		nuevo_hash->capacidad = capacidad;
		}
	if(!nuevo_hash->lista_elementos){
		free(nuevo_hash);
		return NULL;
	}

	return nuevo_hash;
}

int rehashear(hash_t* hash){
	size_t capacidad = hash->capacidad*2;
	size_t indice = 0;
	
	lista_t** nueva_lista_elementos = calloc(1,capacidad*sizeof(lista_t*));
	if(!nueva_lista_elementos)	return ERROR;

	for(int i=0; i<capacidad; i++){
		nueva_lista_elementos[i] = lista_crear(comparador, no_destruye);
	}
	
	for(size_t i=0; i<hash->capacidad; i++){
		while(!lista_vacia(hash->lista_elementos[i]) && indice < lista_elementos(hash->lista_elementos[i])){
			dato_t* dato = lista_elemento_en_posicion(hash->lista_elementos[i], indice);
			size_t valor_hash = valor(dato->clave, capacidad);
			lista_insertar(nueva_lista_elementos[valor_hash], dato);
			indice++;
		}
		indice = 0;
	}
	for(size_t i=0; i<hash->capacidad; i++){
		lista_destruir(hash->lista_elementos[i]);
	}
	free(hash->lista_elementos);
	hash->lista_elementos = nueva_lista_elementos;
	hash->capacidad = capacidad;
	return EXITO;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if(!hash || !clave) return ERROR;

	if(hash->cantidad_elementos/hash->capacidad > 2){
		if(rehashear(hash) == ERROR) return ERROR;
	}

	size_t valor_hash = valor(clave, hash->capacidad);

	dato_t* nuevo_dato = calloc(1,sizeof(dato_t));
	if(!nuevo_dato) return ERROR;
	
	nuevo_dato->clave = clave;
	dato_t* repetido = lista_buscar_elemento(hash->lista_elementos[valor_hash], nuevo_dato);
	
	if(repetido){
		(*hash->destruir_elemento)(repetido->elemento);
		repetido->elemento = elemento;
		free(nuevo_dato);
		return EXITO;
		}
	else{
		nuevo_dato->clave = duplicar_string(clave);
		hash->cantidad_elementos++;
		nuevo_dato->elemento = elemento;
		return lista_insertar(hash->lista_elementos[valor_hash], nuevo_dato);
	}
}

int hash_quitar(hash_t* hash, const char* clave){
	if(!hash || !clave) return ERROR;

	size_t valor_hash = valor(clave, hash->capacidad);
	dato_t* dato = calloc(1,sizeof(dato_t));
	if(!dato) return ERROR;
		
	dato->clave = clave;
	dato_t* borrar = lista_buscar_elemento(hash->lista_elementos[valor_hash], dato);
	if(!borrar){
		free(dato);
		return ERROR;	
	} 
	(*hash->destruir_elemento)(borrar->elemento);
	free((char*)borrar->clave);
	borrar->clave = NULL;
	borrar->elemento = NULL;
	free(dato);
	hash->cantidad_elementos--;
	return EXITO;
}

void* hash_obtener(hash_t* hash, const char* clave){
	if(!hash || !clave) return NULL;

	size_t valor_hash = valor(clave, hash->capacidad);
	dato_t* dato = calloc(1,sizeof(dato_t));
	if(!dato) return NULL;

	dato->clave = clave;
	dato_t* elemento = lista_buscar_elemento(hash->lista_elementos[valor_hash], dato);
	free(dato);
	if(!elemento) return NULL;
	return elemento->elemento;
}

bool hash_contiene(hash_t* hash, const char* clave){
	if(!hash || !clave) return false;
	
	size_t valor_hash = valor(clave, hash->capacidad);
	if(lista_vacia(hash->lista_elementos[valor_hash]))
		return false;
	
	else{
		dato_t* dato = calloc(1,sizeof(dato_t));
		dato->clave = clave;
		dato_t* contiene = lista_buscar_elemento(hash->lista_elementos[valor_hash], dato);
		free(dato);
		
		if(!contiene) return false;
		
		else if(contiene->elemento)	return true;
		
		else return false;
	}
}

size_t hash_cantidad(hash_t* hash){
	size_t cantidad_elementos = 0;
	if(!hash) return cantidad_elementos;

	for(size_t i=0; i<hash->capacidad; i++){
		if(!lista_vacia(hash->lista_elementos[i]))
			cantidad_elementos++;
	}

	return hash->cantidad_elementos;
}

void hash_destruir(hash_t* hash){
	if(!hash) return;

	for(int i=0; i<hash->capacidad; i++){
		lista_con_cada_elemento(hash->lista_elementos[i],borrar_dato, hash);
		lista_destruir(hash->lista_elementos[i]);
	}
	free(hash->lista_elementos);
	free(hash);
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if(!hash) return EXITO;

	size_t cantidad_recorrido = 0;
	bool variable = false;
	
	hash_iterador_t* iter = hash_iterador_crear(hash);

  	while(hash_iterador_tiene_siguiente(iter) && !variable){
    	const char* clave = hash_iterador_siguiente(iter);
    	if(clave){
     		cantidad_recorrido++;
      		variable = (*funcion)(hash, clave, aux);
  		}
	}
  
	hash_iterador_destruir(iter);
	return cantidad_recorrido;
}

hash_iterador_t* hash_iterador_crear(hash_t* hash){
	if(!hash)	return NULL;

	hash_iterador_t* it = calloc(1,sizeof(hash_iterador_t));
	if(!it) return NULL;

	it->hash = hash;
	return it;
}

const char* hash_iterador_siguiente(hash_iterador_t* iterador){
	if(!iterador || !hash_iterador_tiene_siguiente(iterador)) return NULL;
	
	dato_t* dato = NULL;
	if(!lista_vacia(iterador->hash->lista_elementos[iterador->indice]) && lista_elementos(iterador->hash->lista_elementos[iterador->indice]) > iterador->indice_lista){
		dato = lista_elemento_en_posicion(iterador->hash->lista_elementos[iterador->indice], iterador->indice_lista);
		iterador->indice_lista++;

	}
	if(lista_vacia(iterador->hash->lista_elementos[iterador->indice]) || !dato){
		iterador->indice_lista = 0;
		iterador->indice++;
	}
	if(dato)
		return dato->clave;
	else
		return NULL;
}

bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador || (hash_cantidad(iterador->hash)==0)) return false;
	else if(iterador->indice < iterador->hash->capacidad)
		return true;
	else return false;
}

void hash_iterador_destruir(hash_iterador_t* iterador){
	if(iterador)
		free(iterador);
}