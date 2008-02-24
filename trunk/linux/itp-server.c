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

/*
    This file is part of iTouchpad.

    iTouchpad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    iTouchpad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with iTouchpad.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "mouseevent.h"

#define SCROLL_AMT 80
#define BUTTON_SCROLL_UP 5
#define BUTTON_SCROLL_DOWN 4


typedef int SOCKET;

int main( int argc, char ** argv)
{
	MouseEvent event;
	pMouseEvent pEvent = &event;

	Display	*dpy; /* X server connection */
	int xtest_major_version = 0;
	int xtest_minor_version = 0;
	int dummy;

	SOCKET s, s_accept;
	struct sockaddr_in s_add; //from anyone!
	struct sockaddr s_client;
	socklen_t s_client_size = sizeof( struct sockaddr );
	int port = PORT;
	int recvsize;

	int button, yDelta = 0, yTmp;

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

//network stuff
	//configure socket
	if ( ( s = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) 
	{
		perror ( "Failed to create socket :(" ); 
		exit( 2 );

	}

	memset( &s_add, 0, sizeof( struct sockaddr_in ) );
	s_add.sin_family = AF_INET;
	s_add.sin_port = htons( port );
	s_add.sin_addr.s_addr = INADDR_ANY;

	if ( bind( s, (struct sockaddr * )&s_add, sizeof( struct sockaddr_in ) ) == -1 )
	{
		perror( "Failed to bind socket" );
		exit( 2 );
	}

	if( listen( s , 1 ) )
	{
		perror( "Can't listen!" );
		exit( 2 );
	}

	while( 1 )
	{

		s_accept = accept( s, &s_client, &s_client_size );

		if ( s_accept == -1 )
		{
			perror( "failed to accept!" );
			return -1;
		}

		while( 1 )
		{
			recvsize = recv( s_accept, pEvent, sizeof( MouseEvent ), MSG_WAITALL );
			if ( recvsize == sizeof( MouseEvent ) )//got data
			{

				switch( pEvent->event_t )
				{
					case EVENT_TYPE_MOUSE_MOVE:
					//	printf( "move: %d, %d!\n", pEvent->move_info.dx, pEvent->move_info.dy );
						XTestFakeRelativeMotionEvent( dpy, pEvent->move_info.dx, pEvent->move_info.dy, 0 );
						break;
					case EVENT_TYPE_MOUSE_SCROLL_MOVE:
						//no x-scrolling :-/
						yDelta += pEvent->move_info.dy;
						if ( yDelta < 0 )//down
						{
							button = BUTTON_SCROLL_DOWN;
							yTmp = - yDelta;
						}
						else
						{
							button = BUTTON_SCROLL_UP;
							yTmp = yDelta;
						}

						// send as many clicks as necessary (ty synergy for this)
						for( ; yTmp >= SCROLL_AMT; yTmp -= SCROLL_AMT )
						{
							XTestFakeButtonEvent( dpy, button, 1, 0 );
							XTestFakeButtonEvent( dpy, button, 0, 0 );
						}

						//fix yTmp:
						if ( yDelta < 0 )//we were scrolling down
						{
							yDelta = -yTmp;
						}
						else
						{
							yDelta = yTmp;
						}

						break;

					case EVENT_TYPE_MOUSE_DOWN:
						//printf( "mouse down: %d", pEvent->button_info.button );
						XTestFakeButtonEvent( dpy, pEvent->button_info.button, 1, 0 );
						break;

					case EVENT_TYPE_MOUSE_UP:	
						//printf( "mouse up: %d", pEvent->button_info.button );
						XTestFakeButtonEvent( dpy, pEvent->button_info.button, 0, 0 );
						break;

					default:
						fprintf( stderr, "unknown message type: %d\n", pEvent->event_t );
						break;
				}

				XFlush( dpy );
			
			}
			else if ( recvsize > 0 )
			{
				fprintf( stderr, "partial recv!" );
			}
			else if ( recvsize == 0 )
			{
				//connection terminated
				close( s_accept );
				break; //exit this while loop, wait for another connection
			}
			else
			{
				perror( "error in recv" );
			}
		}

	}

	//shouldn't get here!

	return 0;
}

