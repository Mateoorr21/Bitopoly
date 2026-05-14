/* Autores: Ruiz Padilla Darío Julian
            Arao Olvera Akemi Clarissa
            Rodríguez Robles Mateo
*/
// Cabeceras a utilizar
#include "bitopoly.h" // Librería propia
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Definicion de constantes
#define TAM 20 // tamaño del tablero será de 20
#define NUMJUGADORES 2 // cantidad de jugadores

int main(void) {
    srand(time(NULL)); // inicializamos generador de aleatorios
    Casilla tablero[TAM]; // tablero con 20 casillas
    Jugador jugadores[NUMJUGADORES]; // 2 jugadores
    inicializar_tablero(tablero); // inicializamos tablero
    inicializar_jugadores(jugadores); // inicializamos jugadores
    int turno = 0, turnosTotales = 0; // el primer turno es del jugador 1
    int option; // opcion
    // menú principal
    do { 
        printf("Bienvenido al Menú del Juego Bitopoly....\n");
        // opciones del menu
        printf("1. Iniciar Nueva Partida\n");
        printf("2. Cargar Partida Existente\n");
        printf("3. Mostrar Ranking\n");
        printf("4. Reglas Generales del Juego\n");
        printf("5. Salir\n");
        printf("Selecciona una opción:\n");
        scanf("%d", &option); // capturamos la opción
        getchar(); // limpia el \n para que no afecte
        switch (option) {
            // si decide iniciar una partida nueva
            case 1:
                capturar_nombres(jugadores); // capturamos los nombres
                ejecutar_juego(tablero, jugadores, turno, turnosTotales); // iniciamos el juego
                break;
            case 2:
            // si decide cargar una partida existente 
                // pasamos turno y turnosTotales por referencia para actualizarlos
                cargar_partida(tablero, jugadores, &turno, &turnosTotales);
                break;
            case 3:
            // si decide mostrar el ranking histórico de partidas
                mostrar_ranking();
                break;
            case 4:
            // si decide mostrar las reglas
                mostrar_reglas();
                break;
            case 5:
            // si decide salir del menú
                printf("GRACIAS POR ELEGIR BITOPOLY\n");
                break;
            default:
                // si no es una opción válida lo indicamos. solicitamos que ingrese de nuevo
                printf("OPCION NO VÁLIDA. INTENTE DE NUEVO\n");
                ESPERA();
                CLEAN();
        }
    } while (option != 5); // mientras la opción sea diferente a salir
    return 0;
}

