#include "Puente.h"

struct Puente* crearPuente(int t){
    struct Puente* puente = malloc(sizeof(struct Puente*));
    puente->tamPuente = t;
    puente->cantCarros = 0;
    puente->carretera = malloc(t * sizeof(int*)); //crea los espacios del puente
    for(int i=0;i<t;i++){
        puente->carretera[i] = 0;
    }
    pthread_mutex_init(&puente->posicion, NULL);
    pthread_mutex_init(&puente->contador, NULL);
    puente->posicion = malloc(t * sizeof(pthread_mutex_t));
	for(int i = 0; i < t; i++){
		pthread_mutex_init(&puente->posicion[i], NULL); 
	}
    puente->ambulancia = false;
    puente->direccion = false;
    return puente;
}
