/* This file contains all the header definitions for working with this
 * library of functions that make X stuff a lot easier to swallow.
 *
 *              --  This code under the GNU copyleft --
 * 
 *   Dominic Giampaolo
 *   dbg@sgi.com
 */

#ifndef _LIBSX_H_    /* prevent accidental re-inclusions */
#define _LIBSX_H_  

#ifdef __cplusplus   /* be friendly to people using C++ */
 extern "C" {
#endif


#include <X11/Intrinsic.h>

#define SX_SHAREDIR "/usr/share/libsx"

/*
 * Predefined Args
 */
extern char **PredefArgs;


/*
 * Version and author information
 */

char *LibVersion();
char *Author();

/*
 * General prototypes for the setup functions
 */
void   ReadLocale(char *language);
int    OpenDisplay(int argc, char **argv);
int    GLOpenDisplay(int argc, char **argv, int *attributes);
void   ShowDisplay(void);

void   MainLoop(void);
void   CheckForEvent(void);
void   SyncDisplay(void);


Widget MakeWindow(char *window_name, char *display_name, int exclusive);
void   SetCurrentWindow(Widget w);
void   CloseWindow(void);
void   ExitMainLoop(void);  /* only call after CloseWindow() */

/* #define's for use with MakeWindow() */
#define SAME_DISPLAY         NULL
#define NONEXCLUSIVE_WINDOW  0
#define EXCLUSIVE_WINDOW     1

  
/* #define for use with SetCurrentWindow() */
#define ORIGINAL_WINDOW  NULL


Widget GetTopWidget(Widget w);

Widget MakeForm(Widget parent);
Widget GetForm(void);
void   SetForm(Widget form);

/* for use w/MakeForm() and SetForm() */
#define TOP_LEVEL_FORM  NULL


/*
 * These are typedef's for the various styles of callback functions.
 */
typedef void (*ButtonCB)(Widget w, void *data);
typedef void (*StringCB)(Widget w, char *string, void *data);
typedef void (*ScrollCB)(Widget w, float new_val, void *data);
typedef void (*ListCB)(Widget w, char *string, int index, void *data);
typedef void (*FreqCB)(Widget w, char *dir_name, char *file_name, void *data);


/*
 * These typedef's are for drawing area callbacks only.
 */
typedef void (*RedisplayCB)(Widget w, int new_width, int new_height, void *d);
typedef void (*MouseButtonCB)(Widget w, int button, int x, int y, void *dat);
typedef void (*KeyCB)(Widget w, char *input, int up_or_down, void *data);
typedef void (*MotionCB)(Widget w, int x, int y, void *data);
typedef void (*EnterCB)(Widget w, int x, int y, void *data);
typedef void (*LeaveCB)(Widget w, int x, int y, void *data);



/*
 * Prototypes for the widget creation functions.  General functions
 * that apply to any widget (such as setting its color or position) follow
 * after these.
 */


/*
 * Button and Label Widget routines.
 */
Widget MakeButton(char *label, ButtonCB function, void *data); 
Widget MakeLabel(char *txt);


/*
 * Toggle Widget routines.
 */
Widget MakeToggle(char *txt, int state, Widget w, ButtonCB func, void *d);
void   SetToggleState(Widget w, int state);
int    GetToggleState(Widget w);



/*
 * Drawing Area and drawing functions.
 */

Widget MakeDrawArea(int width, int height, RedisplayCB redisplay, void *data);

void   SetButtonDownCB(Widget w, MouseButtonCB button_down);
void   SetButtonUpCB(Widget w, MouseButtonCB button_up);
void   SetKeypressCB(Widget w, KeyCB keypress);
void   SetMouseMotionCB(Widget w, MotionCB motion);
void   SetEnterCB(Widget w, EnterCB enter);
void   SetLeaveCB(Widget w, LeaveCB leave);

int    TurnOnBackingStore(Widget w);
void   SetColor(int color);
void   SetDrawMode(int mode);

#define SANE_XOR  0x7f  /* A sane mode for drawing XOR lines and stuff */

void   SetLineStyle(int style); /* LineSolid, LineOnOffDash, LineDoubleDash */
void   SetLineWidth(int width);
void   SetDrawArea(Widget w);
void   GetDrawAreaSize(int *w, int *h);

void   ClearDrawArea(void);

void   DrawPixel(int x1, int y1);
int    GetPixel(int x1, int y1);
void   DrawLine(int x1, int y1, int x2, int y2);
void   DrawPolyline(XPoint *points, int n);
void   DrawFilledPolygon (XPoint *points, int n);
void   DrawFilledBox(int x, int y, int width, int height);
void   DrawBox(int x, int y, int width, int height);
void   DrawText(char *string, int x, int y);
void   DrawArc(int x, int y, int width, int height, int angle1, int angle2);
void   DrawFilledArc(int x, int y, int w, int h, int angle1, int angle2);
void   DrawImage(char *data, int x, int y, int width, int height);
void   DrawBitmap(char *data, int x, int y, int width, int height);
void   GetImage(char *data, int x, int y, int width, int height);

void   ScrollDrawArea(int dx, int dy, int x1,int y1, int x2, int y2);

void   SwapBuffers(void);  /* only if libsx compiled with -DOPENGL_SUPPORT */

char  *GrabPixel(char *format);


/*
 * String Entry routines.
 */
Widget  MakeStringEntry(char *txt, int size, StringCB func, void *data);
void    SetStringEntry(Widget w, char *new_text);
char   *GetStringEntry(Widget w);


/*
 * Ascii Text display widget routines.
 */
Widget  MakeTextWidget(char *txt, int is_file, int editable, int w, int h);
void    SetTextEditable(Widget w, int can_edit);
void    SetTextWidgetText(Widget w, char *text, int is_file);
char   *GetTextWidgetText(Widget w);
void    SetTextWidgetPosition(Widget w, int text_pos);

/*
 * File requestor
 */
char *GetFile(char *_label, char *_path, FreqCB _func, void *_data);
void SetFreqFilter(char *filter);

/* defines for the is_file argument */
#define EDIT_FILE      1
#define EDIT_STRING    0

/* defines for the editable argument to MakeTextWidget() */
#define CAN_MODIFY     1
#define NO_MODIFY      0


/*
 * Scrollbar routines.
 */
Widget MakeHorizScrollbar(int len,    ScrollCB scroll_func, void *data);
Widget MakeVertScrollbar( int height, ScrollCB scroll_func, void *data);
void   SetScrollbar(Widget w, float where, float max, float size_shown);
void   SetScrollbarStep(Widget w, float fact);
void   SetThumbBitmap(Widget w, char *black_bits, int width, int height);
void   SetScrollbarDirection(float dir);

/*
 * Scrolled list routines.
 */
Widget MakeScrollList(char **list, int width, int height, ListCB func,void *d);
void   SetCurrentListItem(Widget w, int list_index);
int    GetCurrentListItem(Widget w);
void   ChangeScrollList(Widget w, char **new_list);



/*
 * Menu and MenuItem routines.
 */
Widget MakeMenu(char *name);
Widget MakeMenuItem(Widget menu, char *name, ButtonCB func, void *arg);

void   SetMenuItemChecked(Widget w, int state);
int    GetMenuItemChecked(Widget w);




/*
 * Widget position setting functions (used to do algorithmic layout).
 */
void  SetWidgetPos(Widget w, int where1, Widget from1,int where2,Widget from2);

/*
 * define's for button/gadget placement, used to call SetWidgetPos()
 */
#define NO_CARE       0x00 /* don't care where the gadget is placed */
#define PLACE_RIGHT   0x01 /* place me to the right of specified gadget */
#define PLACE_UNDER   0x02 /* place me under the specified gadget */



void AttachEdge(Widget w, int edge, int attach_to);

#define LEFT_EDGE      0x00   /* These #define's specify which edge we want */
#define RIGHT_EDGE     0x01
#define TOP_EDGE       0x02
#define BOTTOM_EDGE    0x03


#define ATTACH_LEFT    0x00   /* attach given edge to the left side of form */
#define ATTACH_RIGHT   0x01   /* attach given edge to the right side of form */
#define ATTACH_TOP     0x02   /* attach given edge to the top of the form */
#define ATTACH_BOTTOM  0x03   /* attach given edge to the bottom of the form */



/*
 * General Setting/Getting of Widget attributes.  These apply to any
 * type of widget.
 */
void  SetWindowTitle(char *title);
void  SetIconTitle(char *title);
void  SetFgColor(Widget w, int color);
void  SetBgColor(Widget w, int color);
void  SetBorderColor(Widget w, int color);

int   GetFgColor(Widget w);
int   GetBgColor(Widget w);

void  SetLabel(Widget w, char *txt);

void  SetWidgetState(Widget w, int state);    /* turn widgets on and off */
int   GetWidgetState(Widget w);

void  SetWidgetBitmap(Widget w, char *data, int width, int height);
#define XPM_SUPPORT
#ifdef XPM_SUPPORT
void SetWidgetPixmap(Widget w, char **xpmdata);
#endif

void  SetWidgetSize(Widget w, int width, int height);

void  Beep(void);


/*
 * Font things.
 */
typedef XFontStruct *XFont;     /* make it a little easier to read */

XFont GetFont(char *fontname);
void  SetWidgetFont(Widget w, XFont f);
XFont GetWidgetFont(Widget w);
void  FreeFont(XFont f);
int   FontHeight(XFont f);
int   TextWidth(XFont f, char *txt);



/*
 * Miscellaneous functions.
 */
typedef void (*GeneralCB)(void *data);
typedef void (*IOCallback)(void *data, int *fd);
     
void  AddTimeOut(unsigned long interval, GeneralCB func, void *data);
void  AddReadCallback(int fd,  IOCallback func, void *data);
void  AddWriteCallback(int fd, IOCallback func, void *data);


/*
 * User-input functions 
 */
char *GetString(char *blurb, char *default_string);
char *GetLongString(char *blurb, char *default_string, int width);
char *GetText(char *blurb, char *default_string, int width, int height);
int   GetYesNo(char *question);
int   GetOkay(char *blurb);
int   GetTriState(char *blurb);


/*
 *  Labels
 */
#define NUM_DIAL 28
extern char *SX_Dialog[NUM_DIAL];
#define MAIN_DIAL 0
#define POPUP_DIAL 6
#define COLSEL_DIAL 9
#define FREQ_DIAL 17


/*
 * Colormap things.
 */

extern int WHITE,        /* Global color values to use for drawing in color */
           BLACK,
           RED, 
           GREEN, 
           BLUE,
           YELLOW,
           HILIGHT,
           BUTTONBG,
           INPUTBG;


/*
 * Getting/Setting/Freeing Color and Colormap function prototypes
 */
void GetStandardColors(void);
int  GetNamedColor(char *name);
int  GetRGBColor(int r, int g, int b);
void FreeStandardColors(void);


int  GetPrivateColor(void);
void SetPrivateColor(int which, int r, int g, int b);
void FreePrivateColor(int which);


/*
 * The following functions completely take over the display colormap.
 *                       ** Use with caution **
 */
int  GetAllColors(void);
void SetColorMap(int num);
void SetMyColorMap(int n, unsigned char *r, unsigned char *g,unsigned char *b);
void FreeAllColors(void);

/*
 * Prototype of data for color selector widget
 */

typedef struct {
  void *data;
  Widget csel_window;
  Widget lmode, lcolor, draw, scroll_list;
  Widget red_label, red_string, red_scroll;
  Widget green_label, green_string, green_scroll;
  Widget blue_label, blue_string, blue_scroll;  
  Widget black_label, black_string, black_scroll;  

  int color;
  int mode;
  int cancelled;
  int output;
  float r, g, b;
  float h, s, v;
  float c, m, y, k;
  char rgb_list[1000][60];
  char *rgb_ptr[1000];
  char match_list[1000][80];
  char *match_ptr[1000];
  char save[40];
} CSelData;

typedef void (*CSelCB)(Widget w, CSelData *data);

char *
SelectColor(char *inicolor, int output, char *txt, CSelCB func, void *data);

/*
 * define's for use in calling SetColorMap()
 */
#define GREY_SCALE_1    0   /* grey-scale with a few other colors */
#define GREY_SCALE_2    1   /* pure grey-scale (0-255) */
#define RAINBOW_1       2   /* different types of rainbows/bands of colors */
#define RAINBOW_2       3


#ifdef __cplusplus
 }
#endif


#endif /* _LIBSX_H_ */
