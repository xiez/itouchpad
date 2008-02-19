/*
 * =====================================================================================
 *
 *	  Filename:  itp-server.c
 *
 *    Description:  itp-server --listens for a itp client and accepts mouse requests..
 *
 *	   Version:  1.0
 *	   Created:  02/18/2008 09:18:12 PM
 *
 *	    Author:  Will Dietz (WD), wdietz2@uiuc.edu
 *	   Company:  dtzTech
 *
 * =====================================================================================
 */

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#define CURSOR_SPEED 10

int main( int argc, char ** argv)
{
	//NOTES:
	//Current code simply accepts arrowkey input and moves the mouse
	//Network code, etc, to come shortly.
	


	Display	*dpy; /* X server connection */
	int xtest_major_version = 0;
	int xtest_minor_version = 0;
	int dummy;

	int inputKey;

	int dx, dy;

	/*
	 * curses init stuff... is this all really necessary for getch?
	 */
	initscr();	
	keypad(stdscr, TRUE); /* enable keyboard mapping */
	nonl(); /* tell curses not to do NL->CR/NL on output */
	cbreak(); /* take input chars one at a time, no wait for \n */
	noecho(); /* don't echo input */



    /*
	* Open the display using the $DISPLAY environment variable to locate
	* the X server.  See Section 2.1.
	*/
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
	   fprintf(stderr, "%s: can't open %s\en", argv[0], XDisplayName(NULL));
	   exit(1);
    }

    Bool success = XTestQueryExtension(dpy, &dummy, &dummy,
&xtest_major_version, &xtest_minor_version);
    if(success == False || xtest_major_version < 2 ||
(xtest_major_version <= 2 && xtest_minor_version < 2))
    {
	   fprintf(stderr,"XTEST extension not supported");
	   exit(1);
    }

	while( ( inputKey = getch() ) )
	{
		dx = dy = 0;
		switch ( inputKey )
		{
			case KEY_UP:
				dx = 0;
				dy = -CURSOR_SPEED;
				break;
			case KEY_DOWN:
				dx = 0;
				dy = CURSOR_SPEED;
				break;
			case KEY_LEFT:
				dx = -CURSOR_SPEED;
				dy = 0;
				break;
			case KEY_RIGHT:
				dx = CURSOR_SPEED;
				dy = 0;

			default:
//				printf( "unknown!" );
				break;
		}

		XTestFakeRelativeMotionEvent( dpy, dx, dy, 0 ); 
		XFlush( dpy );

	}

	return 0;
}

