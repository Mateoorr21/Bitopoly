// Evaluación Asincrónica II
/* Autores: Ruiz Padilla Darío Julian
            Arao Olvera Akemi Clarissa
            Rodríguez Robles Mateo
*/
// predirectivas de procesamiento
#ifndef BITOPOLY
#define BITOPOLY

// enumeración para el Tipo de Casilla
// 0: Inicio, 1: Propiedad, 2: Suerte, 3: Impuesto, 4: Carcel
typedef enum {
    INICIO, PROPIEDAD, SUERTE, IMPUESTO, CARCEL
} TipoCasilla;

// enumeración para el estado del Jugador
// 0: Bancarrota, 1: Activo
typedef enum {
    BANCARROTA, SIGUE_JUGANDO    
} EstadoJugador;

// estructura para la información de una casilla
typedef struct {
    unsigned int precio;   // 4 bytes
    unsigned int renta;    // 4 bytes
    TipoCasilla tipo;      // 4 bytes (los enums son enteros)
    char nombre[20];       // 20 bytes (múltiplo de 4, no hay padding)
    unsigned char propietario : 2;  // 1 byte (stride de 3 bytes)
} Casilla; // 36 bytes

// estructura para los elementos de un jugador
typedef struct {
    int dinero;                    // 4 bytes 
    EstadoJugador estado;          // 4 bytes (los enums son enteros)
    char nombre[20];               // 20 bytes (múltiplo de 4)
    // Bloque de campos de bits (Agrupados en unidad de 4 bytes)
    unsigned int posicion : 5;     // 5 bits: suficiente para cubrir el 0-19
    unsigned int propiedades : 20; // 20 bits: uno por cada propiedad
    // Booleano para determinar si está en la carcel
    unsigned int enCarcel : 2;     // 2 bit: booleano (puede tomar hasta 2)  
    // (6 bits de padding para los 32)
} Jugador; // 32 bytes

// Funciones de la Librería bitopoly.h
// Funciones para el inicio del juego
void capturar_nombres(Jugador jugadores[]);
void inicializar_tablero(Casilla tablero[], int tam);
void inicializar_jugadores(Jugador jugadores[]);
// Función para lanzar un dado y obtener número del 1 al 6
int lanzar_dado();
// Función recursiva para tirar los dados hasta que no haya repetidos
int tirar_hasta_no_dobles();
// Función para mover de posición un jugador y determinar si dio una vuelta
int mover_jugador(Jugador* j, int pasos);
// Función para revisar la casilla en la que cayo el jugador
void revisar_casilla(Casilla tablero[], Jugador* j, Jugador* otro, int turno);
// Funciones de acuerdo al tipo de casilla, es decir, al valor TipoCasilla
void casilla_sin_propietario(Casilla tablero[], int turno, Jugador *j);
void casilla_con_propietario(Casilla tablero[], int turno, Jugador *j, Jugador *otro);
void casilla_suerte(Jugador *j);
void casilla_impuesto(Jugador *j);
void casilla_carcel(Jugador *j);
// Función para verificar si está en bancarrota
int verificar_bancarrota(Jugador j);
// Función para imprimir una casilla del tablero (y a un jugador si se encuentra en la casilla)
void imprimir_casilla(int i, Jugador jugadores[]);
// Función para imprimir el tablero de juego actual
void imprimir_tablero(Jugador jugadores[]);
// Función para controlar el flujo del juego
void ejecutar_juego(Casilla tablero[], Jugador jugadores[], int turno);
// Función para mostrar las reglas generales del juego
void mostrar_reglas();
// Funciones para el manejo de archivos
// función para guardar el estado actual de una partida
void guardar_partida(Casilla tablero[], Jugador jugadores[], int turno);
// función para cargar una partida ya iniciada
void cargar_partida(Casilla tablero[], Jugador jugadores[], int *turno);
// función para guardar el resultado de una partida
void guardar_ranking(char ganador[], int turnos);
// función para mostrar el ranking
void mostrar_ranking();
#endif