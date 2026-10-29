// Uso:  ./tetris           corre los tests y despues el juego
//       ./tetris --test    corre solo los tests (valgrind --leak-check=full ./tetris --test)

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h> // Usado para la función de test

// ========= CONSTANTES DEL JUEGO =========
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 800
#define GRID_ROWS 20
#define GRID_COLS 10
#define CELL_SIZE 35
#define OFFSET_X 75
#define OFFSET_Y 50
#define TICK_RATE 30

const int TETROMINOS[7][4][4][4] = {
    { {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}}, {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}}, {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}} },
    { {{2,0,0,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}}, {{0,2,2,0},{0,2,0,0},{0,2,0,0},{0,0,0,0}}, {{0,0,0,0},{2,2,2,0},{0,0,2,0},{0,0,0,0}}, {{0,2,0,0},{0,2,0,0},{2,2,0,0},{0,0,0,0}} },
    { {{0,0,3,0},{3,3,3,0},{0,0,0,0},{0,0,0,0}}, {{0,3,0,0},{0,3,0,0},{0,3,3,0},{0,0,0,0}}, {{0,0,0,0},{3,3,3,0},{3,0,0,0},{0,0,0,0}}, {{3,3,0,0},{0,3,0,0},{0,3,0,0},{0,0,0,0}} },
    { {{0,4,4,0},{0,4,4,0},{0,0,0,0},{0,0,0,0}}, {{0,4,4,0},{0,4,4,0},{0,0,0,0},{0,0,0,0}}, {{0,4,4,0},{0,4,4,0},{0,0,0,0},{0,0,0,0}}, {{0,4,4,0},{0,4,4,0},{0,0,0,0},{0,0,0,0}} },
    { {{0,5,5,0},{5,5,0,0},{0,0,0,0},{0,0,0,0}}, {{0,5,0,0},{0,5,5,0},{0,0,5,0},{0,0,0,0}}, {{0,0,0,0},{0,5,5,0},{5,5,0,0},{0,0,0,0}}, {{5,0,0,0},{5,5,0,0},{0,5,0,0},{0,0,0,0}} },
    { {{0,6,0,0},{6,6,6,0},{0,0,0,0},{0,0,0,0}}, {{0,6,0,0},{0,6,6,0},{0,6,0,0},{0,0,0,0}}, {{0,0,0,0},{6,6,6,0},{0,6,0,0},{0,0,0,0}}, {{0,6,0,0},{6,6,0,0},{0,6,0,0},{0,0,0,0}} },
    { {{7,7,0,0},{0,7,7,0},{0,0,0,0},{0,0,0,0}}, {{0,0,7,0},{0,7,7,0},{0,7,0,0},{0,0,0,0}}, {{0,0,0,0},{7,7,0,0},{0,7,7,0},{0,0,0,0}}, {{0,7,0,0},{7,7,0,0},{7,0,0,0},{0,0,0,0}} }
};

SDL_Color colors[8] = {
    {30, 30, 30, 255}, {0, 255, 255, 255}, {0, 0, 255, 255}, {255, 165, 0, 255},
    {255, 255, 0, 255}, {0, 255, 0, 255}, {128, 0, 128, 255}, {255, 0, 0, 255}
};

// ========= ESTRUCTURAS DE DATOS =========

// 1. Nodo para la fila del tablero (Lista Enlazada)
typedef struct RowNode {
    int cells[GRID_COLS]; 
    struct RowNode* next; 
} RowNode;

// 2. Nodo para la cola de próximas piezas (Queue)
typedef struct PieceQueueNode {
    int type; 
    struct PieceQueueNode* next;
} PieceQueueNode;

// 3. Nodo para el sistema de partículas 
typedef struct ParticleNode {
    float x, y;
    float vel_x, vel_y;
    int color;
    int life_timer;
    struct ParticleNode* next;
} ParticleNode;

typedef struct ActivePiece {
    int col, row;
    int type;      
    int rotation;  
    int active;
} ActivePiece;

