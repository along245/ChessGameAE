
#include "Structs.h"
#define f_error(s) ("cannot open file %s",s)

void saveGame(char *filename, GameState *s){
	char row[9];
	int index = 0;
	FILE *out = fopen(filename, 'w');
	if (out == NULL){
		ferror(filename);
		exit(0);
	}
	int i = 7;
	fprintf(out, "<game>\n");
	fprintf(out, "<next_turn>%s</next_turn>\n", s->player);
	fprintf(out, "<game_mode>%s</game_mode>\n", s->gameMode);
	fprintf(out, "<difficulty>%s</difficulty>\n", s->difficulty);
	fprintf(out, "<user_color>%s</user_color>\n", s->userColor);
	fprintf(out, "<board>\n");
	for (index; index >= 0; index--){
		for (i = 0; i < 8; i++){
			row[i] = s->cur_board[i, index];
		}
		row[i] = '\0';
		fprintf(out, "<row_%d>%s</row%d>\n", index, row, index);
	}

	fclose(out);
}

GameState *loadGame(char *filename){
	FILE *in = fopen(filename, 'r');
	if (in == NULL){
		f_error(filename);
		exit(0);
	}
	GameState *state = (GameState*)malloc(sizeof(GameState));
	if (state == NULL){
		exit(2);
	}
	
}//