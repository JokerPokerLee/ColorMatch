#include <stdlib.h>

#include "libsx.h"

int **ROW, **COL;
Widget hint;

void PrintHint(char* msg) {
	SetLabel(hint, msg);
}

void SetXY(Widget w, void *data) {
	Widget *val = (Widget*)data;
	Widget xstr = val[0];
	Widget ystr = val[1];
	if (strlen(GetStringEntry(xstr)) > 5 || strlen(GetStringEntry(ystr)) > 5) {
		PrintHint("Both number should be less than 20.");
		return;
	}
	char** rem;
	int tx, ty;
	tx = strtol(GetStringEntry(xstr), rem, 10);
	if (**rem) {
		PrintHint("  Numbers contain illegal digits.  ");
		return;
	}
	ty = strtol(GetStringEntry(ystr), rem, 10);
	if (**rem) {
		PrintHint("  Numbers contain illegal digits.  ");
		return;
	}
	if (tx > 20 || ty > 20) {
		PrintHint("Both number should be less than 20.");
		return;
	}
	if (tx < 1 && ty < 1) {
		PrintHint("Both number should be more than 0. ");
		return;
	}
	if ((tx & 1) && (ty & 1)) {
		ty++;
	}
	**ROW = tx, **COL = ty;
	CloseWindow();
}

void Set(Widget w, char *val, void *data) {

}

void StartSetting(int* x, int* y) {
	ROW = &x, COL = &y;
	**ROW = 4, **COL = 4;
	Widget xl = MakeLabel("    X:  ");
	Widget yl = MakeLabel("    Y:  ");
	Widget val[2];
	val[0] = MakeStringEntry(NULL, 130, Set, NULL);
	val[1] = MakeStringEntry(NULL, 130, Set, NULL);
	SetWidgetPos(val[0], PLACE_RIGHT, xl, NO_CARE, NULL);
	SetWidgetPos(yl, PLACE_UNDER, xl, NO_CARE, NULL);
	SetWidgetPos(val[1], PLACE_RIGHT, yl, PLACE_UNDER, val[0]);
	hint = MakeLabel("                                   ");
	SetWidgetPos(hint, PLACE_UNDER, yl, PLACE_UNDER, val[1]);
	Widget empty = MakeLabel("            ");
	Widget comfirm = MakeButton("Set", SetXY, &val[0]);
	SetWidgetPos(empty, PLACE_UNDER, hint, NO_CARE, NULL);
	SetWidgetPos(comfirm, PLACE_RIGHT, empty, PLACE_UNDER, hint);
	ShowDisplay();
	MainLoop();
}