typedef struct GameBoard {
    RowNode* first_row;             
    PieceQueueNode* queue_head;     
    PieceQueueNode* queue_tail;     
    ParticleNode* first_particle;   
    ActivePiece current_piece;
    int score;
    int tick_timer;
} GameBoard;

// ========= VARIABLES GLOBALES =========
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
GameBoard* game_board = NULL;

// ========= EJERCICIO 1: STRINGS (NO USAR string.h) =========

char* strDuplicate(char* src) {
    // TODO: Contar los caracteres de src, reservar con malloc (no olvidar el '\0'),
    //       copiar todo y devolver el puntero nuevo.
    return NULL;
}

int strCompare(char* s1, char* s2) {
    // TODO: Devolver 0 si son iguales, 1 si s1 < s2, -1 si s1 > s2.
    //       Ojo: es la convencion INVERSA a strcmp.
    return 0;
}

char* strConcatenate(char* src1, char* src2) {
    // TODO: Reservar un string nuevo con src1 seguido de src2, y liberar src1 y src2.
    //       Precondicion: src1 y src2 vienen de malloc y son punteros distintos.
    return NULL;
}

// ========= EJERCICIO 2: FUNCIONES A IMPLEMENTAR (EL TP) =========

void gameBoardDelete(GameBoard* board) {
    //  Debe tolerar board == NULL.

    if (board == NULL) return; // no hay nada que hacer

    // Recorrer y liberar todos los RowNode con free().

    RowNode* row_actual = board -> first_row; // inicializo en el primer nodo

    while (row_actual != NULL){ //libero el actual y avanzo al siguiente hasta que no haya nada
        RowNode* siguiente = row_actual -> next;
        free(row_actual);
        row_actual = siguiente;
    }

    // Recorrer y liberar todos los PieceQueueNode. Mismo algoritmo que antes.

    PieceQueueNode* piece_actual = board -> queue_head;

    while (piece_actual != NULL){
        PieceQueueNode* siguiente = piece_actual -> next;
        free(piece_actual);
        piece_actual = siguiente;
    }

    // Recorrer y liberar todos los ParticleNode.

    ParticleNode* particle_actual = board -> first_particle;

    while (particle_actual != NULL){
        ParticleNode* siguiente = particle_actual -> next;
        free(particle_actual);
        particle_actual = siguiente;
    }

    // Finalmente, liberar la estructura GameBoard.

    free(board);
}

void setRowToZero(RowNode* row){
    for (int i = 0 ; i < GRID_COLS ; i++){
        row -> cells[i] = 0;
    }
}

GameBoard* gameBoardNew() {
    // Reservar memoria para el GameBoard.

    GameBoard *new_GameBoard = (GameBoard*) malloc(sizeof(GameBoard));
    if (new_GameBoard == NULL){
        return NULL; // error de malloc
    }

    // Inicializar queue_head, queue_tail y first_particle en NULL
    // Inicializar score y tick_timer en 0.
    // Inicializar current_piece con una pieza inactiva.

    new_GameBoard->queue_head = NULL;
    new_GameBoard->queue_tail = NULL;
    new_GameBoard->first_particle = NULL;
    new_GameBoard->tick_timer = 0;
    new_GameBoard->score = 0;
    new_GameBoard->current_piece = (ActivePiece){0,0,0,0,0}; // Pone todos los parametros de active piece en 0.

    // Usar un bucle para hacer malloc de las 20 filas (RowNode) y conectarlas.

    new_GameBoard -> first_row = NULL; 
    RowNode* last_row = NULL;
    for (int i = 0 ; i < GRID_ROWS ; i++){
        RowNode* new_row = (RowNode*) malloc(sizeof(RowNode));
        if (new_row == NULL){ //error de malloc, desalojar todo
            gameBoardDelete(new_GameBoard);
            return NULL;
        }
        setRowToZero(new_row);
        new_row -> next = NULL;

        if (last_row == NULL){ // El puntero first_row debe apuntar al primero de esos nodos.
            new_GameBoard -> first_row = new_row;
        }
        else{
            last_row -> next = new_row; // enlazo el nueo nodo al anterior
        }
        last_row = new_row; // acutualizo last_row
    }

    return new_GameBoard;
}

