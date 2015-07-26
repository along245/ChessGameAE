#ifndef DRAUGHTS_
#define DRAUGHTS_

#include <stdio.h>


#define W_KING 'k'
#define W_ROOK 'r'
#define W_PAWN 'p'
#define W_BISHOP 'b'
#define W_QUEEN 'q'
#define W_KNIGHT 'n'

#define B_KING 'K'
#define B_ROOK 'R'
#define B_PAWN 'P'
#define B_BISHOP 'B'
#define B_QUEEN 'Q'
#define B_KNIGHT 'N'

#define WHITE 1
#define BLACK -1
#define EMPTY ' '
#define WHITE_T WHITE//white turn
#define BLACK_T BLACK//black turn
#define WHITE_COL 'w'
#define BLACK_COL 'b'





#define BOARD_SIZE 10

typedef char** board_t;
#define WELCOME_TO_DRAUGHTS "Welcome to Draughts!\n"
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 6\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_DICS "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ENTER_YOUR_MOVE "Enter your move:\n" 
#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message));



void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void clear(char board[BOARD_SIZE][BOARD_SIZE]);
int calcPos(char i); // given a char representing a column, return its required int value in the matrice
int evaluate(void* state);
#endif  
