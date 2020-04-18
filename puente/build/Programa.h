#ifndef PROGRAM_C
#define PROGRAM_C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "Carro.h"
#include "Cola.h"
#include "Puente.h"

/*--VARIABLES DEL PROGRAMA--*/
struct Puente *puente; //puente
struct Cola *colaL; //carros del lado izquierdo
struct Cola *colaR; //carros del lado derecho
struct Cola *carrosTerminados; //carros que pasaron del lado izquierdo al derecho y al reves
struct Cola *ambulancias; //cuando aparezcan ambulancias guardarlas en un lugar
pthread_mutex_t mutexColaL; //MUTEX para asegurar la cola Iquierda
pthread_mutex_t mutexColaR; //MUTEX para asegurar la cola Derecha
int lucesDuracion[2]; //duracion de las luces de los semaforos [0]izquierda [1]derecha
bool trafico; //true = izquierda, false = derecha - como pasan los carros
int distCarNuevosL; //media de distribucion de carros del lado izquierdo
int distCarNuevosR; //media de distribucion de carrtos del lado derecho
int velocidades[4]; //0 velMaxIzq, 1 velMaxDer, 2 velMinIzq, 3 velMinDer

void crearPrograma(int ta, int lucesD, int lucesI, int dl, int dr, int vMi, int vMd, int vmi, int vmd);
void run(int opc); //1 el primero en llegar, 2 semaforos, 3 oficial
void *llegadaCarros(void *lado);
void tiempoEspera(bool l);
int definirVelocidades(bool lado);
int definirVelMax(bool lado);
int definirVelMin(bool lado);

#endif