Marco Masciulli -- dni:39804927 -- padron:105145 -- Algo2Mendez

linea 58 -- Hash_crear: Recibe un destructor, y la capacidad. Devuelve un putero al Hash creado. Dentro de la funcion, 
evaluo que el destructor no sea NULL, y que la capacidad sea mayor a la minima. En caso de ser menor a la minima, se 
la asigna la minima posible. Luego se inicializan las listas para el hash, segun la capacidad del mismo.

Linea 140 -- Hash_insertar: Recibe un Hash, una clave y un elemento. Lo primero que se hace, es determinar la posicion
que va a ocupar dentro del hash. Esto se hace atravez de la funcion "valor". Una vez obtenido el valor que tendra dentro 
del hash, se busca a ver si dicha clave que se quiere insertar ya se encuentra en el hash. En caso de encontrarse, se modi-
fica el elemento que contienen. En caso de no existir, se inserta la nueva clave con su respectivo elemento. 

linea 169 -- Hash_quitar: Recibe un hash y una clave. Se vuelve a utilizar la funcion valor, que devuelve el valor que tiene
dentro del hash. Se busca en la lista correspondiente a dicho valor el elemento, atravez del comparador asociado a la lista.
la cual compara que el elemento en la lista, tenga la misma clave que la buscada. En caso de ser encontrado, se quita el elemento.

linea 191 -- Hash_obtener: Utiliza la funcion valor. Busca en dicha lista con la funcion "lista_buscar_elemento". Devulve el elemento 
si lo encontro, NULL en caso contrario.

linea 205 -- Hash_contiene: Utiliza la funcion valor. Primero verifica que la lista asociada a el valor del hash correspondiste 
no este vacia. Si esta vacia, devuelve false. En caso contrario, busca con el mismo comparador que en quitar. En caso 
de encontrarlo, devuelve true.

linea 226 -- Hash_cantidad: Va recorriendo todas las listas que no esten vacias en el hash. Devuelve la cantidad de elementos.

linea 238 -- Hash_destruir: Destruye todos los elementos del hash. Para destruir cada lista y cada elemento llama a la funcion
lista_con_cada_elemento.

linea 249 -- Hash_con_cada_clave: Utiliza el iterador del hash para iterar cada elemento y llamar a al funcion pasada por parametros.

hash_iterador --> Se crea el iterador, asociando el hash. Luego en la funcion hash_iterador_siguiente va recorriendo la distintas listas
del hash hasta encontrar un elemento. Cuando lo encuentra, se suma 1 al indice y devuelve el elemento. hash_iterador_tiene_siguiente: Com-
para el indice del iterador, con el tamaño del hash.

Tipos de Hash: Hash abierto y cerrado. la diferencia entre estos ronda en como se resuelven las colisiones, Mientras que en el abierto
el factor de carga puede ser mayor a 1, debido a que el mismo contiene lista para los elementos, entonces en una misma posicion puede
haber mas de un elemento, esto hace que no haya problema si dos elementos colisionan, porque pueden ocupar el mismo espacio. En el
hash cerrado, al tener la posibilidad de un unico elemento, las colisiones se resuelven pasando el elemento
a la siguiente posicion o segun la postura que se quiera tomar, por lo general el factor de carga de un hash cerrado de 0.7.

Rehashear: Rehashear significa aumentar la capacidad del mismo para que puedan seguir entrando elementos. Me parece mucho mas importante el rehasheo y 
tener mucho cuidad con el factor de carga en el hash cerrado, dado que es muy facil quedarse sin espacio. Por otro lado, en el hash abierto, se podria 
considerar que, se podria trabajar sin rehashear, aunque estoy entorpece el funcionamiento del mismo.
