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
    fscanf(fichero, "%d %d %d %d %d %d %d %d %d %d %d" , &tamPuente, &lucesTD, &lucesTI, &generarI, &generarD, &velMaxI, &velMaxD, &velMinI, &velMinD, &genAmbI, &genAmbD);
    printf(" Tamanio del puente:%i\n Duracion de luces Semaforo: Der: %i Izq: %i\n Generacion de carros: Der: %i Izq: %i\n Velocidad Max: Izq: %i Der: %i\n Velocidad Min: Izq: %i Der: %i\n Generacion Ambulancias: Izq: %i Der: %i\n",tamPuente,lucesTD,lucesTI,generarI,generarD,velMaxI,velMaxD,velMinI,velMinD,genAmbI,genAmbD);
    
    crearPrograma(tamPuente, lucesTD, lucesTI, generarI, generarD, velMaxI, velMaxD, velMinI, velMinD,genAmbI,genAmbD);
    int opcion;
    printf("Digite una opcion de simulacion[1=El primero que llega pasa, 2=Semaforo, 3=Transito]> ");
    scanf("%d",&opcion);
    fclose(fichero);
    system("clear");
    run(opcion);
    

    return 0;
}
