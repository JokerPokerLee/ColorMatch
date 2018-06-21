#include <stdio.h>
#include <stdlib.h>

#include "libsx.h"

#include "game.h"
#include "board.h"
#include "setting.h"

int set_row = 4, set_col = 4;

void BuildGame(Widget w, void *data) {
	// create a window for game
	Widget gameWindow = MakeWindow("Game", SAME_DISPLAY, 1);
	StartGame(set_row, set_col);
}

void BuildBoard(Widget w, void *data) {
	// create a window for demonstrate leaderboard
	Widget boardWindow = MakeWindow("Leaderboard", SAME_DISPLAY, 1);
	StartBoard();
}

void BuildSetting(Widget w, void *data) {
	// create a window for setting
	Widget settingWindow = MakeWindow("Setting", SAME_DISPLAY, 1);
	StartSetting(&set_row, &set_col);
	SetSizeInBoard(set_row, set_col);
}

void Quit(Widget w, void *data) {
	CloseWindow();
}

void BuildMain() {
	// create a window for main
	Widget mainWindow = MakeWindow("ColorMatch", SAME_DISPLAY, 1);
	// create buttons for game, leaderboard, setting and quit
	Widget start     = MakeButton("   Start Game   ", BuildGame, &mainWindow);
	Widget leadboard = MakeButton("  Leader Board  ", BuildBoard, NULL);
	Widget setting   = MakeButton("    Setting     ", BuildSetting, NULL);
	Widget quit      = MakeButton("      Quit      ", Quit, NULL);
	// organize the buttons
	SetWidgetPos(leadboard, PLACE_UNDER, start, NO_CARE, NULL);
	SetWidgetPos(setting, PLACE_UNDER, leadboard, NO_CARE, NULL);
	SetWidgetPos(quit, PLACE_UNDER, setting, NO_CARE, NULL);
}

int main(int argc, char **argv) {

	argc = OpenDisplay(argc, argv);
	if (argc == 0) {
		exit(-1);
	}

	SetSizeInBoard(set_row, set_col);
	BuildMain();

	ShowDisplay();
	MainLoop();

	return 0;
}