// Evaluación Asincrónica II
/* Autores: Ruiz Padilla Darío Julian
            Arao Olvera Akemi Clarissa
            Rodríguez Robles Mateo
*/
// Cabeceras a utilizar
#include "bitopoly.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constantes utilizadas 
#define SUERTE_MIN 50   // rango inferior para el regalo de SUERTE
#define SUERTE_MAX 400  // rango superior para el regalo de SUERTE
#define TAM 20          // tamaño del tablero
#define NUMJUGADORES 2 // cantidad de jugadores

// Macro para detener el flujo de ejución después de un turno
#define ESPERA() \
    printf("Presione la tecla [ENTER] para CONTINUAR...\n"); \
    fflush(stdin); \
    getchar(); // obtenemos un caracter (caracter \n para continuar)
// Macro para limpiar pantalla usando system("cls") de stdlib.h
#define CLEAN() system("cls");
// Macro para un salto de línea
#define ENTER() printf("\n");

// Función para captuar los nombres de los jugadores
void capturar_nombres(Jugador jugadores[]) {
    // for que recorre la cantidad de jugadores
    for (int i = 0; i < NUMJUGADORES; i++) {
        printf("Ingrese el nombre del Jugador %d: ", i+1);
        // capturamos el elemento nombre de la estructura jugador con fgets (es una cadena)
        fgets(jugadores[i].nombre, sizeof(jugadores[i].nombre), stdin);
        // cambiamos el último caracter (salto de línea) por caracter de terminación para no afectar impresión
        if (jugadores[i].nombre[strlen(jugadores[i].nombre) - 1] == '\n') jugadores[i].nombre[strlen(jugadores[i].nombre) - 1] = '\0';
    }
}

// función que define cada casilla
void inicializar_tablero(Casilla tablero[]){
    int precio_inicial = 100; // precio de la primer propiedad
    tablero[0].tipo = INICIO;// Posicion 0 es el inicio (casilla 1)
    tablero[10].tipo = CARCEL;// Posicion 10 (casilla 11) es la cárcel y aquí te penalizan
    tablero[8].tipo = SUERTE;// Posicion 8 (casilla 9) es suerte, te regalan una cantidad de dinero
    tablero[16].tipo = IMPUESTO;// Posicion 16 (casilla 17) te cobran impuestos por un 40% de tu dinero

    // Ciclo for que define el resto de casillas con sus propiedades
    for (int i = 1; i < TAM; i++){ 
        // Si la posicion es la número 10, 8 o 16 saltamos esa iteración
        if(i == 10 || i == 8 || i == 16) continue;
        // De lo contrario definimos sus elementos
        tablero[i].tipo = PROPIEDAD; // es una propiedad
        sprintf(tablero[i].nombre, "Casilla %d", i+1); // nombre de casilla
        tablero[i].precio = precio_inicial; // precio
        tablero[i].renta = (int)(precio_inicial * 0.5); // la renta es el 50%
        tablero[i].propietario = 0; // inicialmente no tiene propietario
        precio_inicial += 20; // el precio va aumentando de 20 en 20
    }
}

// función para inicializar los elementos de los jugadores (todos menos el nombre que ya capturamos)
void inicializar_jugadores(Jugador jugadores[]){
    // recorremos los jugadores
    for (int i = 0; i < NUMJUGADORES; i++){
        jugadores[i].dinero = 1000;     // el dinero inicial es 0
        jugadores[i].estado = SIGUE_JUGANDO;    // estado es 1 (sigue jugando)
        jugadores[i].posicion = 0;              // inicia en posición 0 del arreglo (casilla 1)
        jugadores[i].enCarcel = 0;              // booleano en carcel Falso
        jugadores[i].propiedades = 0;           // no tiene propiedades
    }
}

// función para lanzar un dado
int lanzar_dado(){
    // devolvemos el módulo 6 de un aleatorio + 1
    // obtenemos números del 1 al 6
    return rand() % 6 + 1;
}

