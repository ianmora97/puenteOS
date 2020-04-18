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

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x)) //define la posicion del cursor
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*--VARIABLES DEL PROGRAMA--*/
int cantidadCarros;
struct Puente *puente; //puente
struct Cola *colaG; //cola para visualizar los carros que llegan
struct Cola *colaL; //carros del lado izquierdo
struct Cola *colaR; //carros del lado derecho
struct Cola *carrosTerminados; //carros que pasaron del lado izquierdo al derecho y al reves
struct Cola *ambulancias; //cuando aparezcan ambulancias guardarlas en un lugar
pthread_mutex_t contCarros; //cuenta los carros dentro del puente
pthread_mutex_t mutexColaL; //MUTEX para asegurar la cola Iquierda
pthread_mutex_t mutexColaR; //MUTEX para asegurar la cola Derecha
pthread_mutex_t semaforoTiempo; //MUTEX para el semaforo, tiempo en que cambia
int lucesDuracion[2]; //duracion de las luces de los semaforos [0]izquierda [1]derecha
bool trafico; //true = izquierda, false = derecha - como pasan los carros
int distCarNuevosL; //media de distribucion de carros del lado izquierdo
int distCarNuevosR; //media de distribucion de carrtos del lado derecho
int velocidades[4]; //0 velMaxIzq, 1 velMaxDer, 2 velMinIzq, 3 velMinDer
int cantL,cantR;
bool direccion; //para el semaforo

void crearPrograma(int ta, int lucesD, int lucesI, int dl, int dr, int vMi, int vMd, int vmi, int vmd);
void run(int opc); //1 el primero en llegar, 2 semaforos, 3 oficial
void *llegadaCarros(void *lado);
void tiempoEspera(bool l);
int definirVelocidades(bool lado);
int definirVelMax(bool lado);
int definirVelMin(bool lado);
void trafic();
void semaforo();
void printCuadro();
void *hiloEspera();
#endif