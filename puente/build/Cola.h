#ifndef COLA_H
#define COLA_H
#include "Carro.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Nodo{
    struct Car *carro;
    struct Nodo *sig;
};
struct Cola{
    struct Nodo *head;
    struct Nodo *tail;
    int tam;
};
struct Cola* crearCola(int c);
bool vacia(struct Cola *c);
void enqueue(struct Cola *c,struct Car *car);
struct Car* dequeue(struct Cola *c);
void borrarCola(struct Cola *c);
void imprimirCola(struct Cola *c);

#endif