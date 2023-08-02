#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "hash_iterador.h"

#define CAPACIDAD 7
#define EXITO 0
#define ERROR -1

char* duplicar_string(const char* s){
  if(!s)
    return NULL;
  
  char* p = malloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}

void destruir_string(void* elemento){
	if(elemento)	free(elemento);
}

bool listar_productos(hash_t* mercado, const char* producto, void* aux){
	if(!producto)	return true;

	aux=aux;
	printf("%s -> %s\n", producto, (char*)hash_obtener(mercado, producto));
	
	return false;
}

void agregar_producto(hash_t* mercado, const char* producto, const char* cantidad, bool imprimir){
	int retorno = hash_insertar(mercado, producto, duplicar_string(cantidad));
	if(imprimir){
		printf("Agregando producto: %s ", producto);
		printf("\t\t%s\n", (retorno==EXITO)?"EXITO":"ERROR");
	}
}

void stock_producto_reponer(hash_t* mercado, const char* producto){
	printf("%s Cantidad en stock: ", producto);
	const char* cantidad = hash_obtener(mercado, producto);
	if(!cantidad) printf("Producto no almacenado\n");
	else if((strcmp(cantidad, "50") < 0)) printf("%s. Es necesario reponer stock\n", cantidad);
	else printf("%s. No hace falta reponer stock\n", cantidad);
}

void borrar_producto(hash_t* mercado, const char* producto){
	int retorno = hash_quitar(mercado, producto);
	printf("%s\n",(retorno==EXITO)?" - Se borro con exito":" - No se encontro el producto");
}

void actualizar_cantidad(hash_t* mercado, const char* producto, const char* nuevo_valor){
	const char* cantidad = hash_obtener(mercado, producto);
	if(!cantidad){
		printf("\nNo se encontro el producto: %s\n", producto);
		return;
	}
	printf("\nProducto: %s -- Valor inicial: %s", producto, cantidad);
	agregar_producto(mercado, producto, nuevo_valor, false);
	cantidad = hash_obtener(mercado, producto);
	printf(" -- Valor actual: %s ",cantidad);
}

void pruebas_standar(){
	hash_t* mercado = hash_crear(destruir_string, CAPACIDAD);

	agregar_producto(mercado, "Coca-Cola", "50", true);
	agregar_producto(mercado, "Sprite", "65", true);
	agregar_producto(mercado, "Fanta", "75", true);
	agregar_producto(mercado, "Pepsi", "30", true);
	agregar_producto(mercado, "Manaos", "10", true);
	agregar_producto(mercado, "Manaos Uva", "100", true);
	agregar_producto(mercado, "Quilmes", "125", true);
	agregar_producto(mercado, "Stella", "75", true);
	agregar_producto(mercado, "Patagonia", "35", true);
	agregar_producto(mercado, "Oreo", "130", true);
	agregar_producto(mercado, "Rumba", "50", true);
	agregar_producto(mercado, "Pepito", "65", true);
	agregar_producto(mercado, "Don Satur", "75", true);
	agregar_producto(mercado, "Chocolinas", "30", true);
	agregar_producto(mercado, "Dientitos", "120", true);
	agregar_producto(mercado, "Moritas", "100", true);
	agregar_producto(mercado, "Gomitas", "25", true);
	agregar_producto(mercado, "Queso", "75", true);
	agregar_producto(mercado, "Dulce", "35", true);
	agregar_producto(mercado, "Leche", "130", true);

	printf("\nProducto -> cantidad\n\n");
	hash_con_cada_clave(mercado, listar_productos, NULL);
	printf("\n");

	stock_producto_reponer(mercado, "Coca-Cola");
	stock_producto_reponer(mercado, "Manaos");
	stock_producto_reponer(mercado, "Dulce");
	stock_producto_reponer(mercado, "Gomitas");
	stock_producto_reponer(mercado, "Queso");

	size_t cantidad_productos = hash_cantidad(mercado);

	hash_iterador_t* it = hash_iterador_crear(mercado);

 	printf("\nBorrando todos los productos que tengan menos de 35 stock\n");
	
	while(hash_iterador_tiene_siguiente(it)){
    	const char* producto = hash_iterador_siguiente(it);
    	if(producto){
    		printf("Producto: %s", producto);
    		const char* cantidad = hash_obtener(mercado, producto);
    		if((strcmp(cantidad, "35") < 0)) borrar_producto(mercado, producto);
    		else printf(" - Stock suficiente\n");
    }
  }
  	hash_iterador_destruir(it);
	
  	printf("\nCantidad de productos inicialmente: %zu\nCantidad de productos restantes: %zu\n", cantidad_productos, hash_cantidad(mercado));
	hash_destruir(mercado);
}