void gameBoardAddParticle(GameBoard* board, float x, float y, int color) {
    // Reservamos memoria para el nuevo nodo.
    ParticleNode* new_particle_node = (ParticleNode*) malloc(sizeof(ParticleNode));
    if (new_particle_node == NULL) { // Error de malloc.
        return;
    }

    // Inicializamos los valores del nodo.
    new_particle_node->x = x;
    new_particle_node->y = y;
    new_particle_node->color = color;

    // Velocidades aleatorias y tiempo de vida fijo.
    new_particle_node->vel_x = ((rand() % 100) / 10.0f) - 5.0f; // Valores sacados de juego_base.c
    new_particle_node->vel_y = ((rand() % 50) / -10.0f) - 2.0f;
    new_particle_node->life_timer = 30 + (rand() % 30);

    // Agregamos el nuevo nodo al principio de la lista de partículas.
    new_particle_node->next = board->first_particle;
    board->first_particle = new_particle_node; 
}

int isRowComplete(RowNode* row) {
    for (int i = 0; i < GRID_COLS ; i++){ // Miro todas las columnas
        if (row -> cells[i] == 0) return 0; // Cuando aparezca un 0 la fila no está completa
    }
    return 1; // Caso contrario, la fila está completa
}

int gameBoardCheckAndClearLines(GameBoard* board) {
    if (board == NULL) return 0;

    // Recorrer la lista de RowNode.

    RowNode* row_actual = board -> first_row;
    RowNode* row_anterior = NULL;

    int filasEliminadas = 0;
    int filaIndex = 0;
    while(row_actual != NULL){
        if (isRowComplete(row_actual)){ // Si un RowNode tiene todas sus celdas > 0, desenlazarlo y hacer free().
            RowNode* fila_a_borrar = row_actual;
            if (row_anterior == NULL){ // Caso border donde la primera fila está completa
                board -> first_row = row_actual -> next;
                row_actual = board -> first_row; // el segundo elemento pasa a ser el primero
            }
            else{
                row_anterior -> next = row_actual -> next; // conecto el anterior con el siguiente
                row_actual = row_anterior -> next;
            }

            // Por cada celda destruida, invocar a gameBoardAddParticle().

            for (int i = 0 ; i < GRID_COLS ; i++){
                int x = OFFSET_X + i * CELL_SIZE;
                int y = OFFSET_Y + filaIndex * CELL_SIZE;
                int color = fila_a_borrar -> cells[i];
                gameBoardAddParticle(board, x, y, color);
            }

            // Libero row e incremento el contador
            free(fila_a_borrar);
            filasEliminadas++;
            filaIndex++;
            continue;
        }
        
        row_anterior = row_actual;
        row_actual = row_actual -> next;
        filaIndex++;
    }

    for (int i = 0 ; i < filasEliminadas ; i++){
        // Crear un nuevo RowNode vacío (lleno de ceros).

        RowNode* new_row = (RowNode*) malloc(sizeof(RowNode));
        if (new_row == NULL){
            printf("Error de memoria al reemplazar fila\n");
            gameBoardDelete(board);
            exit(EXIT_FAILURE); // Salgo por error en memoria
        }
        setRowToZero(new_row);
        
        // Insertar el nuevo RowNode en la CABEZA del tablero (board->first_row).

        new_row -> next = board -> first_row;
        board -> first_row = new_row;
    }

    return filasEliminadas;
}

void gameBoardEnqueuePiece(GameBoard* board, int type) {
    // Reservamos memoria para el nuevo nodo.
    PieceQueueNode* new_piece_node = (PieceQueueNode*) malloc(sizeof(PieceQueueNode));
    if (new_piece_node == NULL) { // Error de malloc.
        return; 
    }

    new_piece_node->type = type;   // Inicializamos los valores del nodo.
    new_piece_node->next = NULL; 

    if (board->queue_head == NULL){ // Si la cola está vacía.
        board->queue_head = new_piece_node; // El nuevo nodo será el primero y último.
        board->queue_tail = new_piece_node;
    }

    else { // Si hay piezas en la cola.
        board->queue_tail->next = new_piece_node; // Enlazamos el nuevo nodo al último.
        board->queue_tail = new_piece_node; // Actualizamos el último nodo.
    }
}

