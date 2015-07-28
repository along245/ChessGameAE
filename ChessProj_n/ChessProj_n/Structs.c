


#include "ChessLogic.h"
#include "Structs.h"

int compareLocs(LOC_PTR loc1, LOC_PTR loc2){
	if (((*loc1).col != (*loc2).col) || ((*loc1).row != (*loc2).row)){
		return 0;
	}
	else{
		return 1;
	}
}

int compareMoves(move_PTR move1, move_PTR move2){

	ListRef moves1;
	ListRef moves2;

	moves1 = (*move1).moves; ///list pointer to list of locs
	moves2 = (*move2).moves;

	while (moves1 != NULL && moves2 != NULL){ // compare each loc
		if (compareLocs((LOC_PTR)headData(moves1), (LOC_PTR)headData(moves2))){
			moves1 = tail(moves1);
			moves2 = tail(moves2);
		}
		else{
			return 0;
		}
	}
	if (moves1 == NULL && moves2 == NULL){
		return 1;
	}
	else{
		return 0;
	}
}

int calcPos(char i){
	return i - 'a';
}

int calcInt(char i){
	return i - '0';
}

int legalPos(int i, int j){
	if (i >= 0 && i <= 9 && j >= 0 && j <= 9 && (i + j) % 2 == 0){
		return 1;
	}
	else{
		return 0;
	}
}

char manToKing(int row, char man){
	if (man == 'K' || man == 'k'){
		return man;
	}
	else{

		if (row == 9 && man == 'm'){
			return 'k';
		}
		else if (row == 0 && man == 'M'){
			return 'K';
		}
		else{
			return man;
		}
	}
}

//copies board from to board to
void copyBoard(char from[BOARD_SIZE][BOARD_SIZE], char to[BOARD_SIZE][BOARD_SIZE]){
	int i, j;	
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			to[i][j] = from[i][j];
		}
	}
}

GameState *copyState(GameState *state){
	GameState* res;
	int i;
	int j;
	res = (GameState*)malloc(sizeof(GameState));
	if (res == NULL){
		return NULL;
	}
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			(*res).cur_board[i][j] = (*state).cur_board[i][j];
		}
	}
	(*res).minmaxDepth = (*state).minmaxDepth;
	(*res).player = (*state).player;
	(*res).black_kings = (*state).black_kings;
	(*res).black_men = (*state).black_men;
	(*res).white_kings = (*state).white_kings;
	(*res).white_men = (*state).white_men;
	return res;
}

move_PTR init_move(LOC_PTR location){
	move_PTR res;
	res = (move_PTR)malloc(sizeof(Move));
	if (res == NULL){
		return NULL;
	}
	(*res).moves = newList(location);
	return res;
}

move_PTR add_move(move_PTR list, LOC_PTR location){
	ListRef c1;
	c1 = append((*list).moves, location);
	if (c1 == NULL){
		return NULL;
	}
	return list;
}

LOC_PTR init_Location(int column, int row, char board[BOARD_SIZE][BOARD_SIZE]){
	LOC_PTR c1;
	c1 = (LOC_PTR)malloc(sizeof(Location));
	if (c1 == NULL){
		return NULL;
	}
	(*c1).row = row;
	(*c1).col = column;
	if (((*c1).col + (*c1).row) % 2 == 1){
		(*c1).color = WHITE;
	}
	else{
		(*c1).color = BLACK;
	}
	(*c1).cont = board[column][row];

	return c1;
}

mmr_PTR init_MiniMaxResult(int score, int index){
	mmr_PTR c1;
	c1 = (mmr_PTR)malloc(sizeof(mmr));
	if (c1 == NULL){
		return NULL;
	}
	(*c1).value = score;
	(*c1).index = index;
	return c1;
}

void clear_army(ARMY_PTR army){
	army->BP = 0;
	army->BK = 0;
	army->BQ = 0;
	army->BN = 0;
	army->BR = 0;
	army->BB = 0;
	army->WP = 0;
	army->WK = 0;
	army->WQ = 0;
	army->WN = 0;
	army->WR = 0;
	army->WB = 0;
}

