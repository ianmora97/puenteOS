#include "Programa.h"

void crearPrograma(int ta, int lucesD, int lucesI, int dl, int dr, int vMi, int vMd, int vmi, int vmd, int pal, int par, int kl, int kr) {
    cantidadCarros = 1;
    puente = crearPuente(ta);
    colaG = crearCola(50);
    colaL = crearCola(10);
    colaR = crearCola(10);
    carrosTerminados = crearCola(20);
    ambulancias = crearCola(10);
    pthread_mutex_init(&contCarros, NULL);
    pthread_mutex_init(&mutexColaL, NULL);
    pthread_mutex_init(&mutexColaR, NULL);
    pthread_mutex_init(&semaforoTiempo, NULL);
    lucesDuracion[0] = lucesI;
    lucesDuracion[1] = lucesD;
    trafico = true;
    distCarNuevosL = dl;
    distCarNuevosR = dr;
    velocidades[0] = vMi;
    velocidades[1] = vMd;
    velocidades[2] = vmi;
    velocidades[3] = vmd;
    cantL = 0;
    cantR = 0;
    direccion = true;
    promedioAmbL = pal;
    promedioAmbR = par;
    pl = true;
    pr = false;
    contTrafico = 1;
    k1 = kl;
    k2 = kr;
}

void run(int opc) {
    srand(time(NULL));
    pthread_t izquierda;
    pthread_t derecha;
    pthread_t mostrarCola;
    int i = 1, d = 0;
    pthread_create(&izquierda, NULL, llegadaCarros, &i);
    pthread_create(&derecha, NULL, llegadaCarros, &d);
    pthread_create(&mostrarCola, NULL, imprimirColaPrint, carrosTerminados);

    if (opc == 1) { //EL primero que llega
        pthread_t first;
        pthread_create(&first, NULL, (void*) primeroQueLlega, NULL);
        printCuadro(1);
        //primeroQueLlega();
    } else if (opc == 2) { //Semaforo
        pthread_t sema;
        pthread_create(&sema, NULL, (void*) semaforo, NULL);
        printCuadro(2);
        //semaforo();

    } else { //Oficial transito
        pthread_t tra;
        pthread_create(&tra, NULL, (void*) trafic, NULL);
        printCuadro(3);
        //trafico();
    }
    pthread_join(izquierda, NULL);
    pthread_join(derecha, NULL);
    pthread_join(mostrarCola, NULL);
    while (1);

}

void primeroQueLlega() {
    while (1) {
        struct Nodo *actual;
        while (colaG->cantidad > 0) { //si hay carros
            actual = colaG->head;
            struct Car *temp;
            direccion = actual->carro->orientacion;
            if (direccion) {
                puente->direccion = true;
                gotoxy(65, 5);
                printf("->");
                pthread_mutex_lock(&mutexColaL);
                temp = dequeue(colaL);
                //cantidadCarros--;
                moverCarro(temp);
                pthread_mutex_unlock(&mutexColaL);

            } else {
                puente->direccion = false;
                gotoxy(65, 5);
                printf("<-");
                pthread_mutex_lock(&mutexColaR);
                temp = dequeue(colaR);
                //cantidadCarros--;
                moverCarro(temp);
                pthread_mutex_unlock(&mutexColaR);
            }
            if (colaG->cantidad > 0) {
                actual = actual->sig;
                dequeue(colaG);
            }
        }
    }
}

void semaforo() {
    pthread_t newcar;
    pthread_t tiempo;
    pthread_create(&tiempo, NULL, hiloEspera, NULL);
    while (1) {
        if (direccion) {
            puente->direccion = true;
            if (colaL->cantidad > 0) {
                struct Car *temp;
                while (direccion && pl) {
                    if (!vacia(colaL)) {
                        pthread_mutex_lock(&mutexColaL);
                        temp = dequeue(colaL);
                        pthread_create(&newcar, NULL, moverCarro, temp);
                        if (puente->cantCarros == 0) {
                            enqueue(carrosTerminados, temp);
                        }
                        pthread_mutex_unlock(&mutexColaL);
                    }
                }
            }
            puente->direccion = false;
        } else {
            puente->direccion = false;
            if (colaR->cantidad > 0) {
                struct Car *temp;
                while (!direccion && pr) {
                    if (!vacia(colaR)) {
                        pthread_mutex_lock(&mutexColaR);
                        temp = dequeue(colaR);
                        pthread_create(&newcar, NULL, moverCarro, temp);
                        if (puente->cantCarros == 0) {
                            enqueue(carrosTerminados, temp);
                        }
                        pthread_mutex_unlock(&mutexColaR);
                    }
                }
            }
            puente->direccion = true;
        }
    }
}