int gameBoardDequeuePiece(GameBoard* board, int* out_type) {  
    // Si la cola está vacía, no extraemos nada.
    if (board->queue_head == NULL){
        return 0;
    }
    PieceQueueNode* first_piece_node = board->queue_head; // Guardamos el primer nodo para liberarlo más tarde.
    *out_type = first_piece_node->type; // Guardamos el tipo de pieza extraído. 
    board->queue_head = first_piece_node->next; // Actualizamos el primer nodo de la cola.

    if (board->queue_head == NULL){ // Si la cola queda vacía después de extraer la pieza
        board->queue_tail = NULL; // Actualizamos el último nodo de la cola a NULL.
    }

    free(first_piece_node); // Liberamos la memoria del nodo que extraímos.
    return 1;
}

// ========= LOGICA DE DIBUJADO Y COLISIONES (NO MODIFICAR) =========

RowNode* getRowAt(GameBoard* board, int index) {
    RowNode* curr = board->first_row;
    for(int i = 0; i < index && curr != NULL; i++) curr = curr->next;
    return curr;
}

int canMove(GameBoard* board, int target_col, int target_row, int target_rot) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TETROMINOS[board->current_piece.type][target_rot][r][c] != 0) {
                int board_col = target_col + c;
                int board_row = target_row + r;
                if (board_col < 0 || board_col >= GRID_COLS || board_row >= GRID_ROWS) return 0;
                if (board_row >= 0) {
                    RowNode* row_node = getRowAt(board, board_row);
                    if (row_node && row_node->cells[board_col] != 0) return 0;
                }
            }
        }
    }
    return 1;
}

void lockPiece(GameBoard* board) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int val = TETROMINOS[board->current_piece.type][board->current_piece.rotation][r][c];
            if (val != 0) {
                RowNode* row_node = getRowAt(board, board->current_piece.row + r);
                if (row_node) row_node->cells[board->current_piece.col + c] = val;
            }
        }
    }
    board->current_piece.active = 0;
    board->score += gameBoardCheckAndClearLines(board) * 100;
}

// Devuelve 0 si la pieza nueva no entra (GAME OVER), 1 en otro caso.
int spawnNextPiece(GameBoard* board) {
    int type;
    if (gameBoardDequeuePiece(board, &type)) {
        board->current_piece.type = type;
        board->current_piece.rotation = 0;
        board->current_piece.col = GRID_COLS / 2 - 2;
        board->current_piece.row = 0;
        board->current_piece.active = 1;
        gameBoardEnqueuePiece(board, rand() % 7);
        if (!canMove(board, board->current_piece.col, board->current_piece.row, board->current_piece.rotation)) {
            printf("GAME OVER. Puntaje final: %d\n", board->score);
            board->current_piece.active = 0;
            return 0;   // main sale del loop y libera tablero y SDL
        }
    }
    return 1;
}

// Devuelve 0 cuando termina la partida.
int gameBoardUpdate(GameBoard* board) {
    if (!board) return 0;
    if (!board->current_piece.active && !spawnNextPiece(board)) return 0;

    board->tick_timer++;
    if (board->tick_timer >= TICK_RATE) {
        board->tick_timer = 0;
        if (canMove(board, board->current_piece.col, board->current_piece.row + 1, board->current_piece.rotation)) {
            board->current_piece.row++;
        } else {
            lockPiece(board);
        }
    }

    ParticleNode* curr_part = board->first_particle;
    ParticleNode* prev_part = NULL;
    while (curr_part) {
        curr_part->x += curr_part->vel_x;
        curr_part->y += curr_part->vel_y;
        curr_part->vel_y += 0.5f;
        curr_part->life_timer--;

        if (curr_part->life_timer <= 0) {
            ParticleNode* to_delete = curr_part;
            if (prev_part == NULL) {
                board->first_particle = curr_part->next;
                curr_part = board->first_particle;
            } else {
                prev_part->next = curr_part->next;
                curr_part = prev_part->next;
            }
            free(to_delete);
        } else {
            prev_part = curr_part;
            curr_part = curr_part->next;
        }
    }
    return 1;
}

