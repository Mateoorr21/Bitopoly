#include "bitopoly.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUERTE_MIN 50
#define SUERTE_MAX 400
// Macro para detener el flujo de ejución después de un turno
#define ESPERA() \
    printf("Presione la tecla [ENTER] para CONTINUAR...\n"); \
    fflush(stdin); \
    getchar();
#define CLEAN() system("cls");
#define ENTER() printf("\n");
        

void inicializar_tablero(Casilla tablero[], int tam){
    int precio_inicial = 100;

    tablero[0].tipo = INICIO;
    tablero[10].tipo = CARCEL;
    tablero[8].tipo = SUERTE;
    tablero[16].tipo = IMPUESTO;

    for (int i = 1; i < tam; i++){ 
        if(i == 10 || i == 8 || i == 16) continue;

        tablero[i].tipo = PROPIEDAD;
        sprintf(tablero[i].nombre, "Casilla %d", i);
        tablero[i].precio = precio_inicial;
        tablero[i].renta = (int)(precio_inicial * 0.5);
        tablero[i].propietario = 0;

        precio_inicial += 20;
    }
}
void inicializar_jugadores(Jugador jugadores[], int num_jugadores){
    for (int i = 0; i < num_jugadores; i++){
        sprintf(jugadores[i].nombre, "Jugador %d", i+1);
        jugadores[i].dinero = 1000;
        jugadores[i].estado = SIGUE_JUGANDO;
        jugadores[i].posicion = 0;
        jugadores[i].enCarcel = 0;
        jugadores[i].propiedades = 0;
    }
}
int lanzar_dado(){
    return rand() % 6 + 1;
}
int mover_jugador(Jugador* j, int pasos){
    int vuelta = (j->posicion + pasos >= 20) ? 1 : 0;
    j->posicion = (j->posicion + pasos) % 20;
    return vuelta; 
}
void revisar_casilla(Casilla tablero[], Jugador* j, Jugador* otro, int turno){
    switch(tablero[j->posicion].tipo) {
        case INICIO:
            printf("ESTAS EN LA CASILLA DE INICIO\n");
            break;
        case PROPIEDAD:
            // Es su propiedad
            if ((j->propiedades >> j->posicion) & 1) {
                printf("CAISTE EN TU PROPIEDAD\n");
                break;
            } 

            // Es propiedad de otro jugador
            else if ((otro->propiedades >> j->posicion) & 1) {
                j->dinero -= tablero[j->posicion].renta;
                otro->dinero += tablero[j->posicion].renta;
                printf("CAISE EN UNA PROPIEDAD DE %s\n", otro->nombre);
                printf("TE ESTAN COBRANDO $%d\n", tablero[j->posicion].renta);
                printf("TE QUEDAN $%d\n", j->dinero);
            }
            
            // No tiene propietario
            else {
                unsigned char compra;
                printf("LA PROPIEDAD NO TIENE DUEÑO. INFORMACION...\n");
                printf("- Nombre de la casilla: %s\n", tablero[j->posicion].nombre);
                printf("- Precio: %d\n", tablero[j->posicion].precio);
                printf("- Renta : %d\n", tablero[j->posicion].renta);
                while(1) {
                    printf("¿Desea comprar la propiedad (Y/N)? : ");
                    scanf(" %c", &compra);
                    if (compra != 'Y' && compra != 'y' && compra != 'N' && compra != 'n') {
                        printf("ERROR. RESPUESTA INVALIDA. INTENTE DE NUEVO\n");
                    }
                    else break;
                } 
                ENTER();
                if (compra == 'Y' || compra == 'y') {
                    if(j->dinero < tablero[j->posicion].precio) {
                        printf("FONDOS INSUFICIENTES PARA LA COMPRA :(\n");
                    }
                    else {
                        j->dinero -= tablero[j->posicion].precio;
                        j->propiedades |= (1 << j->posicion);
                        tablero[j->posicion].propietario |= (turno) ? 02 : 04;
                        printf("COMPRA DE PROPIEDAD EXITOSA\n");
                        printf("TE QUEDAN $%d\n", j->dinero);
                    }
                }
                else if (compra == 'N' || compra == 'n') {
                    printf("COMPRA DE PROPIEDAD NO REALIZADA\n");
                }
            }
            unsigned char estadoPropiedad = tablero[j->posicion].propietario;
            break;
        case SUERTE:
            int monto_suerte = rand() % (SUERTE_MAX - SUERTE_MIN + 1) + SUERTE_MIN;
            printf("¡SUERTE! has obtenido $%d\n", monto_suerte);
            j->dinero += monto_suerte;
            printf("TIENES ACTUALMENTE $%d\n", j->dinero);
            break;
        case IMPUESTO:
            int impuesto = (int)(j->dinero * 0.4); 
            printf("¡IMPUESTO!, te vamos a cobrar $%d\n",impuesto);
            printf("TE QUEDAN $%d\n", j->dinero);
            break;
        case CARCEL:
            printf("HAS CAIDO EN LA CARCEL\n");
            unsigned char carcel;
            while(1) {
                printf("¿Deseas salir pagando $200 o esperas inmóvil 2 turnos (Y/N): ");
                scanf(" %c", &carcel); // el espacio evita leer '\n'
                if (carcel != 'Y' && carcel != 'y' && carcel != 'N' && carcel != 'n') {
                    printf("ERROR. RESPUESTA INVALIDA. INTENTE DE NUEVO\n");
                }
                else break;
            } 
            if (carcel == 'Y' || carcel == 'y') {
                    if(j->dinero < 200) {
                        printf("FONDOS INSUFICIENTES PARA LA FIANZA :(\n");
                    }
                    else {
                        j->dinero -= 200;
                        printf("FIANZA PAGADA EXITOSAMENTE\n");
                        printf("TE QUEDAN $%d\n", j->dinero);
                        break;
                    }
                }
            else if (carcel == 'N' || carcel == 'n') {
                printf("FIANZA RECHAZADA. ESPERA DOS TURNOS EN CARCEL\n");
            }
            j->enCarcel = 2; //cantidad de turnos que permanecera en la carcel           
            break;
    }
}