void pruebas_NULL(){
	hash_t* mercado = hash_crear(NULL, CAPACIDAD);
	for(size_t i=0; i<200; i++){
		agregar_producto(mercado, NULL, NULL, false);
	}
	printf("\nIntento quitar un elemento: %s\n", (hash_quitar(mercado, NULL))==ERROR?"No se puede quitar NULL":"ERROR");
	printf("Intento obtener un elemento: %s\n", (hash_obtener(mercado, NULL)==NULL)?"No se puede obtener un elemento NULL":"ERROR");
	printf("Intento saber si contiene un elemento NULL: %s\n", (hash_contiene(mercado, NULL))==false?"No se puede contener clave NULL":"ERROR");
	printf("Cantidad de elmentos en el hash: %zu\n", hash_cantidad(mercado));
	hash_destruir(mercado);
}

void pruebas_insercion_y_borrado(){
	hash_t* mercado = hash_crear(destruir_string, CAPACIDAD);

	printf("\nInserto 20 elementos y luego actulizo los valores\n");
	agregar_producto(mercado, "Coca-Cola", "50", false);
	agregar_producto(mercado, "Sprite", "65", false);
	agregar_producto(mercado, "Fanta", "75", false);
	agregar_producto(mercado, "Pepsi", "30", false);
	agregar_producto(mercado, "Manaos", "10", false);
	agregar_producto(mercado, "Manaos Uva", "100", false);
	agregar_producto(mercado, "Quilmes", "125", false);
	agregar_producto(mercado, "Stella", "75", false);
	agregar_producto(mercado, "Patagonia", "35", false);
	agregar_producto(mercado, "Oreo", "130", false);
	agregar_producto(mercado, "Rumba", "50", false);
	agregar_producto(mercado, "Pepito", "65", false);
	agregar_producto(mercado, "Don Satur", "75", false);
	agregar_producto(mercado, "Chocolinas", "30", false);
	agregar_producto(mercado, "Dientitos", "120", false);
	agregar_producto(mercado, "Moritas", "100", false);
	agregar_producto(mercado, "Gomitas", "25", false);
	agregar_producto(mercado, "Queso", "75", false);
	agregar_producto(mercado, "Dulce", "35", false);
	agregar_producto(mercado, "Leche", "130", false);

	actualizar_cantidad(mercado, "Coca-Cola", "80");
	actualizar_cantidad(mercado, "Sprite", "85");
	actualizar_cantidad(mercado, "Fanta", "30");
	actualizar_cantidad(mercado, "Manaos", "200");
	actualizar_cantidad(mercado, "Pepito", "0");
	actualizar_cantidad(mercado, "Libros", "20");

	printf("\nIntento borrar producto y luego buscarlo:\n");
	hash_quitar(mercado, "Dulce");
	printf("%s -- Producto: Dulce\n", ((hash_obtener(mercado, "Dulce"))==NULL?"Se borro con exito y no se puede encontraro":"Hubo un error"));
	hash_quitar(mercado, "Leche");
	printf("%s -- Producto: Leche\n", ((hash_obtener(mercado, "Leche"))==NULL?"Se borro con exito y no se puede encontraro":"Hubo un error"));
	
	printf("\nBorro todos los elementos quitandolos de la lista: \nCantidad de productos actuales: %zu\n", hash_cantidad(mercado));
	hash_quitar(mercado,"Queso");
	hash_quitar(mercado,"Gomitas");
	hash_quitar(mercado,"Moritas");
	hash_quitar(mercado,"Dientitos");
	hash_quitar(mercado,"Chocolinas");
	hash_quitar(mercado,"Don Satur");
	hash_quitar(mercado,"Pepito");
	hash_quitar(mercado,"Rumba");
	hash_quitar(mercado,"Oreo");
	hash_quitar(mercado,"Patagonia");
	hash_quitar(mercado,"Stella");
	hash_quitar(mercado,"Quilmes");
	hash_quitar(mercado,"Manaos Uva");
	hash_quitar(mercado,"Manaos");
	hash_quitar(mercado,"Pepsi");
	hash_quitar(mercado,"Fanta");
	hash_quitar(mercado,"Sprite");
	hash_quitar(mercado,"Coca-Cola");
	printf("Cantidad de elementos final: %zu\n", hash_cantidad(mercado));


	hash_destruir(mercado);
}

