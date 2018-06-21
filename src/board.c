#include <stdio.h>
#include <stdlib.h>

#include "libsx.h"

#include "board.h"

Widget* rec;

int board_row = 4, board_col = 4;
int board_size;

/*
	Set which size of game to be show on leaderboard.
*/
void SetSizeInBoard(int r, int c) {
	board_row = r, board_col = c;
	board_size = board_row * board_col;
}

/*
	Copy record from r2 to r1
*/
void CopyRecord(Record* r1, Record* r2) {
	strcpy(r1 -> name, r2 -> name);
	r1 -> score = r2 -> score;
	r1 -> size = r2 -> size;
}

/*
	Load leaderboard from file and store into where parameter point to.
*/
void LoadBoard(Record** record, int* num) {
	FILE* fp = fopen("board.txt", "r");
	if (fp == NULL) {
		return;
	}
	*num = 0;
	int tot;
	// read in total number of record
	if (fscanf(fp, "%d", &tot) != 1) {
		// no valid total number
		*record = (Record*)malloc(11 * sizeof(Record));
		(*record)[10].score = -1;
		fclose(fp);
		return;
	}
	*record = (Record*)malloc((tot + 11) * sizeof(Record));

	int extra = 10;
	// use %s to readin the name, that's why no space is allowed in name
	while (*num < 10 && fscanf(fp, "%s %d %d", (*record)[*num].name, &(*record)[*num].score, &(*record)[*num].size) == 3) {
		if ((*record)[*num].size == board_size) {
			*num += 1;
		} else {
			// records for other size game
			CopyRecord((*record) + extra, (*record) + *num);
			extra += 1;
		}
	}
	// mark the end of the board
	(*record)[extra].score = -1;
	fclose(fp);
}

/*
	Save records.
*/
void SaveBoard(Record* record, int num) {
	FILE* fp = fopen("board.txt", "w");
	int i, tot = num;
	// count the record for other size
	for (i = 10; record[i].score != -1; i++, tot++);
	fprintf(fp, "%d\n", tot);
	for (i = 0; i < num; i++) {
		fprintf(fp, "%s %d %d\n", record[i].name, record[i].score, record[i].size);
	}
	for (i = 10; record[i].score != -1; i++) {
		fprintf(fp, "%s %d %d\n", record[i].name, record[i].score, record[i].size);
	}
	fclose(fp);
}

/*
	Print records to labels.
*/
void PrintBoard() {
	Record* record;
	int num, i;
	LoadBoard(&record, &num);
	for (i = 0; i < 10; i++) {
		if (i >= num) {
			// default score: 59min59sec
			record[i].score = 3599;
			// default name: NULL
			strcpy(record[i].name, "NULL");
		}
		// format the score in min-sec
		char str[40];
		int sec = record[i].score;
		int min = sec / 60;
		sec %= 60;
		sprintf(str, "No.%2d  %10s   %4dmin %2dsec", i + 1, record[i].name, min, sec);
		SetLabel(rec[i], str);
	}
}

/*
	Player confirm the leaderboard and quit.
*/
void Ok(Widget w, void* data) {
	CloseWindow();
}

/*
	Initiate the leaderboard with no records.
*/
void Reset(Widget w, void* data) {
	Record* record;
	int num = 0;
	LoadBoard(&record, &num);
	num = 0;
	SaveBoard(record, num);
	PrintBoard(&record, &num);
}

/*
	Build leaderboard UI
*/
void StartBoard() {
	rec = (Widget*)malloc(10 * sizeof(Widget));
	int i;
	for (i = 0; i < 10; i++) {
		// initiate an empty label for each record
		rec[i] = MakeLabel(
			"                                       ");
		if (i) {
			SetWidgetPos(rec[i], PLACE_UNDER, rec[i - 1], NO_CARE, NULL);
		}
	}
	// add OK and reset button
	Widget empty = MakeLabel("      ");
	Widget ok = MakeButton("   OK   ", Ok, NULL);
	Widget reset = MakeButton(" Reset ", Reset, NULL);
	SetWidgetPos(empty, PLACE_UNDER, rec[9], NO_CARE, NULL);

	// organize the buttons
	SetWidgetPos(ok, PLACE_RIGHT, empty, PLACE_UNDER, rec[9]);
	SetWidgetPos(reset, PLACE_RIGHT, ok, PLACE_UNDER, rec[9]);

	// fill the label with records
	PrintBoard();

	ShowDisplay();
	MainLoop();

	// the content of rec array is no longer valid, free it
	free(rec);
}