void gameBoardDraw(GameBoard* board) {
    if (!board) return;
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect board_rect = {OFFSET_X - 2, OFFSET_Y - 2, GRID_COLS * CELL_SIZE + 4, GRID_ROWS * CELL_SIZE + 4};
    SDL_RenderDrawRect(renderer, &board_rect);

    RowNode* current_row = board->first_row;
    int r = 0;
    while (current_row) {
        for (int c = 0; c < GRID_COLS; c++) {
            if (current_row->cells[c] > 0) {
                SDL_Color col = colors[current_row->cells[c]];
                SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);
                SDL_Rect cell_rect = {OFFSET_X + c * CELL_SIZE, OFFSET_Y + r * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1};
                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
        current_row = current_row->next;
        r++;
    }

    if (board->current_piece.active) {
        for(int pr = 0; pr < 4; pr++) {
            for(int pc = 0; pc < 4; pc++) {
                int cell_val = TETROMINOS[board->current_piece.type][board->current_piece.rotation][pr][pc];
                if (cell_val != 0) {
                    SDL_Color col = colors[cell_val];
                    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);
                    SDL_Rect p_rect = {OFFSET_X + (board->current_piece.col + pc) * CELL_SIZE, 
                                       OFFSET_Y + (board->current_piece.row + pr) * CELL_SIZE, 
                                       CELL_SIZE - 1, CELL_SIZE - 1};
                    SDL_RenderFillRect(renderer, &p_rect);
                }
            }
        }
    }

    ParticleNode* curr_part = board->first_particle;
    while (curr_part) {
        SDL_Color col = colors[curr_part->color];
        int alpha = curr_part->life_timer * 8;   // se satura: SDL recibe un Uint8
        if (alpha > 255) alpha = 255;
        SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, alpha);
        SDL_Rect part_rect = {(int)curr_part->x, (int)curr_part->y, 6, 6};
        SDL_RenderFillRect(renderer, &part_rect);
        curr_part = curr_part->next;
    }
    SDL_RenderPresent(renderer);
}

int inicializar() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    window = SDL_CreateWindow("Tetris Base - TP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) return 0;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) return 0;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return 1;
}

// ========= TESTS (COMPLETAR) =========

int checkRowCount(GameBoard* board){
    if (board == NULL || board -> first_row == NULL){ // Caso sin tablero
        return 0;
    }

    RowNode* curr_row = board -> first_row;
    int res = 0;

    while (curr_row != NULL){ // itero todas las filas hasta el final incrementando el contador
        curr_row = curr_row -> next;
        res++;
    }
    
    return res;
}

void fillRow(GameBoard* board, int rowIndex){
    if (board == NULL || board -> first_row == NULL){
        return;
    }

    RowNode* curr_row = board -> first_row;
    int i = 0;

    while (curr_row != NULL){ // Busca la fila a llenar
        if(i == rowIndex){
            for (int j = 0 ; j < GRID_COLS ; j++){ // LLena todas las celdas con valores >0
                curr_row -> cells[j] = 1;
            }
        }
        curr_row = curr_row -> next;
        i++;
    }
}

int areStringsEqual(char* str1, char* str2){
    while(*str1 != '\0' && *str2 != '\0'){ // Avanza en los srings a la vez hasta que uno se completa
        if(*str1 != *str2){ // Comprueba igualdad de caracteres
            return 0;
        }
        str1++;
        str2++;
    }

    if ((*str1 == '\0' && *str2 != '\0') || (*str1 != '\0' && *str2 == '\0')){ // Si hay uno que no termino son distintos
        return 0;
    }
    else{ // Si ambos terminaron son iguales
        return 1;
    }
}

int isRowEmpty(RowNode* row){
    for (int i = 0; i < GRID_COLS ; i++){ // Miro todas las columnas
        if (row -> cells[i] != 0) return 0; // Cuando aparezca algo != 0 la fila no está completa
    }
    return 1; // Caso contrario, la fila está vacía
}

