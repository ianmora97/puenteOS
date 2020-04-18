#ifndef PUENTE_H
#define PUENTE_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

struct Puente{
    int tamPuente;
    int cantCarros;
    int *carretera;
    pthread_mutex_t *posicion; //para evitar los deadlocks
    pthread_mutex_t contador;
    bool ambulancia;
};
struct Puente* crearPuente(int t);

#endif