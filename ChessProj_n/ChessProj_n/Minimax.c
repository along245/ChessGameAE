#include "Structs.h"
#include <math.h>

#define DEBUG
#define COLOR_STT(cont)((cont==107 || cont == 109)?(WHITE_COL):(BLACK_COL))
#define TOLOWER(cont)((cont==77 || cont==75)?(cont+32):(cont))
#define getCol(num)('a'+num)
int global = 0;

/**TODO:
1.figure out how to add moves without kille to final_boards
2.figure out how to fix problems with the boards printed in main
3.add move lists in the recursion
4.check everything.

(added the man-to-king switch, added the flag king eats once as king).
**/

//returns the color of a disk
int COLOR(char cont){
	if (cont == 107 || cont == 109)
		return WHITE;
	if (cont == 77 || cont == 75)
		return BLACK;
	else
		return 999;
}

//coppies board from to board to
void copyBoard(char from[BOARD_SIZE][BOARD_SIZE], char to[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	i = 1;
	//print_board(from);

	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++){
			to[i][j] = from[i][j];
		}
}
//recieves a location source and returns a new copy location res
LOC_PTR copyLocation(LOC_PTR source){
	LOC_PTR res;
	res = (LOC_PTR)malloc(sizeof(Location));
	if (res == NULL){
		return NULL;
	}
	res->col = source->col;
	res->row = source->row;
	res->cont = source->cont;
	res->color = source->color;
	return res;
}
//recieves a move sorce and creates a copy.
move_PTR copyMove(move_PTR source){
	move_PTR res = (move_PTR)malloc(sizeof(Move));
	if (res == NULL)
		return NULL;
	ListRef movesCpy;
	LOC_PTR tmp;
	ListRef appendres;
	res->moves = newList(NULL);
	if (res->moves == NULL){
		freeMove(res);
		return NULL;
	}
	movesCpy = source->moves;
	while (movesCpy != NULL){
		tmp = copyLocation(headData(movesCpy));
		if (tmp == NULL){
			freeLocList(res->moves);
			freeMove(res);
			return NULL;
		}
		appendres = append(res->moves, tmp);
		if (appendres == NULL){
			freeLocList(res->moves);
			freeMove(res);
			return NULL;
		}
		movesCpy = tail(movesCpy);
	}
	return res;

}