int verificar_bancarrota(Jugador j) {
    return (j.estado) ? 1 : 0;
}

int tirar_hasta_no_dobles() {
    int dado1 = lanzar_dado();
    int dado2 = lanzar_dado();
    return (dado1 != dado2) ? (dado1 + dado2) : tirar_hasta_no_dobles();
}

void ejecutar_juego(Casilla tablero[], Jugador jugadores[], int turno) {
    int dado = 0;
    while (jugadores[0].estado || jugadores[1].estado) {
        printf("\n\nTurno de %s", jugadores[turno].nombre);
        imprimir_tablero(tablero, jugadores, turno);
        if(jugadores[turno].enCarcel != 0) {
            jugadores[turno].enCarcel--;
            printf("\nUSTED ESTA EN LA CARCEL. PIERDE SU TURNO\n");
            printf("\nLE QUEDAN %d TURNO(S) EN LA CARCEL", jugadores[turno].enCarcel);
        }
        else {
            dado = tirar_hasta_no_dobles();
            printf("\nResultado de los dados: %d\n", dado);
            int vueltaCompleta = mover_jugador(&(jugadores[turno]), dado);
            if(vueltaCompleta) {
                printf("HAS COMPLETADO UNA VUELTA. RECIBES $25\n");
                jugadores[turno].dinero += 25;
            }
            printf("Caíste en la casilla %d\n", jugadores[turno].posicion);
            ENTER();

            revisar_casilla(tablero, &(jugadores[turno]), &(jugadores[(turno == 0) ? 1 : 0]), turno);
            jugadores[turno].estado = verificar_bancarrota(jugadores[turno]);
        }
        // si el jugador actual no está en bancarrota
        if(jugadores[turno].estado == 1)  turno = (turno == 0) ? 1 : 0; // operador ternario para cambiar turno
        ESPERA();
        CLEAN();
    }

    printf("\n%s HA QUEDADO EN BANCARROTA.\n",  jugadores[turno].nombre);
    printf("¡ EL %s ES EL GANADOR !\n", jugadores[turno].nombre);
    // Indicamos al usuario presionar ENTER para regresar al menú principal
    ESPERA();
    CLEAN();
}

