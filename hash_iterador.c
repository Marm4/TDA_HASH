#include <stdio.h>
#include <stdlib.h>
#include "hash_iterador.h"
#include "lista.h"

struct hash_iter{
	struct hash* hash;
	size_t indice;
};

typedef struct dato{
	lista_t* elemento;
	const char* clave;
} dato_t;

hash_iterador_t* hash_iterador_crear(hash_t* hash){
	hash_iterador_t* it = calloc(1,sizeof(hash_iterador_t));
	if(!it) return NULL;

	it->hash = hash;
	return it;
}

const char* hash_iterador_siguiente(hash_iterador_t* iterador){
	if(!iterador || !hash_iterador_tiene_siguiente(iterador)) return NULL;
	dato_t* dato = iterador->hash->dato;
	const char* elemento;
	size_t var = iterador->indice;
	
	while(hash->((dato_t*)dato[var]).clave == 0 && var < hash->capacidad){
		var++;
	}
	if(var == hash->capacidad) return NULL;
	elemento = hash->dato[var].clave;
	iterador->indice++;
	return elemento;
}

bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador) return false;
	else if(iterador->indice < hash_cantidad(iterador->hash))
		return true;
	else return false;
}

void hash_iterador_destruir(hash_iterador_t* iterador){
	if(iterador)
		free(iterador);
}