ARMY_PTR init_army(){
	ARMY_PTR army = (ARMY_PTR)malloc(sizeof(ARMY));
	if (army == NULL){
		return NULL;
	}
	army->BP = 8;
	army->BK = 1;
	army->BQ = 1;
	army->BN = 2;
	army->BR = 2;
	army->BB = 2;
	army->WP = 8;
	army->WK = 1;
	army->WQ = 1;
	army->WN = 2;
	army->WR = 2;
	army->WB = 2;
}

void init_EmptyState(GameState* emptyState){
	emptyState = (GameState*)malloc(sizeof(GameState));
	if (emptyState == NULL){
		return NULL;
	}
}



GameState *makeLegalMove(GameState *curstate, move_PTR curmove){
	int toRow=curmove->tRow;
	int toCol=curmove->tCol;
	int curRow=curmove->sRow;
	int curCol=curmove->sCol;
	char curUnit;
	GameState *res;		
	res = copyState(curstate);
	res->player = curstate->player*(-1);
	res->cur_board[curRow][curCol] = EMPTY;
	if (curmove->isCastling){
		res->cur_board[curRow][curCol] = EMPTY;
		res->cur_board[toRow][toCol] = EMPTY;
		if (curRow == 0){//WHITE CASTLEING
			if (curmove->isCastling == 1){//SMALL			
				res->cur_board[curRow][curCol + 2] = 'k';
				res->cur_board[toRow][toCol - 2] = 'r';
			}
			else{//BIG
				res->cur_board[curRow][curCol - 2] = 'k';
				res->cur_board[toRow][toCol + 3] = 'r';
			}
		}
		else if (curRow==7){ // BLACK CASTLEING
			if (curmove->isCastling == 1){//SMALL			
				res->cur_board[curRow][curCol + 2] = 'K';
				res->cur_board[toRow][toCol - 2] = 'R';
			}
			else{//BIG
				res->cur_board[curRow][curCol - 2] = 'K';
				res->cur_board[toRow][toCol + 3] = 'R';
			}
		}
	}//END OF CASTELING
	else if (curmove->isPromoting){
		res->cur_board[curRow][curCol] = EMPTY;
		switch (curmove->promoteTo){
		case 'q':
			res->cur_board[toRow][toCol] = 'q';
			break;
		case 'b':
			res->cur_board[toRow][toCol] = 'b';
			break;
		case 'r':
			res->cur_board[toRow][toCol] = 'r';
			break;
		case 'n':
			res->cur_board[toRow][toCol] = 'n';
			break;
		case 'p':
			res->cur_board[toRow][toCol] = 'p';
			break;
		case 'Q':
			res->cur_board[toRow][toCol] = 'Q';
			break;
		case 'B':
			res->cur_board[toRow][toCol] = 'B';
			break;
		case 'R':
			res->cur_board[toRow][toCol] = 'R';
			break;
		case 'N':
			res->cur_board[toRow][toCol] = 'N';
			break;
		case 'P':
			res->cur_board[toRow][toCol] = 'P';
			break;
		default:
			break;
		}
	}
	else{
		curUnit = curstate->cur_board[curRow][curCol];
		res->cur_board[curRow][curCol] = EMPTY;
		res->cur_board[toRow][toCol] = curUnit;
	}

	
}


GameState* start(char board[BOARD_SIZE][BOARD_SIZE], int depth, int next_p, int mode, int user_col){
	GameState* state0;
	state0 = (GameState*)malloc(sizeof(GameState));
	if (state0 == NULL){
		return NULL;
	}
	copyBoard(board, state0->cur_board);
	(*state0).difficulty = depth; //1-4 for reg, 5 for best
	(*state0).player = next_p;
	if (mode == 2){//PVM
		state0->userColor = user_col;
	}
	else{//PVP
		state0->userColor = 0;
	}
	state0->gameMode = mode;		
	return state0;
}


void clear(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	i = 0;
	j = 0;
	while (i < BOARD_SIZE){
		j = 0;
		while (j < BOARD_SIZE){
			*(*(board + i) + j) = EMPTY;
			j++;
		}
		i++;
	}
}