//recieves a list of movePtr's and returns a list same as the input list
ListRef makeCopy(ListRef list){
	ListRef cpy = newList(NULL);
	if (cpy == NULL){
		return NULL;
	}

	move_PTR ptr;
	ListRef appendres;
	ListRef work = list;
	while (work != NULL){
		ptr = copyMove((move_PTR)headData(work));
		if (ptr == NULL){
			freeMoveList(cpy);
			return NULL;
		}
		appendres = append(cpy, ptr);
		if (appendres == NULL){
			freeMove(ptr);
			freeMoveList(cpy);
			return NULL;
		}
		work = tail(work);
	}
	return cpy;
}
//prints a single move
void printMove(move_PTR list){
	ListRef cpy2 = list->moves;
	printf("<%c,%d> to ", getCol(((LOC_PTR)headData(cpy2))->col), (((LOC_PTR)headData(cpy2))->row + 1));
	cpy2 = tail(cpy2);
	while (cpy2 != NULL){
		printf("<%c,%d>", getCol(((LOC_PTR)headData(cpy2))->col), (((LOC_PTR)headData(cpy2))->row + 1));
		cpy2 = tail(cpy2);
	}

}
//prints a list of moves
void printAllMoves(ListRef list){
	ListRef cpy = list;
	while (cpy != NULL){
		printMove(((move_PTR)headData(cpy)));
		printf("\n");
		cpy = tail(cpy);
	}
}
//returns all possible moves for current player
ListRef getChildren(void *curBoard){
	ListRef moves, result, temp1, temp2, temp3, killsList;
	char currentBoard[BOARD_SIZE][BOARD_SIZE];//the board on wich we make moves
	char cpyBoard[BOARD_SIZE][BOARD_SIZE];//the work board - where we check for each disk its moves
	int i, j, player, maxKills, sizeList;
	move_PTR m, tmpMove;
	LOC_PTR l1, l2;
	ListRef appendres;
	copyBoard(((GameState*)curBoard)->cur_board, currentBoard);
	copyBoard(currentBoard, cpyBoard);
	player = ((GameState*)curBoard)->player;
	i = 0;
	j = 0;
	maxKills = 0;
	result = newList(NULL);
	if (result == NULL){
		return NULL;
	}
	killsList = newList(NULL);
	if (killsList == NULL){
		freeMoveList(result);//FREEMOVE
		return NULL;
	}
	//iterating through the board, checking who is the player
	//checking if the disk is of the players color and if so, making
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (COLOR(cpyBoard[j][i]) == player){
				m = (move_PTR)malloc(sizeof(Move));
				if (m == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					return NULL;
				}
				maxKills = 0;
				m->moves = newList(NULL);
				if (m->moves == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					freeMove(m);
					return NULL;
				}
				appendres = append(m->moves, init_Location(j, i, currentBoard));
				if (appendres == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					freeLocList(m->moves);
					freeMove(m);
					return NULL;
				}
				moves = newList(m);
				if (moves == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					freeLocList(m->moves);
					freeMove(m);
					return NULL;
				}
				if (i == 4){}
				temp1 = generateMoveBoards(currentBoard, i, j, 0, moves);
				if (temp1 == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					freeMoveList(moves);
					return NULL;
				}

				if (!isEmpty(temp1) && size(((move_PTR)headData(temp1))->moves) > 1){
					if (isEmpty(result)){
						freeEmpty(result);
						result = temp1;
					}
					else
						concat(result, temp1);
					freeMoveList(moves);
				}
				else{
					freeMoveList(temp1);
				}

			}
		}
	}
	temp2 = result;
	while (temp2 != NULL && !isEmpty(temp2)){
		if ((sizeList = size(((move_PTR)headData(temp2))->moves)) >= maxKills){
			maxKills = sizeList;
		}
		temp2 = tail(temp2);
	}
	temp3 = result;
	while (temp3 != NULL && !isEmpty(temp3) && maxKills > 1){
		if (size(((move_PTR)headData(temp3))->moves) == maxKills){
			int wasEat = 0;
			if (maxKills == 2){//then we only have regular moves or single kill moves
				l1 = (LOC_PTR)headData(((move_PTR)headData(temp3))->moves);//first loce
				l2 = (LOC_PTR)headData(tail(((move_PTR)headData(temp3))->moves));//second loc
				if ((l2->row > l1->row && l2->col > l1->col) && (cpyBoard[l2->col - 1][l2->row - 1] != EMPTY)){// up right eat	
					if (COLOR(cpyBoard[l2->col - 1][l2->row - 1]) != player)
						wasEat = 1;
				}
				if ((l2->row<l1->row && l2->col > l1->col) && (cpyBoard[l2->col - 1][l2->row + 1] != EMPTY)){//down right eat	
					if (COLOR(cpyBoard[l2->col - 1][l2->row + 1]) != player)
						wasEat = 1;

				}
				if ((l2->row < l1->row && l2->col < l1->col) && (cpyBoard[l2->col + 1][l2->row + 1] != EMPTY)){// down left eat	
					if (COLOR(cpyBoard[l2->col + 1][l2->row + 1]) != player)
						wasEat = 1;

				}
				if ((l2->row>l1->row && l2->col < l1->col) && (cpyBoard[l2->col + 1][l2->row - 1] != EMPTY)){ // up left eat
					if (COLOR(cpyBoard[l2->col + 1][l2->row - 1]) != player)
						wasEat = 1;
				}
				if (wasEat){
					tmpMove = copyMove(headData(temp3));
					if (tmpMove == NULL){
						freeMoveList(killsList);
						freeMoveList(result);
						return NULL;
					}
					appendres = append(killsList, tmpMove);
					if (appendres == NULL){
						freeMoveList(killsList);
						freeMoveList(result);
						freeMove(tmpMove);
						return NULL;
					}
				}
			}

			else{
				tmpMove = copyMove(headData(temp3));
				if (tmpMove == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					return NULL;
				}
				appendres = append(killsList, tmpMove);
				if (appendres == NULL){
					freeMoveList(killsList);
					freeMoveList(result);
					freeMove(tmpMove);
					return NULL;
				}
			}
		}
		temp3 = tail(temp3);

	}
	if (size(killsList) > 0){
		freeMoveList(result);//FREEMOVE

		return killsList;
	}

	else{
		freeMoveList(killsList);//FREEMOVE

		return result;
	}
}