// función recursiva para tirar dados dobles no repetidos
int tirar_hasta_no_dobles() {
    // lanzamos dos dados utilizando la función lanzar_dado()
    int dado1 = lanzar_dado();
    int dado2 = lanzar_dado();
    // si los datos son diferentes regresamos la suma, si no llamada recursiva
    return (dado1 != dado2) ? (dado1 + dado2) : tirar_hasta_no_dobles();
}

// función para mover un jugador tantas posiciones
int mover_jugador(Jugador* j, int pasos){
    // booleano que es verdadero si el jugador completa una vuelta
    int vueltaCompleta = (j->posicion + pasos >= 20) ? 1 : 0;
    // la posición del jugador es la suma de lo del dado mod 20
    j->posicion = (j->posicion + pasos) % 20; 
    return vueltaCompleta; // devolemos el booleano (posicion ya se guardo)
}

// función para revisar la casilla en la que se cayo
void revisar_casilla(Casilla tablero[], Jugador* j, Jugador* otro, int turno){
    switch(tablero[j->posicion].tipo) {
        case INICIO:
            printf("ESTAS EN LA CASILLA DE INICIO\n");
            break;
        case PROPIEDAD:
            // No tiene propietario
            if (tablero[j->posicion].propietario == 0) casilla_sin_propietario(tablero, turno, j);
            // De lo contario, si tiene propietario
            else casilla_con_propietario(tablero, turno, j, otro);
            break;
        case SUERTE: 
            // Llamamos a función de casilla de suerte
            casilla_suerte(j); 
            break; 
        case IMPUESTO:
            // Llamamos a función de casilla de impuesto
            casilla_impuesto(j); 
            break; 
        case CARCEL:
            // Llamamos a función de casilla de carcel
            casilla_carcel(j);
    }
}

// función para si la casilla no tiene propietario
void casilla_sin_propietario(Casilla tablero[], int turno, Jugador *j) {
    unsigned char compra; // variable que lee si se hace la compra
    printf("LA PROPIEDAD NO TIENE DUEÑO. INFORMACION...\n");
    // mostramos la información de la propiedad
    printf("- Nombre de la casilla: %s\n", tablero[j->posicion].nombre);
    printf("- Precio: %d\n", tablero[j->posicion].precio);
    printf("- Renta : %d\n", tablero[j->posicion].renta);
    while(1) { // ciclo while hasta entrada valida
        printf("¿Desea comprar la propiedad (Y/N)? : ");
        scanf(" %c", &compra); // leemos respuesta
        // si el caracter no es N o Y respusta invalida
        if (compra != 'Y' && compra != 'y' && compra != 'N' && compra != 'n') {
            printf("ERROR. RESPUESTA INVALIDA. INTENTE DE NUEVO\n");
        }
        else break; // si es salimos del ciclo
    } 
    ENTER();
    // si desae hacer la compra
    if (compra == 'Y' || compra == 'y') {
        // si no le alcanza el dinero fondos insuficientes
        if(j->dinero < tablero[j->posicion].precio) {
            printf("FONDOS INSUFICIENTES PARA LA COMPRA :(\n");
        }
        // si le alcanza acutalizamos dinero, su nueva propiedad y el estado de la propiedad
        else {
            j->dinero -= tablero[j->posicion].precio;
            j->propiedades |= (1 << j->posicion);
            // activamos únicamente el segundo bit si el dueño es el jugador 2
            // si el dueño es el jugador 1 encendemos el primer bit 
            tablero[j->posicion].propietario |= (turno) ? 04 : 02;
            printf("COMPRA DE PROPIEDAD EXITOSA\n");
            printf("TE QUEDAN $%d\n", j->dinero); // mostramos dinero restante
        }
    } // si no efectua la compra
    else if (compra == 'N' || compra == 'n') {
        printf("COMPRA DE PROPIEDAD NO REALIZADA\n");
    }
}

