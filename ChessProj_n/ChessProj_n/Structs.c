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

GameState *makeLegalMove(GameState *curstate, move_PTR curmove){
	int toRow;
	int toCol;
	int curRow;
	int curCol;
	int firstRow;
	int firstCol;
	int regMove;
	char curman;
	int Keaten;
	int Meaten;
	GameState *res;
	Keaten = 0;
	Meaten = 0;
	regMove = 0;
	res = copyState(curstate);
	if (res == NULL){
		return NULL;
	}
	(*res).player = (*res).player*(-1);
	ListRef locs = curmove->moves;
	firstRow = ((LOC_PTR)headData(locs))->row;
	firstCol = ((LOC_PTR)headData(locs))->col;
	curman = (*res).cur_board[firstCol][firstRow];
	locs = tail(locs);
	(*res).cur_board[firstCol][firstRow] = EMPTY;
	if (curman == 'k' || curman == 'K'){ //handle first move seperatley, IF KING
		toRow = ((LOC_PTR)headData(locs))->row;
		toCol = ((LOC_PTR)headData(locs))->col;
		if ((toRow > firstRow && toCol > firstCol) && ((*res).cur_board[toCol - 1][toRow - 1] != EMPTY)){// up right eat	
			if (TOLOWER((*res).cur_board[toCol - 1][toRow - 1]) == 'k'){
				Keaten++;
			}
			else{
				Meaten++;
			}
			(*res).cur_board[toCol - 1][toRow - 1] = EMPTY;
			(*res).cur_board[toCol][toRow] = curman;

		}
		else if ((toRow<firstRow && toCol > firstCol) && ((*res).cur_board[toCol - 1][toRow + 1] != EMPTY)){//down right eat	
			if (TOLOWER((*res).cur_board[toCol - 1][toRow + 1]) == 'k'){
				Keaten++;
			}
			else{
				Meaten++;
			}
			(*res).cur_board[toCol - 1][toRow + 1] = EMPTY;
			(*res).cur_board[toCol][toRow] = curman;

		}
		else if ((toRow < firstRow && toCol < firstCol) && ((*res).cur_board[toCol + 1][toRow + 1] != EMPTY)){// down left eat	
			if (TOLOWER((*res).cur_board[toCol + 1][toRow + 1]) == 'k'){
				Keaten++;
			}
			else{
				Meaten++;
			}
			(*res).cur_board[toCol - 1][toRow - 1] = EMPTY;
			(*res).cur_board[toCol][toRow] = curman;

		}
		else if ((toRow>firstRow && toCol < firstCol) && ((*res).cur_board[toCol + 1][toRow - 1] != EMPTY)){ // up left eat
			if (TOLOWER((*res).cur_board[toCol + 1][toRow - 1]) == 'k'){
				Keaten++;
			}
			else{
				Meaten++;
			}
			(*res).cur_board[toCol + 1][toRow - 1] = EMPTY;
			(*res).cur_board[toCol][toRow] = curman;

		}
		else{ //king regular move
			(*res).cur_board[toCol][toRow] = curman;
			regMove = 1;
		}
	}
	else{ // IF MAN

		toRow = ((LOC_PTR)headData(locs))->row;
		toCol = ((LOC_PTR)headData(locs))->col;
		if (tail(locs) == NULL && abs(firstRow - toRow) == 1){ //check if regular move		
			(*res).cur_board[firstCol][firstRow] = EMPTY;
			(*res).cur_board[toCol][toRow] = manToKing(toRow, curman);
			regMove = 1;
		}
		else{ // man eat
			curCol = firstCol;
			curRow = firstRow;
			(*res).cur_board[firstCol][firstRow] = EMPTY;
			if (TOLOWER((*res).cur_board[(curCol + toCol) / 2][(curRow + toRow) / 2]) == 'k'){
				Keaten++;
			}
			else{
				Meaten++;
			}
			(*res).cur_board[(curCol + toCol) / 2][(curRow + toRow) / 2] = EMPTY;
			(*res).cur_board[toCol][toRow] = manToKing(toRow, curman);
		}
	}
	if (!regMove){//NOW ALL PIECES ARE EQUAL, ALL MOVES ARE EATS
		firstRow = toRow;
		firstCol = toCol;
		locs = tail(locs);
		while (locs != NULL){
			toRow = ((LOC_PTR)headData(locs))->row;
			toCol = ((LOC_PTR)headData(locs))->col;
			(*res).cur_board[firstCol][firstRow] = EMPTY;
			if (TOLOWER((*res).cur_board[(firstCol + toCol) / 2][(firstCol + toRow) / 2]) == 'k'){
				Keaten++;
			}
			else{
				Meaten++;
			}
			(*res).cur_board[(firstCol + toCol) / 2][(firstRow + toRow) / 2] = EMPTY;
			(*res).cur_board[toCol][toRow] = curman;
			firstRow = toRow;
			firstCol = toCol;
			locs = tail(locs);
		}
		(*res).cur_board[toCol][toRow] = manToKing(toRow, curman);

	}
	if (curman == 'k' || curman == 'm'){ //player is white
		(*res).black_kings = (*res).black_kings - Keaten;
		(*res).black_men = (*res).black_men - Meaten;
	}
	else{
		(*res).white_kings = (*res).white_kings - Keaten;
		(*res).white_men = (*res).white_men - Meaten;
	}
	return res;
}

