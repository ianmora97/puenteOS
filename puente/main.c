#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "Carro.h"
#include "Cola.h"
#include "Programa.h"

int main() {
    FILE* fichero;
    fichero = fopen("fichero", "r");
    if (fichero == NULL) {
        fputs("Archivo no encontrado!", stderr);
        exit(1);
    }
    int tamPuente, lucesTD,lucesTI,generarI, generarD;
    int velMaxI, velMaxD, velMinI,velMinD,genAmbI,genAmbD;
    int k1,k2;
    fscanf(fichero, "%d %d %d %d %d %d %d %d %d %d %d %d %d" , &tamPuente, &lucesTD, &lucesTI, &generarI, &generarD, &velMaxI, &velMaxD, &velMinI, &velMinD, &genAmbI, &genAmbD, &k1, &k2);
    printf("Tamanio del puente:\x1b[34m%i\x1b[0m\nDuracion de luces Semaforo: Der: \x1b[34m%i\x1b[0m Izq: \x1b[34m%i\x1b[0m\nGeneracion de carros: Der: \x1b[34m%i\x1b[0m Izq: \x1b[34m%i\x1b[0m\nVelocidad Max: Izq: \x1b[34m%i\x1b[0m Der: \x1b[34m%i\x1b[0m\nVelocidad Min: Izq: \x1b[34m%i\x1b[0m Der: \x1b[34m%i\x1b[0m\nGeneracion Ambulancias: Izq: \x1b[34m%i\x1b[0m Der: \x1b[34m%i\x1b[0m\nK1: \x1b[34m%i\x1b[0m K2: \x1b[34m%i\x1b[0m",tamPuente,lucesTD,lucesTI,generarI,generarD,velMaxI,velMaxD,velMinI,velMinD,genAmbI,genAmbD,k1,k2);
    
    crearPrograma(tamPuente, lucesTD, lucesTI, generarI, generarD, velMaxI, velMaxD, velMinI, velMinD,genAmbI,genAmbD,k1,k2);
    int opcion;
    printf("\n\nDigite una opcion de simulacion\n\t\x1b[33m[1]EL PRIMERO QUE LLEGA\n\t\x1b[36m[2]SEMAFORO\n\t\x1b[32m[3]TRANSITO\n\t\x1b[0m> ");
    scanf("%d",&opcion);
    fclose(fichero);
    system("clear");
    run(opcion);
    

    return 0;
}
