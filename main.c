// Evaluación Asincrónica II
/* Autores: Ruiz Padilla Darío Julian
            Arao Olvera Akemi Clarissa
            Rodríguez Robles Mateo
*/

// Enumeración para identificar el tipo de cada casilla en el tablero 
// typedef enum { INICIO, PROPIEDAD, SUERTE, IMPUESTO, CARCEL } TipoCasilla;

// Enumeración para controlar el estado de actividad de los jugadores
// typedef enum { SIGUE_JUGANDO, BANCARROTA } EstadoJugador;

// Estructura que define las propiedades y características de una casilla 
// Campos: tipo, nombre[20], precio, renta, propietario (campo de bits: 2)
// typedef struct { ... } Casilla;

// Estructura que almacena la información financiera y de progreso del jugador
// Campos: nombre[20], dinero, posicion, propiedades (20 bits), estado
// typedef struct { ... } Jugador;

/* * FUNCIONES MÍNIMAS OBLIGATORIAS PARA LA LÓGICA DE JUEGO
 */

// Configura los valores iniciales del arreglo de 20 casillas
// void inicializar_tablero(Casilla tablero[], int tam);

// Establece el nombre, dinero inicial y estado de los 2 jugadores
// void inicializar_jugadores(Jugador jugadores[], int num_jugadores);

// Genera un número aleatorio simulando un dado 
// int lanzar_dado();

// Actualiza la posición del jugador de forma circular (módulo 20)
// void mover_jugador(Jugador* j, int pasos);

// Gestiona acciones (compra, pago de renta o eventos) según el tipo de casilla 
// void revisar_casilla(Casilla tablero[], Jugador* j, Jugador* otro);

// Evalúa si el dinero del jugador es menor a cero para declarar quiebra
// int verificar_bancarrota(Jugador j);

// Función recursiva: lanza dados hasta que los valores sean distintos y retorna la suma 
// int tirar_hasta_no_dobles();

// Cabeceras a utilizar
#include <stdio.h>  
#include "bitopoly.h" // Función propia
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define TAM 20 // tamaño del tablero será de 20
#define NUMJUGADORES 2 // cantidad de jugadores


int main(void) {
    srand(time(NULL));

    Casilla tablero[TAM];
    Jugador jugadores[2];
    inicializar_tablero(tablero, TAM);
    inicializar_jugadores(jugadores, NUMJUGADORES);
    int turno = 0;
    int option;
    do { 
        printf("Bienvenido al Menú del Juego Bitopoly....\n");
        printf("1. Iniciar Nueva Partida\n");
        printf("2. Cargar Partida Existente\n");
        printf("3. Reglas Generales del Juego\n");
        printf("4. Salir\n");
        printf("Selecciona una opción:\n");
        scanf("%d", &option);
        switch (option) {
            case 1:
                ejecutar_juego(tablero, jugadores, turno);
                break;
            case 2:
                FILE *archivo = fopen("datos/bitopoly_save.dat", "rb");

                if (archivo == NULL) {
                    printf("ERROR. NO SE ENCONTRÓ NINGUNA PARTIDA GUARDADA\n");
                } 
                else {
                    // Cargamos el tablro contenido en el archivo
                    fread(tablero, sizeof(Casilla), TAM, archivo);
                    // Cargamos la información de los jugadores en la partida
                    fread(jugadores, sizeof(Jugador), 2, archivo);
                    // Cargamos el turno actual
                    fread(&turno, sizeof(int), 1, archivo);
                    fclose(archivo); // Cerramos el archivo von los datos
                    printf("PARTIDA CARGADA EXITÓSAMENTE\n");
                    // Llamar a la función para el ciclo del juego
                    ejecutar_juego(tablero, jugadores, turno);
                }
                break;
            case 3:
                mostrar_reglas();
                break;
                
            case 4:
                printf("GRACIAS POR ELEGIR BITOPOLY\n");
                break;
            default:
                // si no es una opción válida lo indicamos. solicitamos que ingrese de nuevo
                printf("OPCION NO VÁLIDA. INTENTE DE NUEVO\n");
        }
    } while (option != 4);
    return 0;
}

