#include "ChessLogic.h"
#include "Structs.c"
#include <math.h>




int main()
{
	int mode;
	char *input;
	int  i, len;
	int cnt;
	char column;
	int row;
	int win;
	int next_p;
	LOC_PTR curloc;
	char c;
	int curcol;
	int currow;
	int moveExists;
	char typo;
	char color;
	char *token;
	int inGS;
	char *comm;
	int depth;
	int wrongPos;
	int wrongMan;
	char *inputcopy;
	int firstRow, firstCol;	
	ListRef cur_moves;
	ListRef copycur_moves;
	move_PTR tmpmove;
	int user_col;
	move_PTR user_move;
	mmr_PTR bestchild;
	char cpyBoard[BOARD_SIZE][BOARD_SIZE];
	GameState *curState;
	GameState *curStatecpy;
	char path[50];
	int pathlen;
	user_col = WHITE;
	depth = 1;
	next_p = 1;
	int pcPlays;
	char board[BOARD_SIZE][BOARD_SIZE];
	init_board(board);
	init_board(cpyBoard);
	clear(cpyBoard);
	inGS = 0; // A boolean represents whether we are in the gamestate
	mode = 1; //Default Mode set to pvp



	while (!(inGS)) {  //Not in game state
		printf(ENTER_SETTINGS);
		i = 0;		
		input = (char *)malloc(52 * sizeof(char));
		if (input == NULL){
			perror_message("malloc");
			exit(0);
		}
		while ((c = getchar()) != '\n') {			
			input[i] = c;
			i++;
			len--;
		}
		input[i] = ' ';
		input[i + 1] = '\0';//stop

		if (strcmp(input, "start ") && !(inGS)){ ///SETTING STATE
			comm = (char *)malloc(sizeof(char)*(strlen(input) + 1));
			if (comm == NULL){
				free(input);
				perror_message("malloc");
				exit(0);
			}
			cnt = 0;
			while (input[cnt] != ' '){
				comm[cnt] = input[cnt];
				cnt++;
			}
			comm[cnt] = '\0'; //comm is the requested command if valid

			if (!(strcmp(comm, "print"))){ //PRINT
				print_board(board);
			}

			else if (!(strcmp(comm, "load"))){ // load
				int pathlen = 1;
				while (input[cnt + pathlen] != '/0'){
					path[pathlen - 1] = input[cnt + pathlen];
					pathlen++;
				}
				path[pathlen + 1] = '\0';
				if (init_EmptyState(curState) == NULL){
					free(comm);
					free(input);
					perror_message("malloc");
					exit(0);
				}
				if (loadtoState(path, curState, mode) == -1){
					printf(WRONG_FILE_NAME);
				}
				else{ //GAME LOADED 
					curStatecpy = curState;
					inGS = 1;
					cur_moves = getChildren(curState);
					copycur_moves = cur_moves;
					print_board(curState->cur_board);
				}
			}				
			else if (!(strcmp(comm, "clear"))){  //CLEAR
				clear(board);
			}
			else if (!(strcmp(comm, "rm"))){  // REMOVE
				column = input[cnt + 2];				
				row = calcInt(input[cnt + 4]) - 1;								
				curloc = init_Location(calcPos(column), row, board);
				if (curloc == NULL){
					perror_message("malloc");
					free(comm);
					free(input);
					exit(0);
				}
				rm(curloc, board);
				freeLoc(curloc);						
			}

			else if (!strcmp(comm, "next_player")){ // NEXT PLAYER
				next_p = calc_int(input[cnt + 1]);
			}
			else if (!(strcmp(comm, "set"))){ // SET
				column = input[cnt + 2];				
				row = calcInt(input[cnt + 4]) - 1;
				color = input[cnt + 7];
				typo = input[cnt + 13];				
				curloc = init_Location(calcPos(column), row, board);
				if (curloc == NULL){
					perror_message("malloc");
					free(comm);
					free(input);
					exit(0);
				}
				copyBoard(board, cpyBoard);
				if (set(curloc, color, typo, board, cpyBoard) == -1){
					perror_message("malloc");
					free(comm);
					free(input);
					free(curloc);
					exit(0);
				}
				freeLoc(curloc);
				
			}
			else if (!(strcmp(comm, "quit"))){ // QUIT
				free(comm);
				free(input);
				exit(0);
			}
			else if (!(strcmp(comm, "difficulty"))){ //MINIMAX DEPTH
				if (mode == 1){
					printf(ILLEGAL_COMMAND);
				}
				else{
					if (input[cnt + 1] == 'b'){ //BEST
						depth = 5;
					}
					else if (input[cnt + 1] == 'd'){ //difficulty depth x
						int d = calcInt(input[cnt + 7]);
						if (d > 4 || d < 1){
							printf(WRONG_MINIMAX_DEPTH);
						}
						else{
							depth = d;
						}
					}
				}
				
			}
			else if (!(strcmp(comm, "user_color"))){ //USER COLOR
				if (mode == 2){
					if (input[cnt + 1] == 'b'){
						user_col = BLACK;
					}
					else{
						user_col = WHITE;
					}
				}
				else{
					printf(ILLEGAL_COMMAND);
				}
				
			}
			else if (!strcmp(input, "start ") && !(inGS)){//PRE GAME STATE ///// BOARD INIT CHECK////			
				curState = start(board, depth, next_p,mode,user_col);
				if (curState == NULL){
					perror_message("malloc");
					free(input);
					exit(0);
				}
				curStatecpy = curState;
				if (ifStartOK(curState)){ // CHECKS IF THE BOARD IS WELL INITIALIZED
					inGS = 1;
					cur_moves = getChildren(curState);
					if (cur_moves == NULL){
						perror_message("malloc");
						free(curState);
						free(input);
						exit(0);
					}
					copycur_moves = cur_moves;
					if (isEmpty(cur_moves)){ // TIE OR LOSE, ANYWAY GAME ENDS
						if (isTie(curState)){// TIE
							printf(TIE);
						}
						else{//LOSE
							if (curState->player == 1){ // PLAYER IS WHITE
								printf("Mate! Black player wins the game\n");
							}
							else{
								printf("Mate! White player wins the game\n");
							}
						}
						freeState(curState);
						free(comm);
						free(input);
						freeMoveList(cur_moves);//?
						exit(0);
					}
					else{
						printf(WROND_BOARD_INITIALIZATION);
						freeState(curState);
					}
				}
			}
				free(comm);
				free(input);
			
		}//END OF SETTING STATE	
	
	while (inGS){ //IN GAME STATE, means we passed the initilization test and inGS=1
		if ((*curState).player == user_col){ //USER's TURN			
			printf(ENTER_YOUR_MOVE);
			i = 0;
			len = 20;
			input = (char *)malloc(20 * sizeof(char));
			if (input == NULL){
				perror_message("malloc");
				freeState(curStatecpy);
				freeMoveList(copycur_moves);//FREEMOVE
				exit(0);
			}
			while ((c = getchar()) != '\n') {
				if (len == 0){
					input = (char *)realloc(input, sizeof(char)*(i + 20));
					len = 20;
					if (input == NULL){
						perror_message("realloc");
						freeState(curStatecpy);
						freeMoveList(copycur_moves);//FREEMOVE
						exit(0);
					}
				}
				input[i] = c;
				i++;
				len--;
			}
			input = (char *)realloc(input, sizeof(char)*(i + 3));
			if (input == NULL){
				perror_message("realloc");
				freeState(curStatecpy);
				freeMoveList(copycur_moves);//FREEMOVE
				exit(0);
			}
			input[i] = '\0';//stop	
			inputcopy = input; //SAVE COPY OF INPUT IN CASE OF MOVE TO OR INVALID
			//COMMANDS//
			if (!strcmp(input, "quit")){ //QUIT
				freeMoveList(copycur_moves);
				freeState(curStatecpy);
				free(input);
				exit(0);
			}
			else if (!strcmp(input, "get_moves")){ //GETMOVES

				printAllMoves(cur_moves);
				free(input);
			}



			else{ // MOVE TO or ILLEGAL command
				wrongPos = 0;
				wrongMan = 0;
				moveExists = 0;

				comm = (char *)malloc(sizeof(char)*(strlen(input) + 1));
				if (comm == NULL){
					perror_message("malloc");
					freeState(curStatecpy);
					freeMoveList(copycur_moves);//FREEMOVE
					free(input);
					exit(0);
				}
				cnt = 0;
				while (input[cnt] != '<' && input[cnt] != '\0'){
					comm[cnt] = input[cnt];
					cnt++;
				}
				comm[cnt] = '\0';
				if (!(strcmp(comm, "move "))){ //MOVE to command
					curcol = calcPos(input[6]);
					if (input[10] == '>'){
						currow = calcInt(input[8]) * 10 + calcInt(input[9]) - 1;
						input = input + 15; //going to actual moves, gaping " to "
					}
					else{
						currow = calcInt(input[8]) - 1;
						input = input + 14;//going to actual moves, gaping " to "
					}
					if (legalPos(curcol, currow)){ // CHECK IF INITIAL POSITION IS OK
						curloc = init_Location(curcol, currow, (*curState).cur_board);
						if (curloc == NULL){
							perror_message("malloc");
							free(inputcopy);
							freeState(curStatecpy);
							freeMoveList(copycur_moves);//FREEMOVE
							exit(0);
						}
						user_move = init_move(curloc); // now input =<><>....
						if (user_move == NULL){
							perror_message("malloc");
							free(inputcopy);
							freeState(curStatecpy);
							freeMoveList(copycur_moves);//FREEMOVE
							freeLoc(curloc);
							exit(0);
						}
						cnt = 0; //WE HAVE USERMOVE AND CURLOC ON HEAP
						token = strtok(input, ">"); //STRTOK, INPUT IS DESTORYED, CARE FOR MEMORY!
						curcol = calcPos(token[1]);
						if (token[4] != '\0'){
							currow = 10 * calcInt(token[3]) + calcInt(token[4]) - 1;
						}
						else{
							currow = calcInt(token[3]) - 1;
						}

						if (legalPos(currow, curcol)){ ///CHECK IF SECOND POS IS OK

							curloc = init_Location(curcol, currow, (*curState).cur_board);
							if (curloc == NULL){
								perror_message("malloc");
								free(inputcopy);
								freeState(curStatecpy);
								freeMoveList(copycur_moves);//FREEMOVE
								freeMove(user_move);//FREEMOVE
								exit(0);
							}
							tmpmove = add_move(user_move, curloc);

							if (tmpmove == NULL){
								perror_message("malloc");
								free(inputcopy);
								freeLoc(curloc);
								freeState(curStatecpy);
								freeMoveList(copycur_moves);//FREEMOVE
								freeMove(user_move);//FREEMOVE
								exit(0);

							}
							token = strtok(NULL, ">");
							while (token != NULL && token != '\0'){//START OF TOKEN LOOP
								curcol = calcPos(token[1]);
								if (token[4] != '\0'){ // no '>'
									currow = 10 * calcInt(token[3]) + calcInt(token[4]) - 1;
								}
								else{
									currow = calcInt(token[3]) - 1;
								}
								if (legalPos(currow, curcol)){ //CHECK IF THIRD AND ON POSs ARE OK

									curloc = init_Location(curcol, currow, (*curState).cur_board);
									if (curloc == NULL){
										perror_message("malloc");
										free(inputcopy);
										freeState(curStatecpy);
										freeMoveList(copycur_moves);//FREEMOVE
										freeMove(user_move);//FREEMOVE
										exit(0);
									}
									tmpmove = add_move(user_move, curloc);
									if (tmpmove == NULL){
										perror_message("malloc");
										free(inputcopy);
										freeLoc(curloc);
										freeState(curStatecpy);
										freeMoveList(copycur_moves);//FREEMOVE
										freeMove(user_move);//FREEMOVE
										exit(0);

									}
								}
								else{ // THIRD OR MORE POS IS CORRUPTED
									wrongPos = 1;
									freeMove(user_move);//FREEMOVE
									break;
								}

								token = strtok(NULL, ">");
							} //END OF TOKEN LOOP
						}
						else{ //SECOND POS IS CORRUPTED
							wrongPos = 1;
							freeMove(user_move);//FREEMOVE
						}

					}

					else{ //INITIAL POSITION IS WRONG
						wrongPos = 1;
					}///EITHER move is ready or an error 

					if (wrongPos == 1){ // (1) WRONG POSITION in one of the locs
						printf(WRONG_POSITION);


					}
					else{ // NO CORRUPTED POSs
						firstRow = (*(LOC_PTR)(headData((*user_move).moves))).row;
						firstCol = (*(LOC_PTR)(headData((*user_move).moves))).col;

						if (((((*curState).cur_board[firstCol][firstRow] == 'm') || ((*curState).cur_board[firstCol][firstRow] == 'k')) && (*curState).player == WHITE) ||
							((((*curState).cur_board[firstCol][firstRow] == 'M') || ((*curState).cur_board[firstCol][firstRow] == 'K')) && (*curState).player == BLACK)){ //// (2) WRONG MAN
							wrongMan = 0;
						}
						else{
							wrongMan = 1;
						}
						if (wrongMan == 1){
							printf(NO_DICS);
							freeMove(user_move);//FREEMOVE

						}
						else{ //We passed both tests, disc and positions are all OK, we check if the move actually exists
							moveExists = 0;
							while (cur_moves != NULL){ //we look for the move in the moves list					
								if (compareMoves((move_PTR)headData(cur_moves), user_move)){
									moveExists = 1;
									break;
								}
								cur_moves = tail(cur_moves);
							}
							if (moveExists){//MOVE EXISTS
								curState = makeLegalMove(curState, (move_PTR)headData(cur_moves)); //CHANGES THE PLAYER, and performs the player's move on the board, curstate is updated
								if (curState == NULL){
									perror_message("malloc");
									freeMove(user_move);//FREEMOVE
									freeState(curStatecpy);
									freeState(curState);
									freeMoveList(cur_moves);//FREEMOVE
									free(comm);
									free(inputcopy);//?
									exit(0);
								}
								freeState(curStatecpy);
								curStatecpy = curState;
								print_board((*curState).cur_board);


								win = gameOver(curState); //CHECK IF THERE ARE NO MORE DISCS OF SOME COLOR, after our move
								if (win != 0){ //0 means game continues
									if (win == BLACK){
										printf("Black player wins!\n");
									}
									else{
										printf("White player wins!\n");
									}
									free(comm);
									freeMoveList(copycur_moves);//FREEMOVE
									free(inputcopy);//?
									freeMove(user_move);
									freeState(curStatecpy);
									exit(0);
								}

								cur_moves = getChildren(curState); // cur_moves contains the rival's moves now

								if (cur_moves == NULL){
									perror_message("malloc");
									freeMove(user_move);//FREEMOVE
									freeState(curStatecpy);
									freeMoveList(copycur_moves);//FREEMOVE
									free(comm);
									free(inputcopy);//?
									exit(0);
								}
								freeMoveList(copycur_moves);//FREEMOVE
								copycur_moves = cur_moves;
								if (isEmpty(cur_moves)){//CHECK IF COMPUTER LOSES by having no more moves
									if (user_col == WHITE){
										printf("White player wins!\n");
									}
									else{
										printf("Black player wins!\n");
									}
									free(inputcopy);
									free(comm);
									freeState(curStatecpy);
									freeMoveList(copycur_moves);//FREEMOVE
									freeMove(user_move);//FREEMOVE
									exit(0);
								}
							}
							else { // (3) move doesn't exist in the move list
								printf(ILLEGAL_MOVE);
								cur_moves = copycur_moves;

							} //end of check (3)
							freeMove(user_move);//FREEMOVE
						}//end of check (2)						
					}//end of check (1)

				}//END OF MOVE
				else{ //ILLEGAL command
					printf(ILLEGAL_COMMAND);
				}
				free(inputcopy);
				free(comm);
			}
		}///END OF USER TURN
		else{//if its the pc's turn		, no chance he has no moves, since we checked it before			
			bestchild = getBestChild(curState, depth, &getChildren, (FreeFunc)&freeMoveList, (FreeFunc)&freeMMRs, (FreeFunc)&freeState, (FreeFunc)&freeMMR, &evaluate, 1); //find the best move

			if (bestchild == NULL){
				perror_message("malloc");
				freeState(curStatecpy);
				freeMoveList(copycur_moves);//FREEMOVE
				exit(0);
			}
			cnt = (*bestchild).index; // the index of the best move in the moves list
			//gameVal = (*bestchild).value;
			freeMMR(bestchild);

			while (cnt > 0){
				cur_moves = tail(cur_moves);
				cnt--;
			}
			printf("Computer: move ");
			printMove((move_PTR)headData(cur_moves));
			printf("\n");
			curState = makeLegalMove(curState, (move_PTR)headData(cur_moves)); // curstate updated with pc's chosen move
			if (curState == NULL){
				perror_message("malloc");
				freeState(curStatecpy);
				freeMoveList(copycur_moves);//FREEMOVE
				exit(0);
			}
			print_board((*curState).cur_board);
			freeState(curStatecpy);
			curStatecpy = curState;


			win = gameOver(curState); //CHECK IF THERE ARE NO MORE DISCS OF SOME COLOR, after the computer's move
			if (win != 0){ //0 means game continues
				if (win == BLACK){
					printf("Black player wins!\n");
				}
				else{
					printf("White player wins!\n");
				}

				freeMoveList(copycur_moves);//FREEMOVE
				freeState(curStatecpy);
				exit(0);
			}
			cur_moves = getChildren(curState); // if the PC made a move update curmoves to our moves			
			if (cur_moves == NULL){
				perror_message("malloc");
				freeState(curStatecpy);
				freeMoveList(copycur_moves);//FREEMOVE
				exit(0);
			}
			freeMoveList(copycur_moves);
			copycur_moves = cur_moves;
			if (isEmpty(cur_moves)){//CHECK IF USER LOSES, by having no more moves
				if (user_col == BLACK){
					printf("White player wins!\n");
				}
				else{
					printf("Black player wins!\n");
				}
				freeState(curStatecpy);
				freeMoveList(copycur_moves);//FREEMOVE				
				exit(0);
			}

		}//END OF PC TURN


	}//END OF GAME STATE
}