GameState* start(char board[BOARD_SIZE][BOARD_SIZE], int depth){

	GameState* state0;
	int blackM, blackK, whiteM, whiteK, i, j;
	blackM = 0;
	blackK = 0;
	whiteM = 0;
	whiteK = 0;
	state0 = (GameState*)malloc(sizeof(GameState));
	if (state0 == NULL){
		return NULL;
	}
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			(*state0).cur_board[i][j] = board[i][j];
			switch (board[i][j]){
			case 'M':
				blackM++;
				break;
			case 'K':
				blackK++;
				break;
			case 'm':
				whiteM++;
				break;
			case 'k':
				whiteK++;
				break;
			default:
				break;
			}
		}
	}
	(*state0).minmaxDepth = depth;
	(*state0).player = WHITE;
	(*state0).black_kings = blackK;
	(*state0).black_men = blackM;
	(*state0).white_kings = whiteK;
	(*state0).white_men = whiteM;
	return state0;
}


void clear(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	i = 0;
	j = 0;
	while (i < BOARD_SIZE){
		j = 0;
		while (j<BOARD_SIZE){
			*(*(board + i) + j) = EMPTY;
			j++;
		}
		i++;
	}
}

void set(struct Location loc, char a, char b, char board[BOARD_SIZE][BOARD_SIZE]){
	if ((loc.row + loc.col) % 2 != 0 || (loc.row >= 10 || loc.col >= 10)){
		print_message(WRONG_POSITION);
	}
	else{
		if (a == 'b'){
			if (b == KING || loc.row == 0){
				*(*(board + loc.col) + loc.row) = 'K';
			}
			else{
				*(*(board + loc.col) + loc.row) = 'M';
			}


		}
		else{
			if (b == KING || loc.row == 9){
				*(*(board + loc.col) + loc.row) = 'k';
			}
			else{
				*(*(board + loc.col) + loc.row) = 'm';
			}
		}
	}
}

void rm(struct Location loc, char board[BOARD_SIZE][BOARD_SIZE]){
	int row = loc.row;
	int col = loc.col;
	if ((row >= 10 || col >= 10) || ((col + row) % 2 != 0)){
		print_message(WRONG_POSITION);
	}
	else{
		*(*(board + loc.col) + loc.row) = EMPTY;
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
			if ((i + j) % 2 == 0){
				if (j <= 3){

					board[i][j] = WHITE_M;
				}
				else if (j >= 6){

					board[i][j] = BLACK_M;
				}
				else{
					board[i][j] = EMPTY;
				}
			}
			else{
				board[i][j] = EMPTY;
			}
		}
	}
}

int evaluate(void* state){
	int i, j, score = 0;
	int white = 0;
	int black = 0;
	score = 0;
	for (i = 0; i<BOARD_SIZE; i++){
		for (j = 0; j<BOARD_SIZE; j++){
			if ((*(GameState*)(state)).cur_board[i][j] == WHITE_M){
				score++;
				white++;
			}
			else if ((*(GameState*)(state)).cur_board[i][j] == WHITE_K){
				score += 3;
				white++;
			}
			else if ((*(GameState*)(state)).cur_board[i][j] == BLACK_M){
				score--;
				black++;
			}
			else if ((*(GameState*)(state)).cur_board[i][j] == BLACK_K){
				score -= 3;
				black++;
			}
			else{
				continue;
			}
		}
	}
	if (black == 0 && white == 0){
		score = 200;
	}
	else if (black == 0){
		score = 100;
	}
	else if (white == 0){
		score = -100;
	}


	return score;
}

int ifStartOK(GameState* startstate){
	int res = 1;
	int BK = (*startstate).black_kings;
	int WK = (*startstate).white_kings;
	int WM = (*startstate).white_men;
	int BM = (*startstate).black_men;
	if (BK + WK + WM + BM == 0){
		res = 0;
	}
	else if (BK + BM == 0 || WK + WM == 0){
		res = 0;
	}
	if (BK + BM>20 || WK + WM>20){
		res = 0;
	}
	return res;
}
