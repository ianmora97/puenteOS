#include "Carro.h"

struct Car* crearCar(int p, bool t, bool o, int v, bool m, struct Puente *pu) {
    struct Car *car;
    car = malloc(sizeof (struct Car*));
    car->puente = crearPuente(5);
    car->placa = p;
    car->tipo = t;
    car->orientacion = o;
    car->velocidad = v;
    car->movimiento = m;
    car->puente = pu;
    car->lugar = 0;
    return car;
}

void imprimirCarro(struct Car *ca) {
    struct Car *carro = ca;
    printf("[");
    printf("Placa: %i ,", carro->placa);
    printf("Direccion: ");
    if (carro->orientacion == true) {
        printf("Este - Oeste, ");
    } else {
        printf("Oeste - Este, ");
    }
    printf("Velocidad: %i ,", carro->velocidad);
    printf("En Movimiento?: ");
    if (carro->movimiento == true) {
        printf("en movimiento, ");
    } else {
        printf("detenido, ");
    }
    printf("Tipo: ");
    if (carro->tipo == false) {
        printf("Ambulancia");
    } else {
        printf("Carro");
    }

    printf("]");
    printf("\n");
}

void imprimirCarroSimp(struct Car *ca) {
    struct Car *carro = ca;
    printf("[%i ,", carro->placa);
    if (carro->orientacion == true) {
        printf("Izquierda");
    } else {
        printf("Derecha");
    }
    printf("]");
}

void* moverCarro(struct Car* c) {
    int pos = 0;
    int or; //direccion del carro
    int inicio;
    int velRel = 15000;
    if (c->orientacion && c->puente->direccion) {//izquierda
        inicio = 0; //inicia de izquierda a derecha
        or = 1;
        pos = inicio;

        pthread_mutex_lock(&(c->puente->posicion[pos]));
        c->puente->carretera[pos] = c->placa;
       
        imprimirPuente(c->puente, c);
        c->puente->carretera[pos] = 0;
        pos = pos + or;

        c->puente->cantCarros++;

        usleep((c->velocidad) * velRel);
        c->lugar = pos;
        c->movimiento = true;
        while ((pos >= 0) && (pos < c->puente->tamPuente)) { //mientras este dentro del puente   
            pthread_mutex_lock(&(c->puente->posicion[pos])); //voy al siguiente
            c->puente->carretera[pos] = c->placa;
            usleep(c->velocidad * velRel);

            pthread_mutex_unlock(&(c->puente->posicion[pos - or])); //dejo el anterior
            c->puente->carretera[pos - or] = 0;
            imprimirPuente(c->puente, c);
            pos = pos + or;
            //imprimirPuente(c->puente);
        }
        pthread_mutex_unlock(&(c->puente->posicion[pos - or])); //libera el ultimo espacio
        c->puente->carretera[pos - or] = 0;

        pthread_mutex_lock(&c->puente->contador);
        c->puente->cantCarros--;
        pthread_mutex_unlock(&c->puente->contador);
        usleep((c->velocidad) * velRel);
        imprimirPuente(c->puente, c);
        c->movimiento = false;
    } 
    if(!c->orientacion && !c->puente->direccion){//derecha
        inicio = c->puente->tamPuente - 1; //inicia de derecha a izquierda
        or = -1;
        pos = inicio;
        pthread_mutex_lock(&(c->puente->posicion[pos]));
        c->puente->carretera[pos] = c->placa;
        c->puente->cantCarros++;
        imprimirPuente(c->puente, c);
        c->puente->carretera[pos] = 0;
        pos = pos + or;

        usleep((c->velocidad) * velRel);
        c->lugar = pos;
        c->movimiento = true;
        while ((pos >= 0) && (pos < c->puente->tamPuente)) { //mientras este dentro del puente   
            pthread_mutex_lock(&(c->puente->posicion[pos])); //voy al siguiente
            c->puente->carretera[pos] = c->placa;
            usleep(c->velocidad * velRel);

            pthread_mutex_unlock(&(c->puente->posicion[pos - or])); //dejo el anterior
            c->puente->carretera[pos - or] = 0;
            imprimirPuente(c->puente, c);
            pos = pos + or;
            //imprimirPuente(c->puente);
        }
        pthread_mutex_unlock(&(c->puente->posicion[pos - or])); //libera el ultimo espacio
        c->puente->carretera[pos - or] = 0;

        pthread_mutex_lock(&c->puente->contador);
        c->puente->cantCarros--;
        pthread_mutex_unlock(&c->puente->contador);
        usleep((c->velocidad) * velRel);
        imprimirPuente(c->puente, c);
        c->movimiento = false;
    }


}

void imprimirPuente(struct Puente *puente, struct Car *c) {
    gotoxy(61, 7);
    for (int i = 0; i < puente->tamPuente; i++) {
        if (puente->carretera[i] != 0) {
            if (!c->tipo) {
                printf("[\x1b[31m%i\x1b[0m]", puente->carretera[i]);
            } else {
                printf("[\x1b[33m%i\x1b[0m]", puente->carretera[i]);
            }
        } else {
            printf("\x1b[34m[%i]\x1b[0m", puente->carretera[i]);
        }
        
    }
    printf("                            ");
    printf("\n");
}