void pruebas_iteradores(){

	hash_iterador_t* it1 = hash_iterador_crear(NULL);
	printf("\nIntento crear un iterador sin un hash: %s\n", (it1!=NULL)?"Se creo, lo cual no deberia":"No se creo");
	hash_iterador_destruir(it1);

	hash_t* mercado = hash_crear(destruir_string, CAPACIDAD);
	
	hash_iterador_t* it2 = hash_iterador_crear(mercado);
	printf("Intento crear un iterador con un hash vacio: %s\n", (it2!=NULL)?"Se creo con exito":"ERROR");
	printf("Intento iterar con un hash vacio: ");
	size_t listados = 0;
	while(hash_iterador_tiene_siguiente(it2)){
    	const char* producto = hash_iterador_siguiente(it2);
    	if(producto){
    		printf("Elemento: %s", producto);
    		listados++;
    	}
    }
    printf("cantidad de elementos listados: %zu", listados);

	hash_iterador_destruir(it2);


	agregar_producto(mercado, "a", "1", false);
	agregar_producto(mercado, "b", "2", false);
	agregar_producto(mercado, "c", "3", false);
	agregar_producto(mercado, "d", "4", false);
	agregar_producto(mercado, "e", "5", false);


	hash_iterador_t* it3 = hash_iterador_crear(mercado);
	printf("\nIntento crear un iterador con un hash promedio: %s\n", (it3!=NULL)?"Se creo con exito":"ERROR");
	
	printf("Intento iterar con un hash promedio: ");
	listados = 0;
	while(hash_iterador_tiene_siguiente(it3)){
    	const char* producto = hash_iterador_siguiente(it3);
    	if(producto){
    		listados++;
    	}
    }
    printf("Se itero la cantidad correcta? %s\n", (hash_cantidad(mercado)==listados?"Si":"No"));
	hash_iterador_destruir(it3);
	hash_destruir(mercado);

	printf("\n---Iterarador interno---\n");

	mercado = hash_crear(destruir_string, CAPACIDAD);

	agregar_producto(mercado, "Coca-Cola", "50", true);
	agregar_producto(mercado, "Sprite", "65", true);
	agregar_producto(mercado, "Fanta", "75", true);
	agregar_producto(mercado, "Pepsi", "30", true);
	agregar_producto(mercado, "Manaos", "10", true);
	agregar_producto(mercado, "Manaos Uva", "100", true);
	
	printf("\n");
	hash_con_cada_clave(mercado, listar_productos, NULL);
	printf("\n");
	
	hash_destruir(mercado);
}

int main(){
	printf("---Pruebas Hash standar---\n");
	pruebas_standar();

	printf("\n---Pruebas Hash NULL---\n");
	pruebas_NULL();

	printf("\n---Pruebas Hash insercion y borrado---\n");
	pruebas_insercion_y_borrado();

	printf("\n---Pruebas Hash iteradores---\n");
	pruebas_iteradores();

	return 0;
}