void trafic() {
    trafico = true; //deja pasar los carros de la izquierda primero si los hay
    int carrosPas;
    while (1) {
        while (!(puente->cantCarros > 0)) {
            if (trafico) { //si la bandera izquierda

                puente->direccion = true;
                if (colaL->cantidad > 0) {
                    gotoxy(65, 5);
                    printf("->");
                    struct Car *temp;
                    carrosPas = k1;
                    for (int i = 0; i < carrosPas && !vacia(colaL); i++) {
                        pthread_mutex_lock(&mutexColaL);
                        temp = dequeue(colaL);
                        moverCarro(temp);
                        if (puente->cantCarros == 0) {
                            enqueue(carrosTerminados, temp);
                        }
                        pthread_mutex_unlock(&mutexColaL);
                    }
                }
                puente->direccion = false;
                trafico = false;
            } else {
                puente->direccion = false;
                if (colaR->cantidad > 0) {
                    gotoxy(65, 5);
                    printf("<-");;
                    struct Car *temp;
                    carrosPas = k2;
                    for (int i = 0; i < carrosPas && !vacia(colaR); i++) {
                        pthread_mutex_lock(&mutexColaR);
                        temp = dequeue(colaR);
                        moverCarro(temp);
                        if (puente->cantCarros == 0) {
                            enqueue(carrosTerminados, temp);
                        }
                        pthread_mutex_unlock(&mutexColaR);
                    }
                }
                puente->direccion = true;
                trafico = true;
            }
        }
    }
}

void *oficialM() {
    while (1) {
        if (puente->cantCarros == 0) {
            if (trafico) {
                gotoxy(65, 5);
                printf("->");
                if (!vacia(colaL)) {
                    if (contTrafico == k1) {
                        trafico = !trafico;
                        pthread_mutex_lock(&contCarros);
                        contTrafico = 1;
                        pthread_mutex_unlock(&contCarros);
                        pl = false;
                        pr = true;
                    }
                } else {
                    trafico = !trafico;
                    pthread_mutex_lock(&contCarros);
                    contTrafico = 1;
                    pthread_mutex_unlock(&contCarros);
                    pl = false;
                    pr = true;
                }

            } else {
                gotoxy(65, 5);
                printf("<-");
                if (!vacia(colaR)) {
                    if (contTrafico == k2) {
                        trafico = !trafico;
                        pthread_mutex_lock(&contCarros);
                        contTrafico = 1;
                        pthread_mutex_unlock(&contCarros);
                        pl = true;
                        pr = false;
                    }
                } else {
                    trafico = !trafico;
                    pthread_mutex_lock(&contCarros);
                    contTrafico = 1;
                    pthread_mutex_unlock(&contCarros);
                    pl = true;
                    pr = false;
                }
            }

        }

    }
}

void *hiloEspera() { //para el semaforo
    int tiempo = 1;
    if (direccion) {
        tiempo = lucesDuracion[0];
    } else {
        tiempo = lucesDuracion[1];

    }
    tiempo = tiempo * 1000;
    while (1) {
        if (puente->cantCarros == 0) {
            if (direccion) { //para dibujar el semaforo
                gotoxy(65, 5);
                printf("->");
                gotoxy(60, 4);
                printf("\e[40m \x1b[0m");
                gotoxy(60, 5);
                printf("\e[42mO\x1b[0m");

                gotoxy(80, 4);
                printf("\e[41mO\x1b[0m");
                gotoxy(80, 5);
                printf("\e[40m \x1b[0m");
                pl = true;
                pr = false;
            } else {
                gotoxy(65, 5);
                printf("<-");
                gotoxy(60, 4);
                printf("\e[41mO\x1b[0m");
                gotoxy(60, 5);
                printf("\e[40m \x1b[0m");

                gotoxy(80, 4);
                printf("\e[40m \x1b[0m");
                gotoxy(80, 5);
                printf("\e[42mO\x1b[0m");
                pl = false;
                pr = true;
            }
            for (int i = 1; i <= tiempo; i++) {
                pthread_mutex_lock(&semaforoTiempo);
                gotoxy(65, 4);
                printf("%i ", (tiempo/1000) - (i / 1000));
                usleep((tiempo / 10));
                pthread_mutex_unlock(&semaforoTiempo);
            }
            direccion = !direccion;
        }
    }
}

void *llegadaCarros(void *lado) {
    bool ladoCola = *(bool*) lado;
    int velMax = definirVelMax(ladoCola);
    int velMin = definirVelMin(ladoCola);
    int cg=1;
    pthread_mutex_t mutex;
    struct Cola *cola;
    int promedioAmb;
    if (ladoCola) {
        mutex = mutexColaL;
        cola = colaL;
        promedioAmb = promedioAmbL;
    } else {
        mutex = mutexColaR;
        cola = colaR;
        promedioAmb = promedioAmbR;
    }
    while (1) {
        tiempoEspera(ladoCola); //distribucion del tiempo
        struct Car *car = crearCar((cantidadCarros), rand() % promedioAmb/*solo 1 caso funciona, cuando es 0*/, ladoCola, (velMin + rand() % (velMax - velMin)), false, puente);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        if (car->orientacion == true) {
            gotoxy(3, 14 + ++cantL);
            printf("(%i, %s, %s, %i)", car->placa, car->orientacion ? "Izq" : "Der", car->tipo ? "\x1b[33mCar\x1b[0m" : "\x1b[31mAmb\x1b[0m", car->velocidad);
            gotoxy(22, 14 + cantL);
            printf("\x1b[32m%02d:%02d\x1b[0m", tm.tm_min, tm.tm_sec);
        }

        if (car->orientacion == false) {
            gotoxy(33, 14 + ++cantR);
            printf("(%i, %s, %s, %i)", car->placa, car->orientacion ? "Izq" : "Der", car->tipo ? "\x1b[33mCar\x1b[0m" : "\x1b[31mAmb\x1b[0m", car->velocidad);
            gotoxy(52, 14 + cantR);
            printf("\x1b[32m%02d:%02d\x1b[0m", tm.tm_min, tm.tm_sec);
        }
        gotoxy(63, 14 + cantR+cantL);
        printf("(%i, %s, %s, %i)", car->placa, car->orientacion ? "Izq" : "Der", car->tipo ? "\x1b[33mCar\x1b[0m" : "\x1b[31mAmb\x1b[0m", car->velocidad);
        gotoxy(82, 14 + cg);
        printf("\x1b[32m%02d:%02d\x1b[0m", tm.tm_min, tm.tm_sec);
        
        pthread_mutex_lock(&mutex);
        enqueue(cola, car);
        enqueue(colaG, car);
        cantidadCarros++;
        pthread_mutex_unlock(&mutex);
    }
}

