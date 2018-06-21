#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libsx.h"

#include "game.h"
#include "board.h"

const int WINDOW_WID = 400;
const int WINDOW_HEI = 500;

char BUTTON_LABEL[400000];

typedef struct {
	Widget w;		// button for the card
	int r, g, b;	// color in RGB format
	int vis;		// if the color is visible or not, paired or not
} Card;

Card* cards;		// cards array

int table_row = 4;	// row number of cards table
int table_col = 4;	// column number of cards table

Card *prev1_card, *prev2_card;	// pointer to last two cards

enum {
	NOT_START,
	PLAYING,
	SHOW_RES,
	CLOSING,
	CLOSED
} game_status = CLOSED;		// indicates current game status

int rem_card;		// the number of cards remain to be paird
int start_time;		// start time when first click happened
int del_time;		// time elapsed during clicking
Widget timer;		// label show the time been used

/*
	Reset all global states
*/
void CloseGame() {
	prev1_card = NULL;
	prev2_card = NULL;
	start_time = -1;
	del_time = 0;
	rem_card = 0;
	free(cards);
	game_status = CLOSED;
}

/*
	Check if two cards are with same color
*/
int SameColor(Card* card1, Card* card2) {
	if (card1 -> r != card2 -> r) {
		return 0;
	}
	if (card1 -> g != card2 -> g) {
		return 0;
	}
	if (card1 -> b != card2 -> b) {
		return 0;
	}
	return 1;
}

/*
	Set the color of one card
*/
void SetCardColor(Card* card, int r, int g, int b) {
	card -> r = r;
	card -> g = g;
	card -> b = b;
	card -> vis = 0;
}

/*
	Hide the color, which is setting the color to white
*/
void RecoverColor(Card* card) {
	SetBgColor(card -> w, GetRGBColor(255, 255, 255));
	card -> vis = 0;
}

/*
	If a new record occurred, update the leaderboard with
	the new name and new score(time elapse)
*/
void UpdateLeaderBoard(Widget w, char *name, void *data) {
	Widget hint = *(Widget*)data;
	int len = strlen(name), i, j;
	// the length of name should not exceed 10
	if (len > 10) {
		SetLabel(hint, "The length should be less than 10. ");
		return;
	}
	// no space allowed
	for (i = 0; i < len; i++) {
		if (name[i] == ' ') {
			SetLabel(hint, "The name should not contains space.");
			return;
		}
	}
	int num;
	Record* record;
	LoadBoard(&record, &num);
	for (i = 0; i < num; i++) {
		// find the new record position
		if (del_time <= record[i].score) {
			// push back the lower ranks
			for (j = num; j > i; j--) {
				if (j < 10) {
					CopyRecord(record + j, record + j - 1);
				}
			}
			break;
		}
	}
	// add the new record to the leaderboard
	strcpy(record[i].name, name);
	record[i].score = del_time;
	record[i].size = table_row * table_col;
	num = num < 10 ? num + 1 : 10;
	// save leaderboard to file
	SaveBoard(record, num);
	free(record);
	CloseWindow();
}

/*
	Button callback to update leaderboard.
*/
void Confirm(Widget w, void* data) {
	Widget** ppw = (Widget**)data;
	UpdateLeaderBoard(w, GetStringEntry(*ppw[1]), ppw[0]);
}

/*
	Player confirm the score and quit the game.
*/
void GameOk(Widget w, void* data) {
	CloseWindow();
}

