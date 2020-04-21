#include"Cola.h"

struct Cola* crearCola(int c){
    struct Cola* cola;
    cola = malloc(sizeof(struct Cola*));
    cola->head = NULL;
    cola->tail = NULL;
    cola->tam = c;
    cola->cantidad = 0;
}

bool vacia(struct Cola *c){
    if(c->head == NULL){
        return true;
    }
    return false;
}

void enqueue(struct Cola *c, struct Car *car){
    struct Nodo *temp = malloc(sizeof(struct Nodo));
    temp->carro = car;
    temp->sig=NULL;
    if (c->head == NULL){
        c->head = temp;
        c->tail = temp;
        c->cantidad++;
    }
    else{
        c->tail->sig = temp;
        c->tail = temp;
        c->cantidad++;
    }
}
struct Car* dequeue(struct Cola *c){
    if (!vacia(c)){
        struct Car *actual = c->head->carro;
        struct Nodo *borrar = malloc(sizeof(struct Nodo*));
        if (c->head->carro->placa == c->tail->carro->placa){
            c->head = NULL;
            c->tail = NULL;
            c->cantidad--;
        }
        else{
            borrar = c->head;
            c->head = c->head->sig;
            c->cantidad--;
        }
        free(borrar);
        return actual;
    }
    else{
        return NULL;
    }
}
void borrarCola(struct Cola *c){
    struct Nodo *actual = c->head;
    struct Nodo *eliminar;
    while(eliminar != NULL){
        eliminar = actual;
        actual = actual->sig;
        free(eliminar);
    }
}
void imprimirCola(struct Cola *c){
    struct Nodo *actual = c->head;
    struct Car *car;

    while (actual != NULL){
        printf("[");
        printf("Placa: %i ,", actual->carro->placa );
        printf("Direccion: ");
        if(actual->carro->orientacion == true){
            printf("Este - Oeste, ");
        }
        else{
            printf("Oeste - Este, ");
        }
        printf("Velocidad: %i ,",actual->carro->velocidad);
        printf("En Movimiento?: ");
        if(actual->carro->movimiento == true){
            printf("en movimiento, ");
        }
        else{
            printf("detenido, ");
        }
        printf("Tipo: ");
        if(actual->carro->tipo == false){
            printf("Ambulancia");
        }
        else{
            printf("Carro");
        }

        printf("]->\n");
        actual = actual->sig;
    }
    printf("\n");
}
void imprimirColaSimp(struct Cola *c){
    struct Nodo *actual = c->head;
    struct Car *car;
    gotoxy(100,c->cantidad+1);
    while (actual != NULL){
        printf("[");
        printf(" %i ,", actual->carro->placa );
        if(actual->carro->orientacion == true){
            printf("Izq, ");
        }
        else{
            printf("Der, ");
        }
        if(actual->carro->tipo == false){
            printf("Ambul");
        }
        else{
            printf("Carro");
        }
        printf("]\n");
        actual = actual->sig;
    }
}
void* imprimirColaPrint(void *cola){
    struct Cola *c = (struct Cola*) cola;
    struct Nodo *actual = c->head;
    struct Car *car; 
/*
    while (actual != NULL){
        gotoxy(,); printf("(%i, %s, %s, %i)", car->placa, car->orientacion ? "Izq" : "Der", car->tipo ? "\x1b[33mCar\x1b[0m" : "\x1b[31mAmb\x1b[0m", car->velocidad);
        actual = actual->sig;
    }
*/
    printf("\n");
}
struct Car* peek(struct Cola *c){
    return c->head->carro;
}