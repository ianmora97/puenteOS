#include "Programa.h"

void crearPrograma(int ta, int lucesD, int lucesI, int dl, int dr, int vMi, int vMd, int vmi, int vmd) {
    cantidadCarros = 0;
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
    direccion = false;
}

void run(int opc) {
    srand(time(NULL));
    pthread_t izquierda;
    pthread_t derecha;
    pthread_t mostrarCola;
    int i = 1, d = 0;
    pthread_create(&izquierda, NULL, llegadaCarros, &i);
    pthread_create(&derecha, NULL, llegadaCarros, &d);   
    pthread_create(&mostrarCola, NULL, imprimirColaPrint, colaG);
    printCuadro();
    if (opc == 1) { //EL primero que llega

    } else if (opc == 2) { //Semaforo
        pthread_t sema;
        pthread_create(&sema, NULL,(void*) semaforo, NULL);
        //semaforo();

    } else { //Oficial transito
        pthread_t tra;
        pthread_create(&tra, NULL,(void*) trafic, NULL);
        //trafico();
    }
    while (1);

}
void semaforo(){    
    pthread_t tiempo;
    pthread_create(&tiempo, NULL, hiloEspera, NULL);
    while(1){
        if(direccion){
            if(colaL->cantidad > 0){
                gotoxy(0,1);
                printf("Izquierda->");
                struct Car *temp;
                while(direccion && !vacia(colaL)){
                    pthread_mutex_lock(&mutexColaL);
                    temp = dequeue(colaL);
                    moverCarro(temp);
                    if(puente->cantCarros == 0){
                        enqueue(carrosTerminados, temp);
                    }
                    pthread_mutex_unlock(&mutexColaL);
                }
            }
        }
        else{
            if(colaR->cantidad > 0){
                gotoxy(0,1);
                printf("<-Derecha  ");
                struct Car *temp;
                while(!direccion && !vacia(colaR)){
                    pthread_mutex_lock(&mutexColaR);
                    temp = dequeue(colaR);
                    moverCarro(temp);
                    if(puente->cantCarros == 0){
                        enqueue(carrosTerminados, temp);
                    }
                    pthread_mutex_unlock(&mutexColaR);
                }
            }
        }
    }
}
void trafic() {        
    
    trafico = true; //deja pasar los carros de la izquierda primero si los hay
    int carrosPas = 4;
    while (1) {
        while(!(puente->cantCarros > 0)){
            if (trafico) { //si la bandera izquierda               
                if (colaL->cantidad > 0) {
                    gotoxy(0,1);
                    printf("Izquierda->");
                    struct Car *temp;
                    for (int i = 0; i < carrosPas && !vacia(colaL); i++) {
                        pthread_mutex_lock(&mutexColaL);
                        temp = dequeue(colaL);
                        moverCarro(temp);
                        if(puente->cantCarros == 0){
                            enqueue(carrosTerminados, temp);
                        }
                        pthread_mutex_unlock(&mutexColaL);
                    }
                }
                trafico = false;
            } else {
                if (colaR->cantidad > 0) {
                    gotoxy(0,1);
                    printf("<-Derecha   ");
                    struct Car *temp;
                    for (int i = 0; i < carrosPas && !vacia(colaR); i++) {
                        pthread_mutex_lock(&mutexColaR);
                        temp = dequeue(colaR);
                        moverCarro(temp);
                        if(puente->cantCarros == 0){
                            enqueue(carrosTerminados, temp);
                        }
                        pthread_mutex_unlock(&mutexColaR);
                    }
                }
                trafico = true;
            }
        }
    }
}
void *hiloEspera(){
    int tiempo = 1;
    if(direccion){
        tiempo = lucesDuracion[0];
    }
    else{
        tiempo = lucesDuracion[1];

    }
    tiempo = tiempo * 1000;
    while(1){
        if(puente->cantCarros == 0){
            if(direccion){
                gotoxy(0,4);printf("\x1b[34mO\x1b[0m");
                gotoxy(0,5);printf("\x1b[32mO\x1b[0m");             
                
                gotoxy(18,4);printf("\x1b[31mO\x1b[0m");
                gotoxy(18,5);printf("\x1b[34mO\x1b[0m");
            }
            else{
                gotoxy(0,4);printf("\x1b[31mO\x1b[0m");
                gotoxy(0,5);printf("\x1b[34mO\x1b[0m");
                
                gotoxy(18,4);printf("\x1b[34mO\x1b[0m");
                gotoxy(18,5);printf("\x1b[32mO\x1b[0m");
            }
            for(int i=1;i<=tiempo;i++){
                pthread_mutex_lock(&semaforoTiempo);
                gotoxy(15,1);printf("Tiempo Semaforo: %i  ",(i/1000));	
                usleep((tiempo/10));
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
    pthread_mutex_t mutex;
    struct Cola *cola;
    if (ladoCola == true) {
        mutex = mutexColaL;
        cola = colaL;
    } else {
        mutex = mutexColaR;
        cola = colaR;
    }
    while (1) {
        
        tiempoEspera(ladoCola);

        struct Car *car = crearCar((++cantidadCarros), rand() % 10, ladoCola, (velMin + rand() % (velMax - velMin)), false, puente);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);      
        if(car->orientacion == true){    
            gotoxy(50, 5 + ++cantL); printf("(%i, %s, %s, %i)",car->placa,car->orientacion ? "Izq" : "Der",car->tipo ? "\x1b[33mCar\x1b[0m" : "\x1b[31mAmb\x1b[0m",car->velocidad);   
            gotoxy(69, 5 + cantL); printf("\x1b[32m%02d:%02d\x1b[0m",tm.tm_min,tm.tm_sec);
        }
        
        if(car->orientacion == false){    
            gotoxy(80, 5 + ++cantR); printf("(%i, %s, %s, %i)",car->placa,car->orientacion ? "Izq" : "Der",car->tipo ? "\x1b[33mCar\x1b[0m" : "\x1b[31mAmb\x1b[0m",car->velocidad);   
            gotoxy(99, 5 + cantR); printf("\x1b[32m%02d:%02d\x1b[0m",tm.tm_min,tm.tm_sec);
        }
        pthread_mutex_lock(&mutex);
        enqueue(cola, car);
        enqueue(colaG,car);
        pthread_mutex_unlock(&mutex);
        
        
        
    }
}
void printCuadro(){
    gotoxy(48,1);printf("+---------------------------+");
    gotoxy(50,2); printf("\x1b[36m Cola Izquierda \x1b[0m");
    for(int i=0;i<28;i++){
        gotoxy(48,2+i);printf("|");
        gotoxy(76,2+i);printf("|");
    }
    gotoxy(48,3);printf("+---------------------------+");
    gotoxy(50,4);printf("pk lado tipo  vel \x1b[32m tiempo\x1b[0m");
    gotoxy(48,5);printf("+---------------------------+");
    
    gotoxy(78,1);printf("+---------------------------+");
    gotoxy(80,2); printf("\x1b[36m Cola Derecha \x1b[0m");
    for(int i=0;i<28;i++){
        gotoxy(78,2+i);printf("|");
        gotoxy(106,2+i);printf("|");
    }
    gotoxy(78,3);printf("+---------------------------+");
    gotoxy(80,4);printf("pk lado tipo  vel \x1b[32m tiempo\x1b[0m");
    gotoxy(78,5);printf("+---------------------------+");
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
    tiempoEspera = -(lado) * (log(randomNumber / (double) 1000));
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