// función para si la casilla de propiedad tiene dueño
void casilla_con_propietario(Casilla tablero[], int turno, Jugador *j, Jugador *otro) {
    // Es su propiedad
    if ((j->propiedades >> j->posicion) & 1) {
        printf("CAISTE EN TU PROPIEDAD\n");
        return;
    } 
    // Es propiedad de otro jugador
    else if ((otro->propiedades >> j->posicion) & 1) {
        // quitamos a renta a jugador y sumamos renta al otro jugador
        j->dinero -= tablero[j->posicion].renta;
        otro->dinero += tablero[j->posicion].renta;
        printf("CAISE EN UNA PROPIEDAD DE %s\n", otro->nombre);
        printf("TE ESTAN COBRANDO $%d\n", tablero[j->posicion].renta);
        printf("TE QUEDAN $%d\n", j->dinero);
    }
}

// función por si la casilla es de suerte
void casilla_suerte(Jugador *j) {
    // generamos el monto de suerte aleatorio dentro del rango
    // utilizamos los extremos del rango guardados en constantes y rand()
    int monto_suerte = rand() % (SUERTE_MAX - SUERTE_MIN + 1) + SUERTE_MIN;
    printf("¡SUERTE! has obtenido $%d\n", monto_suerte);
    j->dinero += monto_suerte; // actualizamos el dinero del jugador (sumamos)
    printf("TIENES ACTUALMENTE $%d\n", j->dinero);
}

// función por si la casilla es de impuesto
void casilla_impuesto(Jugador *j) {
    // calculamos el impuesto a cobrar como el 40% del dinero del jugador
    int impuesto = (int)(j->dinero * 0.4); 
    printf("¡IMPUESTO!, te vamos a cobrar $%d\n",impuesto);
    j->dinero -= impuesto; // actualizamos el dinero del jugador (restamos)
    printf("TE QUEDAN $%d\n", j->dinero);
}

// función por si la casilla es de carcel
void casilla_carcel(Jugador *j) {
    printf("HAS CAIDO EN LA CARCEL\n");
    unsigned char carcel; // variable que guarda si el jugador paga la fianza
    while(1) { // ciclo hasta que la entrada sea valida
        printf("¿Deseas salir pagando $200 o esperas inmóvil 2 turnos (Y/N): ");
        scanf(" %c", &carcel); // el espacio evita leer '\n'
        if (carcel != 'Y' && carcel != 'y' && carcel != 'N' && carcel != 'n') {
            printf("ERROR. RESPUESTA INVALIDA. INTENTE DE NUEVO\n");
        }
        else break; // si es valida salimos del ciclo
    } 
    // si decide pagar la fianza
    if (carcel == 'Y' || carcel == 'y') {
        if(j->dinero < 200) { // si no le alcanza
            printf("FONDOS INSUFICIENTES PARA LA FIANZA :(\n");
        }
        else { // si le alcanza
            j->dinero -= 200; // actualizamos dinero
            printf("FIANZA PAGADA EXITOSAMENTE\n");
            printf("TE QUEDAN $%d\n", j->dinero);
            return;
        }
    } // si decide no pagar la fianza
    else if (carcel == 'N' || carcel == 'n') {
        printf("FIANZA RECHAZADA. ESPERA DOS TURNOS EN CARCEL\n");
    }
    // el booleano de enCarcel se actualiza con el valor de 2
    j->enCarcel = 2; //cantidad de turnos que permanecera en la carcel           
    return;
}

// función para verificar si el jugador está en bancarrota
int verificar_bancarrota(Jugador j) {
    return (j.dinero <= 0) ? 0 : 1; // bancarrota si dinero negativo
}

// función para imprimir una casilla específica
void imprimir_casilla(int i, Jugador jugadores[]) {
    // si el jugador 1 está en la casilla 
    if (jugadores[0].posicion == i-1)
        printf("[🟥%d]", i); // imprimos su color
    // si el jugador 2 está en la casilla
    else if (jugadores[1].posicion == i-1)
        printf("[🟦%d]", i); // imprimios su colpr
    else printf("[%d]", i); // de lo contrario solo la casilla
}