void imprimir_tablero(Casilla tablero[], Jugador jugadores[], int turno){
    printf("-----------------------------------------");
    printf("\nJugador1: 🟥 ");
    printf("Jugador2: 🟦\n");
    for (int i = 0; i < 2; i++){
        printf("%s, Casilla actual: %d, Dinero actual: $%d. \n",jugadores[i].nombre, jugadores[i].posicion, jugadores[i].dinero);
    }
    ENTER();
    for (int i = 0; i <= 5; i++) {
        printf("%s", obtener_casilla_i(i, jugadores));
    }
    ENTER();

    // Laterales
    for (int i = 6; i <= 9; i++) {
        printf("%s              %s\n",
            obtener_casilla_i(25 - i, jugadores),
            obtener_casilla_i(i, jugadores));
    }

    // Abajo derecha -> izquierda
    for (int i = 15; i >= 10; i--) {
        printf("%s", obtener_casilla_i(i, jugadores));
    }
    ENTER();
}
char* obtener_casilla_i(int i, Jugador jugadores[]) {
    static char buffers[10][20];
    static int index = 0;

    index = (index + 1) % 10;

    if (jugadores[0].posicion == i)
        sprintf(buffers[index], "[🟥%d]", i);
    else if (jugadores[1].posicion == i)
        sprintf(buffers[index], "[🟦%d]", i);
    else
        sprintf(buffers[index], "[%d]", i);

    return buffers[index];
}

void mostrar_reglas() {
    printf("\n========== REGLAS GENERALES DE BITOPOLY ==========\n\n");

    printf("👤 JUGADORES:\n");
    printf("- Jugador 1: 🟥 (ROJO) → INICIA LA PARTIDA\n");
    printf("- Jugador 2: 🟦 (AZUL)\n\n");

    printf("OBJETIVO:\n");
    printf("- Ser el último jugador con dinero (evitar la bancarrota).\n\n");

    printf("🎲TURNOS:\n");
    printf("- Cada jugador lanza los dados en su turno.\n");
    printf("- Avanza la cantidad indicada.\n");
    printf("- Si pasa por INICIO, recibe $25.\n\n");

    printf("🏠 PROPIEDADES:\n");
    printf("- Puedes comprar propiedades sin dueño.\n");
    printf("- Si caes en propiedad de otro jugador, pagas renta.\n");
    printf("- Si es tuya, no pasa nada.\n\n");

    printf("🍀 SUERTE:\n");
    printf("- Recibes una cantidad aleatoria de dinero.\n\n");

    printf("💸 IMPUESTO:\n");
    printf("- Pierdes el 40%% de tu dinero actual.\n\n");

    printf("🚓 CÁRCEL:\n");
    printf("- Puedes pagar $200 para salir.\n");
    printf("- O esperar 2 turnos sin jugar.\n\n");

    printf("💀 BANCARROTA:\n");
    printf("- Si tu dinero llega a 0 o menos, pierdes.\n");
    printf("- El otro jugador gana automáticamente.\n\n");

    printf("==================================================\n\n");

    ESPERA();
    CLEAN();
}

void casilla_sin_propietario(Casilla tablero[], int turno, Jugador *j){
    unsigned char compra;
        printf("LA PROPIEDAD NO TIENE DUEÑO. INFORMACION...\n");
        printf("- Nombre de la casilla: %s\n", tablero[j->posicion].nombre);
        printf("- Precio: %d\n", tablero[j->posicion].precio);
        printf("- Renta : %d\n", tablero[j->posicion].renta);
        while(1) {
            printf("¿Desea comprar la propiedad (Y/N)? : ");
            scanf(" %c", &compra);
            if (compra != 'Y' && compra != 'y' && compra != 'N' && compra != 'n') {
                printf("ERROR. RESPUESTA INVALIDA. INTENTE DE NUEVO\n");
            }
            else break;
        } 
        ENTER();
        if (compra == 'Y' || compra == 'y') {
            if(j->dinero < tablero[j->posicion].precio) {
                printf("FONDOS INSUFICIENTES PARA LA COMPRA :(\n");
            }
            else {
                j->dinero -= tablero[j->posicion].precio;
                j->propiedades |= (1 << j->posicion);
                tablero[j->posicion].propietario |= (turno) ? 02 : 04;
                printf("COMPRA DE PROPIEDAD EXITOSA\n");
                printf("TE QUEDAN $%d\n", j->dinero);
            }
        }
        else if (compra == 'N' || compra == 'n') {
            printf("COMPRA DE PROPIEDAD NO REALIZADA\n");
        }
}