//recursively making boards after eating for a single disk
ListRef generateMoveBoards(char curBoard[BOARD_SIZE][BOARD_SIZE], int row, int col, int kingFlag, ListRef moves){
	int num_of_kills;//number of kills that can be made from current position - the "zero kill".
	Location *diskLoc;//the location of the current disk were working on
	Location *loc;//location of current disk after kill
	Location *eatingLoc;//location of disk we eat
	ListRef *locations;//holds a list of possible moves (index 0 no eat, index 1 eating moves)
	ListRef cpy, movesNoEats, result, appendres, recursiveRes;
	char board[BOARD_SIZE][BOARD_SIZE], cont;
	result = newList(NULL);
	diskLoc = init_Location(col, row, curBoard);
	if (diskLoc == NULL){
		free(result);
		return NULL;
	}
	num_of_kills = 0;
	locations = getLocationsLists(curBoard, row, col, kingFlag);
	if (locations == NULL){
		free(result);
		freeLoc(diskLoc);
		return NULL;
	}
	movesNoEats = locations[0];
	cpy = locations[1];//a copy of the possible eating loc list						
	num_of_kills = size(cpy);//the number of possible eats
	if (num_of_kills == 0)//that means no killing moves so we return regular moves
	{
		if (size(movesNoEats) > 0){
			if (size(((move_PTR)headData(moves))->moves) == 1){//that means we havent made any eats.
				ListRef movescpy;
				while (movesNoEats != NULL){
					movescpy = makeCopy(moves);
					if (movescpy == NULL){
						freeLoc(diskLoc);
						freeLocList(locations[1]);
						freeLocList(locations[0]);
						free(locations);
						freeMoveList(result);
						return NULL;
					}
					appendres = append(((move_PTR)headData(movescpy))->moves, copyLocation(headData(movesNoEats)));
					if (appendres == NULL){
						freeLoc(diskLoc);
						freeLocList(locations[1]);
						freeLocList(locations[0]);
						free(locations);
						freeMoveList(result);
						freeMoveList(movescpy);
						return NULL;
					}
					movesNoEats = tail(movesNoEats);
					if (isEmpty(result)){
						freeEmpty(result);
						result = movescpy;

					}
					else
						concat(result, movescpy);
				}
			}
			else{
				if (isEmpty(result)){
					freeEmpty(result);
					result = moves;
				}
				else
					concat(result, moves);

			}
			freeLoc(diskLoc);
			freeLocList(locations[0]);
			freeLocList(locations[1]);
			free(locations);
			return result;
		}

		else{
			freeLoc(diskLoc);
			freeLocList(locations[1]);
			freeLocList(locations[0]);
			free(locations);
			freeMoveList(result);
			return moves;
		}

	}
	while (cpy != NULL && !isEmpty(cpy)){
		ListRef cpyMoves = makeCopy(moves);
		if (cpyMoves == NULL){
			freeLoc(diskLoc);
			freeLocList(locations[1]);
			freeLocList(locations[0]);
			free(locations);
			freeMoveList(result);
			return NULL;
		}
		eatingLoc = headData(cpy);//getting the to_kill loc
		cont = diskLoc->cont;
		copyBoard(curBoard, board);//making a copy of the board so we wont change the original
		if (TOLOWER(cont) == 'k')//that means we ate once and can only eat like men
			kingFlag = 1;
		rm(*eatingLoc, board);//removes current disk
		rm(*diskLoc, board);//removes the eaten disk
		cpy = tail(cpy);
		//sets the current disk at its right location after the kill
		if (diskLoc->row < eatingLoc->row && diskLoc->col < eatingLoc->col)//diag1 upright
		{
			loc = init_Location(eatingLoc->col + 1, eatingLoc->row + 1, board);
			if (loc == NULL){
				freeLoc(diskLoc);
				freeLocList(locations[1]);
				freeLocList(locations[0]);
				freeMoveList(cpyMoves);
				free(locations);
				freeMoveList(result);
				return NULL;
			}
			set(*loc, COLOR_STT(cont), TOLOWER(cont), board);
		}
		else if (diskLoc->row < eatingLoc->row && diskLoc->col >eatingLoc->col)//diag2 upleft
		{
			loc = init_Location(eatingLoc->col - 1, eatingLoc->row + 1, board);
			if (loc == NULL){
				freeLoc(diskLoc);
				freeLocList(locations[1]);
				freeLocList(locations[0]);
				freeMoveList(cpyMoves);
				free(locations);
				freeMoveList(result);
				return NULL;
			}
			set(*loc, COLOR_STT(cont), TOLOWER(cont), board);
		}
		else if (diskLoc->row > eatingLoc->row && diskLoc->col < eatingLoc->col)//diag3 downright
		{
			loc = init_Location(eatingLoc->col + 1, eatingLoc->row - 1, board);
			if (loc == NULL){
				freeLoc(diskLoc);
				freeLocList(locations[1]);
				freeLocList(locations[0]);
				freeMoveList(cpyMoves);
				free(locations);
				freeMoveList(result);
				return NULL;
			}
			set(*loc, COLOR_STT(cont), TOLOWER(cont), board);
		}
		else//diag4 downleft
		{
			loc = init_Location(eatingLoc->col - 1, eatingLoc->row - 1, board);
			if (loc == NULL){
				freeLoc(diskLoc);
				freeLocList(locations[1]);
				freeLocList(locations[0]);
				freeMoveList(cpyMoves);
				free(locations);
				freeMoveList(result);
				return NULL;
			}
			set(*loc, COLOR_STT(cont), TOLOWER(cont), board);
		}
		appendres = append(((move_PTR)headData(cpyMoves))->moves, copyLocation(loc));
		if (appendres == NULL){
			freeLoc(loc);
			freeLoc(diskLoc);
			freeLocList(locations[1]);
			freeLocList(locations[0]);
			freeMoveList(cpyMoves);
			free(locations);
			freeMoveList(result);
			return NULL;
		}
		//if a man reached the top of the board we make him a king and stop the sequence.
		if ((cont == 'M' && loc->row == 0) || (cont == 'm' && loc->row == BOARD_SIZE - 1)){
			if (isEmpty(result)){
				freeEmpty(result);
				result = cpyMoves;
			}
			else
				concat(result, cpyMoves);
		}
		else{
			recursiveRes = generateMoveBoards(board, loc->row, loc->col, kingFlag, cpyMoves);
			if (recursiveRes == NULL){
				freeLoc(loc);
				freeLoc(diskLoc);
				freeLocList(locations[1]);
				freeLocList(locations[0]);
				freeMoveList(cpyMoves);
				freeMoveList(result);
			}
			if (isEmpty(result)){
				freeEmpty(result);
				result = recursiveRes;
			}
			else
				concat(result, recursiveRes);
		}
		//freeMoveList(cpyMoves);
		freeLoc(loc);
	}
	freeLoc(diskLoc);
	freeLocList(locations[1]);
	freeLocList(locations[0]);
	free(locations);
	return result;
}