// función para imprimir el tablero
void imprimir_tablero(Jugador jugadores[]){
    printf("\n--------------------------------------------");
    printf("\nJugador 1: 🟥 ");
    printf("Jugador 2: 🟦\n");
    // imprimimos para cada jugador casilla y dinero actual
    for (int i = 0; i < 2; i++){
        printf("%s, Casilla actual: %d, Dinero actual: $%d. \n",jugadores[i].nombre, jugadores[i].posicion+1, jugadores[i].dinero);
    }
    ENTER(); // imprimimos primero lo superior de izquierda -> derecha
    for (int i = 1; i <= 6; i++) {
        imprimir_casilla(i, jugadores); // imprimimos casilla
    }
    ENTER();
    // imprimimos los laterales del tablero de arriba a abajo
    // el izquierdo comienza en 20 y acabo en 17, el derecho es de 7-10
    for (int i = 7; i <= 10; i++) {
        imprimir_casilla(27 - i, jugadores);
        printf("              "); // espacio entre columnas
        imprimir_casilla(i, jugadores);
        ENTER();
    }
    // imprimimos de izquierda a derecha de manera descendente (16-11)
    for (int i = 16; i >= 11; i--) {
        imprimir_casilla(i, jugadores);
    }
    ENTER();
}

// función para controlar el flujo del juego
void ejecutar_juego(Casilla tablero[], Jugador jugadores[], int turno) {
    int dado = 0, turnosTotales = 0; // dado y turnos total
    // mientras los dos jugadores no se encuentren en bancarrota
    // hay un siguiente turno
    while (jugadores[0].estado && jugadores[1].estado) { 
        turnosTotales++; // aumentamos un turno
        printf("\n\nTurno %d - Va %s.", turnosTotales, jugadores[turno].nombre);
        imprimir_tablero(jugadores); // imprimimos tablero
        // si el jugador está en la carcel (usamos booleano)
        if(jugadores[turno].enCarcel != 0) {
            jugadores[turno].enCarcel--; // restamos un turno
            printf("\nUSTED ESTA EN LA CARCEL. PIERDE SU TURNO\n");
            printf("\nLE QUEDAN %d TURNO(S) EN LA CARCEL\n", jugadores[turno].enCarcel);
        } // el jugador no tiene turno
        // si no está en la carcel, tien eturno
        else {
            dado = tirar_hasta_no_dobles(); // tiramos los dados
            printf("\nResultado de los dados: %d\n", dado);
            // movemos al jugador y obtenemos si este completo una vuelta
            int vueltaCompleta = mover_jugador(&(jugadores[turno]), dado);
            if(vueltaCompleta) { // si la completo se le suman $25
                printf("HAS COMPLETADO UNA VUELTA. RECIBES $25\n");
                jugadores[turno].dinero += 25;
            }
            printf("Caíste en la casilla %d\n", jugadores[turno].posicion+1);
            ENTER();
            // revisamos la casilla en la que cayó con la función revisar_casilla
            revisar_casilla(tablero, &(jugadores[turno]), &(jugadores[(turno == 0) ? 1 : 0]), turno);
            // actualizamos el estado del jugador llamando a la función bancarrota
            jugadores[turno].estado = verificar_bancarrota(jugadores[turno]);
        }
        // si el jugador actual no está en bancarrota
        if(jugadores[turno].estado == 1)  {
            turno = (turno == 0) ? 1 : 0; // operador ternario para cambiar turno
            guardar_partida(tablero, jugadores, turno); // guardamos la partida
        }
        ESPERA(); 
        CLEAN();
    }
    // una vez el jugador en turno este en bancarrota, se tiene ganador
    printf("\n%s HA QUEDADO EN BANCARROTA.\n",  jugadores[turno].nombre);
    printf("¡ %s ES EL GANADOR !\n", jugadores[(turno == 0) ? 1 : 0].nombre);
    // guardamos el ganador,  los turnos en el ranking 
    guardar_ranking(jugadores[(turno == 0) ? 1 : 0].nombre, turnosTotales);
    // borramos la partida guardada ya que ya terminó
    remove("datos/bitopoly_save.dat");
    ESPERA();
    CLEAN();
}