void printCuadro(int o) {
    if (o == 1) {

    } else if (o == 2) {
        gotoxy(59, 3);
        printf("\e[43m+-+\x1b[0m");
        gotoxy(59, 4);
        printf("\e[43m| |\x1b[0m");
        gotoxy(59, 5);
        printf("\e[43m| |\x1b[0m");
        gotoxy(59, 6);
        printf("\e[43m+-+\x1b[0m");

        gotoxy(79, 3);
        printf("\e[43m+-+\x1b[0m");
        gotoxy(79, 4);
        printf("\e[43m| |\x1b[0m");
        gotoxy(79, 5);
        printf("\e[43m| |\x1b[0m");
        gotoxy(79, 6);
        printf("\e[43m+-+\x1b[0m");
    }
    //gotoxy(2,4); gotoxy(20,4);

    //colaL
    gotoxy(1, 10);
    printf("+---------------------------+");
    gotoxy(3, 11);
    printf("\x1b[36m Cola Izquierda \x1b[0m");

    for (int i = 0; i < 15; i++) {
        gotoxy(1, 11 + i);
        printf("|");
        gotoxy(29, 11 + i);
        printf("|");
    }
    gotoxy(1, 12);
    printf("+---------------------------+");
    gotoxy(3, 13);
    printf("pk lado tipo  vel \x1b[32m tiempo\x1b[0m");
    gotoxy(1, 14);
    printf("+---------------------------+");
    //ColaR
    gotoxy(31, 10);
    printf("+---------------------------+");
    gotoxy(33, 11);
    printf("\x1b[36m Cola Derecha \x1b[0m");
    for (int i = 0; i < 15; i++) {
        gotoxy(31, 11 + i);
        printf("|");
        gotoxy(59, 11 + i);
        printf("|");
    }
    gotoxy(31, 12);
    printf("+---------------------------+");
    gotoxy(33, 13);
    printf("pk lado tipo  vel \x1b[32m tiempo\x1b[0m");
    gotoxy(31, 14);
    printf("+---------------------------+");
    //colaG
    gotoxy(61, 10);
    printf("+---------------------------+");
    gotoxy(63, 11);
    printf("\x1b[36m Cola General \x1b[0m");

    for (int i = 0; i < 15; i++) {
        gotoxy(61, 11 + i);
        printf("|");
        gotoxy(89, 11 + i);
        printf("|");
    }
    gotoxy(61, 12);
    printf("+---------------------------+");
    gotoxy(63, 13);
    printf("pk lado tipo  vel \x1b[32m tiempo\x1b[0m");
    gotoxy(61, 14);
    printf("+---------------------------+");
    //colaTerminada
/*
    gotoxy(91, 10);
    printf("+---------------------------+");
    gotoxy(93, 11);
    printf("\x1b[36m Cola Terminados \x1b[0m");

    for (int i = 0; i < 15; i++) {
        gotoxy(91, 11 + i);
        printf("|");
        gotoxy(119, 11 + i);
        printf("|");
    }
    gotoxy(91, 12);
    printf("+---------------------------+");
    gotoxy(93, 13);
    printf("pk lado tipo  vel \x1b[32m tiempo\x1b[0m");
    gotoxy(91, 14);
    printf("+---------------------------+");
*/
}

void tiempoEspera(bool l) {
    int lado;
    if (l) {
        lado = distCarNuevosL;
    } else {
        lado = distCarNuevosR;
    }
    int tiempoEspera, randomNumber;
    randomNumber = rand() % 1000;
    tiempoEspera = -(lado) * (log(randomNumber / (double) 1000)); //media numero aleatorio generado
    tiempoEspera = tiempoEspera * 10000;
    usleep(tiempoEspera);
}

int definirVelMax(bool lado) {
    if (lado) {
        return velocidades[0];
    } else {
        return velocidades[1];
    }
}

int definirVelMin(bool lado) {
    if (lado) {
        return velocidades[2];
    } else {
        return velocidades[3];
    }
}