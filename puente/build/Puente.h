#ifndef PUENTE_H
#define PUENTE_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Puente{
    int tamPuente;
    int cantCarros;
};
struct Puente* crearPuente(int t);

#endif