void correrTests() {
    //  - strDuplicate: "", "x", un string con distintos caracteres.

    char* str_duplicado1 = strDuplicate("");
    char* str_duplicado2 = strDuplicate("x");
    char* str_duplicado3 = strDuplicate("ae2@G)&~!]+");

    assert(areStringsEqual("", str_duplicado1) && "String duplicado no matchea");
    assert(areStringsEqual("x", str_duplicado2) && "String duplicado no matchea");
    assert(areStringsEqual("ae2@G)&~!]+", str_duplicado3) && "String duplicado no matchea");

    free(str_duplicado1);
    free(str_duplicado2);
    free(str_duplicado3);

    //  - strCompare: "" vs "", "a" vs "a", y una desigualdad en ambos sentidos (s1/s2 y s2/s1).

    int resultado_compare = strCompare("", "");
    assert(resultado_compare==0 && "Error strings vacíos");

    resultado_compare = strCompare("a", "a");
    assert(resultado_compare==0 && "Error strings iguales");

    resultado_compare = strCompare("b", "a");
    assert(resultado_compare==-1 && "Error string mayor");

    resultado_compare = strCompare("a", "b");
    assert(resultado_compare==1 && "Error string menos");

    resultado_compare = strCompare("perro", "perros");
    assert(resultado_compare==1 && "Error string prefijo");

    //  - strConcatenate: "" + "abc" y "hola " + "mundo". Los argumentos deben crearse con
    //    strDuplicate, porque strConcatenate los libera (no pasar literales).

    char* str_vacio = strDuplicate("");
    char* str_abc = strDuplicate("abc");
    char* str_hola = strDuplicate("hola ");
    char* str_mundo = strDuplicate("mundo");

    char* str_concatenado1 = strConcatenate(str_vacio, str_abc);
    char* str_concatenado2 = strConcatenate(str_hola, str_mundo);

    assert(areStringsEqual(str_concatenado1, "abc") && "Error al concatenar con string vacío");
    assert(areStringsEqual(str_concatenado2, "hola mundo") && "Error al concatenar 2 strings");

    free(str_concatenado1);
    free(str_concatenado2);

    //  - Cola: 5 push y 6 pop; el 6to debe devolver 0 y dejar queue_head y queue_tail en NULL.

    GameBoard* board = gameBoardNew();

    for (int i = 0; i < 5 ; i++){
        gameBoardEnqueuePiece(board, i);
    }

    assert((board -> queue_head != NULL && board -> queue_tail != NULL) && "Error en push a queue"); // Me fijo si se crearon nodos
    PieceQueueNode* curr_element = board -> queue_head;

    int order[5] = {0,1,2,3,4};
    int index = 0;
    while (curr_element != NULL){ // Recorro la secuencia de nodos para ver que siga el orden esperado
        assert(curr_element -> type == order[index] && "Error orden FIFO en push");
        curr_element = curr_element -> next;
        index++;
    }

    int outputDequeueType;
    int outputDequeue;
    for (int i = 0 ; i < 5 ; i++){ // Para cada pop, me fijo si elimina un elemento y que sea del tipo que corresponde según FIFO
        outputDequeue = gameBoardDequeuePiece(board, &outputDequeueType);
        assert(outputDequeue == 1 && "Pop debería devolver 1 con queue no vacía");
        assert(outputDequeueType == i && "Error orden FIFO del pop");
    }

    int popVacio = gameBoardDequeuePiece(board, &outputDequeueType);
    assert(popVacio == 0 && "Pop con queue vacía no devuelve 0");
    assert(outputDequeueType == 4 && "Pop con queue vacía no debería modificar el último tipo sacado"); // 4 fue el valor de la última pieza sacada
    assert(board -> queue_head == NULL && "Queue vacía no tiene queue_head NULL");
    assert(board -> queue_tail == NULL && "Queue vacía no tiene queue_tail NULL");

    //    Verificar tambien que un push posterior funcione.

    gameBoardEnqueuePiece(board, 3);

    assert(board -> queue_head != NULL && board -> queue_tail != NULL && board -> queue_head == board -> queue_tail && board -> queue_head -> type == 3 && "Push posterior no funciona");

    int popPosterior = gameBoardDequeuePiece(board, &outputDequeueType);
    assert(popPosterior == 1 && outputDequeueType == 3 && "Error al extraer en el push posterior");

    //  - Tablero: llenar la fila de indice 19 (la de abajo), llamar a
    //    gameBoardCheckAndClearLines y comprobar que first_row es la fila nueva vacia y que
    //    la lista sigue teniendo 20 filas.

    RowNode* vieja_primera_fila = board -> first_row;
    fillRow(board, 19);

    int cant_filas_eliminadas = gameBoardCheckAndClearLines(board);
    assert(cant_filas_eliminadas == 1 && "Error al eliminar la última fila");
    assert(board -> first_row != NULL && board -> first_row != vieja_primera_fila && isRowEmpty(board->first_row) && "first_row no es la nueva primera fila");


    int cant_filas = checkRowCount(board);
    assert(cant_filas == 20 && "Error al reemplazar filas eliminadas");

    // Llenar filas 0 y 1 y llamar gameBoardCheckAndClearLines.
    
    fillRow(board, 0);
    fillRow(board, 1);

    cant_filas_eliminadas = gameBoardCheckAndClearLines(board);
    assert(cant_filas_eliminadas == 2 && "Error al eliminar las dos primeras filas");

    cant_filas = checkRowCount(board);
    assert(cant_filas == 20 && "Error al reemplazar filas eliminadas");

    // Liberar toda la memoria pedida en los tests (strings y tableros).

    gameBoardDelete(board);
}