////////////////CHECKLOCATION/////////////////////
/**returns 1 if able to eat this loc
returns 0 if location is occupied by same color
returns 2 if can only move without eating
**/
int checkLocation(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int cur_color, int diag, ListRef *eatLocs, ListRef *moveLocs, char cont){
	//while we have empty spaces on the diagonal we can move to that place.
	int flag1, flag2;
	LOC_PTR location;
	ListRef appendres;
	flag2 = 0;
	flag1 = 0;
	if (board[col][row] == EMPTY){
		if (cont == 'm')
			flag1 = ((diag == 1) || (diag == 2));//we can only move a white man without eating upwards
		else if (cont == 'M')
			flag1 = ((diag == 3) || (diag == 4));//we can only move a black man without eating downwards
		else
			flag1 = 1;//a king can move on all diags
		if (flag1){//if the disk is able to move to that location
			location = init_Location(col, row, board);
			appendres = append(*moveLocs, location);
			if (location == NULL || appendres == NULL)
				return 999;
			return 2;
		}
		else{
			return 0;
		}
	}
	else{
		//if we find a disk on the diagonal, we cant move further.
		if (COLOR(board[col][row]) != cur_color){//the disk is of opponent's color
			switch (diag){//checking if the next spot on the diagonal is empty and within board.
			case 1:
				if (col + 1 < BOARD_SIZE&&row + 1 < BOARD_SIZE&&board[col + 1][row + 1] == EMPTY)
					flag2 = 1;
				break;
			case 2:
				if (col - 1 >= 0 && row + 1 < BOARD_SIZE&&board[col - 1][row + 1] == EMPTY)
					flag2 = 1;
				break;
			case 3:
				if (col + 1 < BOARD_SIZE && row - 1 >= 0 && board[col + 1][row - 1] == EMPTY)
					flag2 = 1;
				break;
			case 4:
				if (col - 1 >= 0 && row - 1 >= 0 && board[col - 1][row - 1] == EMPTY)
					flag2 = 1;
				break;
			default:
				break;
			}
			if (flag2){
				location = init_Location(col, row, board);
				appendres = append(*eatLocs, location);
				if (location == NULL || appendres == NULL)
					return 999;
				return 1;
			}
			else{//we cant eat the disk, because next spot is out of range
				return 0;
			}
		}
		else
			return 0;//there is a disk of our color
	}
}
//////////////////////////////////////////
/////////GETMOVELISTS////////////////////
/**gets a board and a location of current disk. the function then devides into two cases
if the disk is king or man
the function then uses "checkLocation" to check if each location in each diagonal is a possible
eat/move. the function returns an array of size 2. index 0 is locations with no eats
index 1 is locations with eats.
*/
ListRef *getLocationsLists(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int kingFlag){
	char cont;
	ListRef to_eat;
	ListRef no_eat_moves;
	ListRef *totalMoves;
	totalMoves = (ListRef *)malloc(2 * sizeof(ListRef));
	if (totalMoves == NULL)
		return NULL;
	no_eat_moves = newList(NULL);
	to_eat = newList(NULL);
	if (no_eat_moves == NULL){
		if (to_eat != NULL){
			free(to_eat);
		}
		freeLocList(totalMoves[0]);
		freeLocList(totalMoves[1]);
		return NULL;
	}
	int i, j, cur_col, diag1, diag2, diag3, diag4, locAnswer, freeFlag;
	cont = board[col][row];
	cur_col = COLOR(cont);
	freeFlag = 0;
	diag1 = 1;//up right
	diag2 = 1;//up left
	diag3 = 1;//down right
	diag4 = 1;//down left
	if (kingFlag)
		cont = 'm';
	switch (TOLOWER(cont)){
	case 'k':
		i = 1;
		j = 1;
		while (diag1 || diag2 || diag3 || diag4){
			if (row + i == BOARD_SIZE || col + j == BOARD_SIZE)
				diag1 = 0;
			if (row + i == BOARD_SIZE || col - j < 0)
				diag2 = 0;
			if (row - i < 0 || col + j == BOARD_SIZE)
				diag3 = 0;
			if (row - i < 0 || col - i < 0)
				diag4 = 0;
			if (diag1){//up right
				//while we have empty spaces on the diagonal we can move to that place.
				locAnswer = checkLocation(board, row + i, col + j, cur_col, 1, &to_eat, &no_eat_moves, cont);
				if (locAnswer == 999)
					freeFlag = 1;
				if (locAnswer == 1 || locAnswer == 0)
					diag1 = 0;
			}

			if (diag2){//up left
				locAnswer = checkLocation(board, row + i, col - j, cur_col, 2, &to_eat, &no_eat_moves, cont);
				if (locAnswer == 1 || locAnswer == 0)
					diag2 = 0;
				if (locAnswer == 999)
					freeFlag = 1;
			}
			if (diag3){//down right
				locAnswer = checkLocation(board, row - i, col + j, cur_col, 3, &to_eat, &no_eat_moves, cont);
				if (locAnswer == 1 || locAnswer == 0)
					diag3 = 0;
				if (locAnswer == 999)
					freeFlag = 1;
			}
			if (diag4)//down left
			{
				locAnswer = checkLocation(board, row - i, col - j, cur_col, 4, &to_eat, &no_eat_moves, cont);
				if (locAnswer == 1 || locAnswer == 0)
					diag4 = 0;
				if (locAnswer == 999)
					freeFlag = 1;
			}
			i++;
			j++;
		}
		break;
	case 'm':
		i = 1;
		j = 1;
		if (row + i == BOARD_SIZE || col + j == BOARD_SIZE)
			diag1 = 0;
		if (row + i == BOARD_SIZE || col - j < 0)
			diag2 = 0;
		if (row - i < 0 || col + j == BOARD_SIZE)
			diag3 = 0;
		if (row - i < 0 || col - i < 0)
			diag4 = 0;
		if (diag1){//up right
			locAnswer = checkLocation(board, row + i, col + j, cur_col, 1, &to_eat, &no_eat_moves, cont);
			if (locAnswer == 999)
				freeFlag = 1;
		}

		if (diag2){//up left
			locAnswer = checkLocation(board, row + i, col - j, cur_col, 2, &to_eat, &no_eat_moves, cont);
			if (locAnswer == 999)
				freeFlag = 1;
		}
		if (diag3){//down right
			locAnswer = checkLocation(board, row - i, col + j, cur_col, 3, &to_eat, &no_eat_moves, cont);
			if (locAnswer == 999)
				freeFlag = 1;
		}
		if (diag4)//down left
		{
			locAnswer = checkLocation(board, row - i, col - j, cur_col, 4, &to_eat, &no_eat_moves, cont);
			if (locAnswer == 999)
				freeFlag = 1;
		}
		break;
	default:
		break;
	}
	if (freeFlag){
		freeLocList(totalMoves[0]);
		freeLocList(totalMoves[1]);
		freeLocList(to_eat);
		freeLocList(no_eat_moves);
		return NULL;

	}
	totalMoves[0] = no_eat_moves;
	totalMoves[1] = to_eat;
	return totalMoves;
}


