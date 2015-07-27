
#ifndef STRUCTS_
#define STRUCTS_
#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<string.h>
#include "ListUtils.h"
#include "ChessLogic.h"
#define TOLOWER(cont)((cont==77 || cont==75)?(cont+32):(cont))
#define DEBUG
#define COLOR_STT(cont)((cont==107 || cont == 109)?(WHITE_COL):(BLACK_COL))




// Location Struct
typedef struct Location{
	int row;
	int col;
} Location, *LOC_PTR;


// Move, which contains a list of locations
typedef struct Move{
	struct List *moves;
} Move, *move_PTR;



//Gamestate structs, contains the current board, player , difficulty , and number of discs
typedef struct _GameState{
	char cur_board[BOARD_SIZE][BOARD_SIZE];
	int player;//whos turn
	int score;
	int gameMode;
	int difficulty;
	int userColor;
	int minmaxDepth;
} GameState;

//Initializes a gamestate
GameState* start(char board[BOARD_SIZE][BOARD_SIZE], int depth);

/**
* The maximum allowed value to be returned by any evaluation function.
*/
#define MAX_EVALUATION 100

/**
* The minimum allowed value to be returned by any evaluation function.
*/
#define MIN_EVALUATION -100
/**
* Represents the result of a MiniMax computation.
*/
typedef struct MiniMaxResult {
	/**
	* The index of the best child.
	*/
	int index;

	/**
	* The value of the best child (according to the evaluation function).
	*/
	int value;
}mmr, *mmr_PTR;



/**
given a gamestate, return its optional moves
as a list of moves
**/

ListRef getChildren(void *curBoard);

/**
given a move, prints it in the required format
**/

void printMove(move_PTR list);

/**
given a list of moves, prints it in the required format
**/

void printAllMoves(ListRef list);

//Initializes a location

LOC_PTR init_Location(int column, int row, char board[BOARD_SIZE][BOARD_SIZE]);

//Initializes a minimax result

mmr_PTR init_MiniMaxResult(int score, int index);

/**
* Returns the best child of the given state, resulting from the MiniMax computation up to maxDepth.
* The getChildren function will be used to get each state's children.
* The evaluate function will be used to evaluate the state score.
* IsMaxPlayer is 1 for White, -1 for Black


*/

mmr_PTR getBestChild(void* state,
	unsigned int maxDepth,
	ListRef(*getChildren) (void* state),
	FreeFunc freeMoves,
	FreeFunc freeMMRs,
	FreeFunc freeState,
	FreeFunc freeMMR,
	int(*evaluate) (void* state),
	int isMaxPlayer);//

//removes the disc located in loc

void rm(struct Location loc, char board[BOARD_SIZE][BOARD_SIZE]);

//sets the given disc in the board

void set(struct Location loc, char a, char b, char board[BOARD_SIZE][BOARD_SIZE]);

//copies a given board to an empty one 

void copyBoard(char from[BOARD_SIZE][BOARD_SIZE], char to[BOARD_SIZE][BOARD_SIZE]);

//creates a copy of a given list
ListRef makeCopy(ListRef list);
ListRef generateMoveBoards(char curBoard[BOARD_SIZE][BOARD_SIZE], int row, int col, int kingFlag, ListRef moves);
int checkLocation(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int cur_color, int diag, ListRef *eatLocs, ListRef *moveLocs, char cont);
ListRef *getLocationsLists(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int kingFlag);
GameState *makeLegalMove(GameState *curstate, move_PTR curmove);
/**free functions
Each function matches a certain type of struct
according to our implementation
**/


void freeState(GameState* c1);

void freeMMR(mmr_PTR c1);

void freeEmpty(ListRef c1);

void freeLoc(LOC_PTR c1);

void freeMove(move_PTR c1);

void freeStateList(ListRef state_list);

void freeMoveList(ListRef move_list);

void freeMMRs(ListRef mmrs_list);

void freeLocList(ListRef loc_list);








#endif /* STRUCTS_ */
