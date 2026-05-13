#ifndef BITOPOLY
#define BITOPOLY

typedef enum {
    INICIO, PROPIEDAD, SUERTE, IMPUESTO, CARCEL
} TipoCasilla;

typedef enum {
    BANCARROTA, SIGUE_JUGANDO    
} EstadoJugador;

typedef struct {
    TipoCasilla tipo;
    char nombre[20];
    unsigned int precio;
    unsigned int renta;
    unsigned char propietario : 2; 
} Casilla;

typedef struct {
    char nombre[20];
    int dinero;
    unsigned int posicion;
    unsigned int propiedades;
    unsigned int enCarcel;
    EstadoJugador estado;
} Jugador;

void inicializar_tablero(Casilla tablero[], int tam);
void inicializar_jugadores(Jugador jugadores[], int num_jugadores);
int lanzar_dado();
int mover_jugador(Jugador* j, int pasos);
void revisar_casilla(Casilla tablero[], Jugador* j, Jugador* otro, int turno);
int verificar_bancarrota(Jugador j);
int tirar_hasta_no_dobles();
void ejecutar_juego(Casilla tablero[], Jugador jugadores[], int turno);
void imprimir_tablero(Casilla tablero[], Jugador jugadores[], int turno);
char* obtener_casilla_i(int i, Jugador jugadores[]);
void mostrar_reglas();
void casilla_sin_propietario(Casilla tablero[], int turno, Jugador *j);

#endif