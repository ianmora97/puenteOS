#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include "Carro.h"
#include "Cola.h"
#include "Programa.h"

/*
WINDOW *win;
void InitVideo(){
       win = initscr(); // Crea la ventana 
       clear();  // Borra la pantalla entera bajo ncurses 
       refresh(); // Actualiza la ventana con los cambios 
       noecho();
       cbreak();
       keypad(win, TRUE);
}
void Exit(){
       refresh();
       endwin();
       exit(1);
}
*/
int main(){
    //InitVideo();

    /*struct Cola *cola = crearCola(5);
    struct Puente *p = crearPuente(5);

    struct Car *car = crearCar(1, false, true, 25, true,p);
    enqueue(cola, car);
    struct Car *car2 = crearCar(2,true, false, 45, true,p);
    enqueue(cola, car2);
    struct Car *car3 = crearCar(3,false, true, 35, false,p);
    enqueue(cola, car3);
    imprimirCola(cola);
    dequeue(cola);
    imprimirCola(cola);*/
    
    crearPrograma(6,10,10,500,600,10,10,20,20);
    run(2);
    

    return 0;
}