///////////////////////////////////////// getBestChildren




mmr_PTR getBestChild(void* state,
	unsigned int maxDepth,
	ListRef(*getChildren) (void* state),
	FreeFunc freeMoves,
	FreeFunc freeMMRs,
	FreeFunc freeState,
	FreeFunc freeMMR,
	int(*evaluate) (void* state),
	int isMaxPlayer)
{
	//in case we haven't reached the leaves yet
	if (maxDepth > 0){
		ListRef mmrs;
		int cnt;
		int maxind;
		int maxscore;
		mmr_PTR cpy;
		ListRef children;
		ListRef childrencopy;
		ListRef appendres;
		GameState *statecpy;
		mmr_PTR res;
		ListRef mmrscopy;
		cnt = 0;
		children = getChildren(state);//list of moves from current State
		if (children == NULL){
			return NULL;
		}
		childrencopy = children;
		//evaluating the nodes recursivly, results into mmrs
		mmrs = newList(NULL); // initialize an mmr
		if (mmrs == NULL){
			(*freeMoves)(children);//freeCheck
			return NULL;
		}
		if (!isEmpty(childrencopy)){ // THERE ARE MOVES
			while (childrencopy != NULL){ // we go over the moves, and evaluate each gamestate applied by it into a simple MMR

				statecpy = makeLegalMove((GameState*)state, (move_PTR)headData(childrencopy));

				if (statecpy == NULL){
					(*freeMoves)(children);//FREEMOVE
					(*freeMMRs)(mmrs);
					return NULL;
				}

				cpy = getBestChild(statecpy, maxDepth - 1, getChildren, freeMoves, freeMMRs, freeState, freeMMR, evaluate, isMaxPlayer*(-1));

				if (cpy == NULL){
					(*freeMoves)(children);//freeCheck
					(*freeMMRs)(mmrs);//freeCheck
					(*freeState)(statecpy);//freeCheck
					return NULL;
				}


				appendres = append(mmrs, cpy);

				if (appendres == NULL){
					(*freeMoves)(children);//FREEMOVE
					(*freeMMRs)(mmrs);
					(*freeState)(statecpy);
					(*freeMMR)(cpy);
					return NULL;
				}

				(*freeState)(statecpy);//freeCheck


				childrencopy = tail(childrencopy);

				cnt++;
			}
		}
		(*freeMoves)(children);//freeCheck
		mmrscopy = mmrs;
		res = init_MiniMaxResult(0, 0); // we want to find the optimal child for us, initialize an mmr
		if (res == NULL){
			//(*freeMoves)(children);//FREEMOVE
			(*freeMMRs)(mmrs);
			return NULL;
		}

		if (cnt == 0){ // if there were no moves we evaluate our state with either win or lose
			switch (isMaxPlayer){
			case 1:
				(*res).index = -1;
				(*res).value = -100;
				break;
			case -1:
				(*res).index = -1;
				(*res).value = 100;
				break;
			default:break;
			}
		}
		else{ //mmrs isnt empty**/

			switch (isMaxPlayer){  //having the evaluation for each move applied to our state, we chose the best one for our purpose switching over IsMaxPLayer
			case 1: // if we are in a maximizing proccess 
				maxind = 0;
				cnt = 0;
				maxscore = (*(mmr_PTR)headData(mmrs)).value;
				mmrs = tail(mmrs);
				while (mmrs != NULL){
					cnt++;
					if ((*(mmr_PTR)headData(mmrs)).value > maxscore){
						maxscore = (*(mmr_PTR)headData(mmrs)).value;
						maxind = cnt;
					}
					mmrs = tail(mmrs);
				}
				(*res).index = maxind;
				(*res).value = maxscore;
				break;
			case -1: // if we are in a minimizing proccess , maxscore is actually minscore
				maxind = 0;
				cnt = 0;
				maxscore = (*(mmr_PTR)headData(mmrs)).value;
				mmrs = tail(mmrs);
				while (mmrs != NULL){
					cnt++;
					if ((*(mmr_PTR)headData(mmrs)).value < maxscore){
						maxscore = (*(mmr_PTR)headData(mmrs)).value;
						maxind = cnt;
					}
					mmrs = tail(mmrs);
				}
				(*res).index = maxind;
				(*res).value = maxscore;
				break;
			default:
				break;

			}
		}

		(*freeMMRs)(mmrscopy);//freeCheck
		return res; // return the best mmr

	}
	else{ //reached a leaf, in case maxdepth=0, we evaluate the board without looking at its children	
		mmr_PTR res;
		int player = isMaxPlayer*(((GameState *)state)->player); //  maxplayer is white iff minplayer is black iff player=1,so let the eval score as it is, else multiply it by -1 
		int eval = (*evaluate)((GameState *)state);
		//global++;
		//printf("\n%d\n", global);
		res = init_MiniMaxResult(eval*player, 0);

		if (res == NULL){
			return NULL;
		}
		return res;
	}
}


