#ifndef CARRO_H
#define CARRO_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "Puente.h"

struct Car{
    int placa; //para diferenciar
    bool tipo; //ambulancia = false, carro = true
    bool orientacion; //true = este a oeste, false = oeste a este
    int velocidad; //velocidad del carro
    bool movimiento; //true= en moviemiento, false= detenido
    struct Puente *puente;
    int lugar; //momento en el puente
};

struct Car* crearCar(int p, bool t, bool o, int v, bool m, struct Puente *pu);
void* moverCarro(struct Car* c);
void imprimirCarro(struct Car *ca);
void imprimirCarroSimp(struct Car *ca);
void imprimirPuente(struct Puente *puente, struct Car *c);
#endif