// Compara contra "--test" sin depender de string.h ni del Ejercicio 1.
int esFlagTest(char* arg) {
    char* flag = "--test";
    int i = 0;
    while (arg[i] != '\0' && arg[i] == flag[i]) i++;
    return arg[i] == '\0' && flag[i] == '\0';
}

int main(int argc, char* args[]) {
    srand(time(NULL));

    // Los tests corren antes de SDL. Con --test termina aca, asi valgrind solo
    // reporta la memoria del TP (SDL tiene bloques propios que no se liberan).
    correrTests();
    if (argc > 1 && esFlagTest(args[1])) return 0;

    if (!inicializar()) return 1;

    game_board = gameBoardNew();
    if (game_board == NULL) {
        printf("ADVERTENCIA: Implementa gameBoardNew() para iniciar el juego.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    for(int i=0; i<3; i++) gameBoardEnqueuePiece(game_board, rand() % 7);

    SDL_Event e;
    int game_over = 0;

    while (!game_over) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) game_over = 1;
            if (e.type == SDL_KEYDOWN && game_board->current_piece.active) {
                if (e.key.keysym.sym == SDLK_LEFT && canMove(game_board, game_board->current_piece.col - 1, game_board->current_piece.row, game_board->current_piece.rotation)) game_board->current_piece.col--;
                if (e.key.keysym.sym == SDLK_RIGHT && canMove(game_board, game_board->current_piece.col + 1, game_board->current_piece.row, game_board->current_piece.rotation)) game_board->current_piece.col++;
                if (e.key.keysym.sym == SDLK_DOWN && canMove(game_board, game_board->current_piece.col, game_board->current_piece.row + 1, game_board->current_piece.rotation)) game_board->current_piece.row++;
                if (e.key.keysym.sym == SDLK_UP) {
                    int next_rot = (game_board->current_piece.rotation + 1) % 4;
                    if (canMove(game_board, game_board->current_piece.col, game_board->current_piece.row, next_rot)) game_board->current_piece.rotation = next_rot;
                }
                if (e.key.keysym.sym == SDLK_SPACE) {
                     while(canMove(game_board, game_board->current_piece.col, game_board->current_piece.row + 1, game_board->current_piece.rotation)) game_board->current_piece.row++;
                     lockPiece(game_board);
                }
            }
        }
        if (!gameBoardUpdate(game_board)) game_over = 1;
        gameBoardDraw(game_board);
        SDL_Delay(16);
    }
    gameBoardDelete(game_board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