void ShowResult() {
	Widget result_window = MakeWindow("ColorMatch", SAME_DISPLAY, 1);
	// calculate the time elapsed
	del_time = time(0) - start_time;
	// format the time in min-sec form
	int sec = del_time;
	int min = sec / 60;
	sec %= 60;
	char str[30];
	sprintf(str,
		"     Complete in  %dmin %dsec !    ", min, sec);
	// add a label to demonstrate the score
	Widget result = MakeLabel(str);

	// check if this is a new record
	Record* record;
	int num, i, j;
	LoadBoard(&record, &num);
	int new_record = 0;
	for (i = 0; i < 10; i++) {
		if (i >= num || del_time <= record[i].score) {
			new_record = 1;
			break;
		}
	}
	free(record);

	if (new_record) {
		// congratulations
		Widget new_rec_lab = MakeLabel(
			"     You've made a new record!     ");
		SetWidgetPos(new_rec_lab, PLACE_UNDER, result, NO_CARE, NULL);
		// prompt player the name
		Widget ent_name = MakeLabel(
			"         Enter your name:          ");
		SetWidgetPos(ent_name, PLACE_UNDER, new_rec_lab, NO_CARE, NULL);
		// for later name checking error message
		Widget hint = MakeLabel(
			"                                   ");
		// for alignment
		Widget empty = MakeLabel("         ");

		// organize the widgets
		SetWidgetPos(empty, PLACE_UNDER, ent_name, NO_CARE, NULL);
		Widget name_input = MakeStringEntry(NULL, 100, UpdateLeaderBoard, &hint);
		SetWidgetPos(name_input, PLACE_RIGHT, empty, PLACE_UNDER, ent_name);
		SetWidgetPos(hint, PLACE_UNDER, name_input, NO_CARE, NULL);

		// add a button to trigger the leaderboard update
		Widget* tmp[2];
		tmp[0] = &hint;
		tmp[1] = &name_input;
		Widget empty2 = MakeLabel("            ");
		Widget confirm = MakeButton("Confirm", Confirm, &tmp);
		// organize the widgets
		SetWidgetPos(empty2, PLACE_UNDER, hint, NO_CARE, NULL);
		SetWidgetPos(confirm, PLACE_RIGHT, empty2, PLACE_UNDER, hint);
	} else {
		Widget empty = MakeLabel("        ");
		SetWidgetPos(empty, PLACE_UNDER, result, NO_CARE, NULL);
		Widget ok = MakeButton("Ok", GameOk, NULL);
		SetWidgetPos(ok, PLACE_RIGHT, empty, PLACE_UNDER, result);
	}

	ShowDisplay();
	MainLoop();

	// show new leaderboard
	Widget board = MakeWindow("Leaderboard", SAME_DISPLAY, 1);
	StartBoard();
}

/*
	Update the time elapsed and display in timer label widget.
*/
void UpdateTime(void *data) {
	// only update if player continue playing
	if (game_status == PLAYING) {
		// calculate the current elapse time
		int sec = time(0) - start_time;
		int min = sec / 60;
		sec %= 60;
		if (start_time == -1) {
			sec = min = 0;
		}
		// format the score
		char str[20];
		sprintf(str, "%dmin %dsec", min, sec);
		// display the score
		SetLabel(timer, str);
		AddTimeOut(100, UpdateTime, data);
	}
}

/*
	Callback function when a card is clicked.
	 - check if clicked card is already revealed
	 - check if last two card match
	 - check if all cards have been paired
*/
void Flip(Widget w, void *data) {
	// the first click
	if (game_status == NOT_START) {
		// start counting
		start_time = time(0);
		// change game state
		game_status = PLAYING;
		// add function to periodically update timer label
		AddTimeOut(100, UpdateTime, &timer);
	}

	// get the card which is clicked
	Card* card = (Card*) data;
	if (prev2_card != NULL) {
		// recover last two cards if their color don't match
		if (!SameColor(prev1_card, prev2_card)) {
			RecoverColor(prev1_card);
			RecoverColor(prev2_card);
		}
		// renew the last two card pointer
		prev1_card = NULL;
		prev2_card = NULL;
	}

	// click an already color-visible card
	// do nothing
	if (card -> vis) {
		return;
	}
	// reveal the color of the card
	SetBgColor(w, GetRGBColor(card -> r, card -> g, card -> b));
	// mark the card visible
	card -> vis = 1;
	// update last two cards pointer
	prev2_card = prev1_card;
	prev1_card = card;
	// check if last two click find a card pair
	if (prev2_card != NULL && SameColor(prev1_card, prev2_card)) {
		// substract remaining card counter
		rem_card -= 2;
		// if all the cards are paired, show the result
		if (!rem_card) {
			// change game status to stop updating timer
			game_status = SHOW_RES;
			// show result
			ShowResult();
			// terminate the game
			CloseWindow();
		}
	}
}