int set(LOC_PTR loc, char a, char b, char board[BOARD_SIZE][BOARD_SIZE], char cpy[BOARD_SIZE][BOARD_SIZE]){
	if (loc->row >= 8 || loc->col >= 8){
		print_message(WRONG_POSITION);
	}
	else{
		ARMY_PTR check;
		int row = loc->row;
		int col = loc->col;
		int res;
		putInBoard(cpy, row, col, a, b);
		check = boardToArmy(cpy);
		if (check == NULL){
			return -1;
		}
		res = ifArmyOK(check);
		free(check);
		if (res == 1){
			putInBoard(board, row, col, a, b);
		}
		else{
			printf(NO_PIECE);
		}
	}
	return 0;
}


		
	


void rm(LOC_PTR loc, char board[BOARD_SIZE][BOARD_SIZE]){
	int row = loc->row;
	int col = loc->col;
	if (row >= 8 || col >= 8){
		print_message(WRONG_POSITION);
	}
	else{
		board[row][col] = EMPTY;
	}
}

void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

void init_board(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (i == 1){
				board[i][j] = WHITE_P;
			}
			else if (i == 6){
				board[i][j] = BLACK_P;
			}
			else if (i == 0){
				if (j == 0 || j == 7){
					board[i][j] =  WHITE_R;
				}
				else if (j == 1 || j == 6){
					board[i][j] = WHITE_N;
				}
				else if (j == 2 || j == 5){
					board[i][j] = WHITE_B;
				}
				else if (j == 3){
					board[i][j] = WHITE_Q;
				}
				else if (j == 4){
					board[i][j] = WHITE_K;
				}
			}
			else if (i == 7){
				if (j == 0 || j == 7){
					board[i][j] = BLACK_R;
				}
				else if (j == 1 || j == 6){
					board[i][j] = BLACK_N;
				}
				else if (j == 2 || j == 5){
					board[i][j] = BLACK_B;
				}
				else if (j == 3){
					board[i][j] = BLACK_Q;
				}
				else if (j == 4){
					board[i][j] = BLACK_K;
				}

			}
			

		}
	}
}

int evaluate(void* state){
	int i, j, score = 0;	
	score = 0;	
	return score;
}

int ifStartOK(GameState* startstate){
	int res = 1;
	
	if (startstate->B_K+startstate->W_K !=2){
		res = 0;
	}
	
	return res;
}

int ifArmyOK(ARMY_PTR army){
	int res;
	
	if (army->BR > 2 || army->BB >2 || army->BN >2 || army->WR >2 || army->WB>2 || army->WN>2){
		res = 0;
	}
	if (army->BP>8 || army->WP>8){
		res = 0;
	}
	return res;

}

ARMY_PTR boardToArmy(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	ARMY_PTR army = (ARMY_PTR)malloc(sizeof(ARMY));
	if (army == NULL){
		return NULL;
	}
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++){
			switch (board[i][j]){
			case 'p':
				army->WP++;
				break;
			case 'k':
				army->WK++;
				break;
			case 'q':
				army->WQ++;
				break;
			case 'n':
				army->WN++;
				break;
			case 'b':
				army->WB++;
				break;
			case 'r':
				army->WR++;
				break;
			case 'P':
				army->BP++;
				break;
			case 'K':
				army->BK++;
				break;
			case 'Q':
				army->BQ++;
				break;
			case 'N':
				army->BN++;
				break;
			case 'B':
				army->BB++;
				break;
			case 'R':
				army->BR++;
				break;
			default:
				break;
			}
		}
	}
	return army;
}

void putInBoard(char cpy[BOARD_SIZE][BOARD_SIZE], int col, int row, char a, char b){
	if (a == 'b'){
		switch (b){
		case 'p':
			cpy[row][col] = 'P';
			break;
		case 'k':
			cpy[row][col] = 'K';
			break;
		case 'q':
			cpy[row][col] = 'Q';
			break;
		case 'n':
			cpy[row][col] = 'N';
			break;
		case 'b':
			cpy[row][col] = 'B';
			break;
		case 'r':
			cpy[row][col] = 'R';
			break;
		default:
			break;
		}
	}
	else if (a == 'w'){
		switch (b){
		case 'p':
			cpy[row][col] = 'p';
			break;
		case 'k':
			cpy[row][col] = 'k';
			break;
		case 'q':
			cpy[row][col] = 'q';
			break;
		case 'n':
			cpy[row][col] = 'n';
			break;
		case 'b':
			cpy[row][col] = 'b';
			break;
		case 'r':
			cpy[row][col] = 'r';
			break;
		default:
			break;
		}
	}
}