// función para mostrar las reglas del juego Bitopoly
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

// función para guardar la partida después de cada turno realizado
// no usamos puntero en turno porque solo queremos el valor contenido en la direccion
void guardar_partida(Casilla tablero[], Jugador jugadores[], int turno) {
    // abrimos el archivo bitopoly_save.dat con fopen
    // pasamos parametro wb para abrir en modo escritura binaria
    // crea el archivo si no existe, si existe borra su contenido
    FILE *archivo = fopen("datos/bitopoly_save.dat", "wb"); 
    if (archivo == NULL) { // si no se encuentra la carpeta Datos
        printf("ERROR AL GUARDAR LA PARTIDA.\n"); // Error
        return;
    }
    // Escribimos el estado del juego 
    // fwrite recibe el puntero desde el que se copia, el tamaño de cada elemento
    // la cantidad de elementos que se copian y el archivo al que se copian
    fwrite(tablero, sizeof(Casilla), 20, archivo); // tablero
    fwrite(jugadores, sizeof(Jugador), 2, archivo); // jugadores
    fwrite(&turno, sizeof(int), 1, archivo); // turno actual
    fclose(archivo); // cerramose el archivo
}

// función para cargar una partida que se ha iniciado
// se utiliza el puntero de turno para modificar su valor con el contenido en el archivo
void cargar_partida(Casilla tablero[], Jugador jugadores[], int *turno) {
    // abrimos el archivo en modo rb de lectura binaria
    FILE *archivo = fopen("datos/bitopoly_save.dat", "rb");
    if (archivo == NULL) { // si no hay partida error
        printf("ERROR. NO SE ENCONTRÓ NINGUNA PARTIDA GUARDADA\n");
        ESPERA(); 
        CLEAN();
    }
    else {
        // Cargamos el tablero contenido en el archivo
        fread(tablero, sizeof(Casilla), TAM, archivo);
        // Cargamos la información de los jugadores en la partida
        fread(jugadores, sizeof(Jugador), 2, archivo);
        // Cargamos el turno actual
        fread(turno, sizeof(int), 1, archivo);
        fclose(archivo); // Cerramos el archivo con los datos
        printf("PARTIDA CARGADA EXITOSAMENTE\n");
        // Llamar a la función para el ciclo del juego
        ejecutar_juego(tablero, jugadores, *turno);
    }
}

// función para guardar el resultado de una partida en el ranking (ganador y turnos totales)
void guardar_ranking(char ganador[], int turnos) {
    // abrimos el archivo en modo "a" para añadir al final, no borramos
    FILE *archivo = fopen("datos/ranking.txt", "a");
    if (archivo == NULL) {
        printf("ERROR. NO SE PUDO ACCEDER AL ARCHIVO CON EL RANKING.\n");
        return;
    }
    // %-15s para un ancho de por lo menos 15 caracteres alineados a la izquierda
    fprintf(archivo, "Ganador: %-15s | Turnos totales: %d\n", ganador, turnos);
    fclose(archivo);
}

// función para mostrar el ranking histórico del resultado de todas las partidas realizadas
void mostrar_ranking() {
    // abrimos el archivo en modo r para lectura
    FILE *archivo = fopen("datos/ranking.txt", "r");
    if (archivo == NULL) { // si no se tiene archivo
        printf("\nEL RANKING ESTA VACIO O TODAVÍA NO EXISTE.\n");
        return;
    }
    char linea[100]; // cadena para cada renglon (cada partida)
    printf("\n========== RANKING HISTORICO ==========\n");
    while (fgets(linea, sizeof(linea), archivo)) { // mientras haya partidas
        printf("%s", linea); // imprimimos la línea con el resultado
    }
    printf("=======================================\n");
    fclose(archivo); // cerramos archivo
    ESPERA();
    CLEAN();
}