//FREE FUNCS for OBJECTS//


void freeState(GameState* c1){
	free(c1);
}

void freeMMR(mmr_PTR c1){
	free(c1);
}

void freeEmpty(ListRef c1){
	free(c1);
}

void freeLoc(LOC_PTR c1){
	/**printf("\n%d\n%d\n", c1->row, c1->col);**/
	free(c1);
}

void freeMove(move_PTR c1){
	freeLocList(c1->moves);
	free(c1);
}

//FREE FUNCS for LISTS, uses recursion over the given list, freeing each of its members//

void freeStateList(ListRef state_list){
	if (isEmpty(state_list)){
		freeEmpty(state_list);
	}
	else {
		if (tail(state_list) != NULL){

			freeStateList(tail(state_list));

		}
		destroyNext(state_list);
		freeState((GameState *)headData(state_list));
		freeEmpty(state_list);
	}
}

void freeMoveList(ListRef move_list){
	if (isEmpty(move_list)){
		freeEmpty(move_list);
	}
	else {
		if (tail(move_list) != NULL){

			freeMoveList(tail(move_list));

		}
		destroyNext(move_list);
		freeMove((move_PTR)headData(move_list));
		freeEmpty(move_list);
	}
}

void freeMMRs(ListRef mmrs_list){
	if (isEmpty(mmrs_list)){
		freeEmpty(mmrs_list);
	}
	else {
		if (tail(mmrs_list) != NULL){

			freeMMRs(tail(mmrs_list));

		}
		destroyNext(mmrs_list);
		freeMMR((mmr_PTR)headData(mmrs_list));
		freeEmpty(mmrs_list);
	}
}

void freeLocList(ListRef loc_list){
	if (isEmpty(loc_list)){
		freeEmpty(loc_list);
	}
	else {
		if (tail(loc_list) != NULL){
			freeLocList(tail(loc_list));
		}
		destroyNext(loc_list);
		freeLoc((LOC_PTR)headData(loc_list));
		freeEmpty(loc_list);

	}
}