/*
	Callback function of back button.
*/
void GoBack(Widget w, void* data) {
	CloseWindow();
}

/*
	Initiate UI and the color of cards.
	Initiate correct game status.
*/
void StartGame(int x, int y) {
	int i, j, k;
	int* idx;
	int r, g, b;
	int card_wid, card_hei;

	// specify the number of cards
	table_row = x;
	table_col = y;

	// add "Back" button
	Widget back = MakeButton(" < Back ", GoBack, NULL);
	// add alignment label
	Widget empty = MakeLabel("                                        ");
	// add timer label
	timer = MakeLabel("Time      ");
	// organize "Back" button and timer
	SetWidgetPos(empty, PLACE_RIGHT, back, NO_CARE, NULL);
	SetWidgetPos(timer, PLACE_RIGHT, empty, NO_CARE, NULL);

	// allocate memory space for cards
	cards = (Card*)malloc(table_row * table_col * sizeof(Card));

	// calculate card size by the number of row and column
	card_wid = WINDOW_WID / table_col;
	card_hei = WINDOW_HEI / table_row;
	for (i = 0; i < table_row; i++) {
		for (j = 0; j < table_col; j++) {
			k = i * table_col + j;
			// create button for the card
			cards[k].w = MakeButton(NULL, Flip, (void*)(cards + k));
			// assign a bitmap to the button to specify its size
			SetWidgetBitmap(cards[k].w, BUTTON_LABEL, card_wid, card_hei);
			// organize the cards
			if (!i && !j) {
				// the first card is aligned with the "Back" button
				SetWidgetPos(cards[k].w, PLACE_UNDER, back, NO_CARE, NULL);
			} else
			if (j && !i) {
				// the first row is aligned with left card and the "Back" button
				SetWidgetPos(cards[k].w, PLACE_RIGHT, cards[k - 1].w, PLACE_UNDER, back);
			} else
			if (i && !j) {
				// the first column is aligned with the up card
				SetWidgetPos(cards[k].w, PLACE_UNDER, cards[k - table_col].w, NO_CARE, NULL);
			} else
			if (i && j) {
				// other cards are aligned with the up card and right card
				SetWidgetPos(cards[k].w, PLACE_RIGHT, cards[k - 1].w, PLACE_UNDER, cards[k - table_col].w);
			}
		}
	}

	// initiate the color
	// generate a permutation of card index(idx[])
	idx = (int*)malloc(table_row * table_col * sizeof(int));
	for (i = 0; i < table_row * table_col; i++) {
		idx[i] = i;
	}
	srand(time(0));
	for (i = 0; i < table_row * table_col; i++) {
		// randomize the idx[]
		k = rand() % (table_row * table_col - i) + i;
		j = idx[i];
		idx[i] = idx[k];
		idx[k] = j;

		// paint two card whose index is adjacent in idx[]
		if (!(i & 1)) {
			// randomize a new color every two round
			// which makes two card whose index being adjacent
			// share the same color
			r = rand() % 255;
			g = rand() % 255;
			b = rand() % 255;
		}
		// record color in card struct
		SetCardColor(cards + idx[i], r, g, b);
	}
	free(idx);

	// initiate status
	rem_card = table_col * table_row;
	game_status = NOT_START;

	ShowDisplay();
	MainLoop();

	// stop updating the timer
	game_status = CLOSING;

	// restore status
	CloseGame();
}