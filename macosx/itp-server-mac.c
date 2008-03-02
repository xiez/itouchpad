/*
 * =====================================================================================
 *
 *	  Filename:  itp-server-mac.c
 *
 *    Description:  itp-server --listens for a itp client and accepts mouse requests..
 *
 *	   Version:  1.0
 *	   Created:  03/1/2008 03:06:18 PM
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
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include "mouseevent.h"

#define SCROLL_AMT 10
#define BUTTON_SCROLL_UP 5
#define BUTTON_SCROLL_DOWN 4


typedef int SOCKET;

int main( int argc, char ** argv)
{
	CGPoint newloc;
	Point pt;
	MouseEvent event;
	pMouseEvent pEvent = &event;

	SOCKET s, s_accept;
	struct sockaddr_in s_add; //from anyone!
	struct sockaddr s_client;
	socklen_t s_client_size = sizeof( struct sockaddr );
	int port = PORT;
	int recvsize;

	int xDelta=0, yDelta=0;

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
						printf("Mouse move\n");
						fflush( stdout );
						GetGlobalMouse( &pt );//get cursor pos
						//update x/y
						newloc.x = pt.h + pEvent->move_info.dx;
						newloc.y = pt.v + pEvent->move_info.dy;

						CGPostMouseEvent( newloc, true /*yes move there*/, 0 , false); 

						break;
					case EVENT_TYPE_MOUSE_SCROLL_MOVE:
						printf( "Scrolling\n" );
						fflush( stdout );
						xDelta += pEvent->move_info.dx;
						yDelta += pEvent->move_info.dy;

						//TODO: mac osx has a api to get the exact scroll amount, look into that
						if ( ( yDelta/SCROLL_AMT != 0 )  || ( xDelta/SCROLL_AMT != 0 ) )
						{//if any clicks need to be made...
							
							//send the number of whole clicks....
							CGPostScrollWheelEvent( 2, -yDelta/SCROLL_AMT, -xDelta/SCROLL_AMT ); 

							//remove them from our counter
							xDelta %=SCROLL_AMT;
							yDelta %=SCROLL_AMT;
						}

						break;
					case EVENT_TYPE_MOUSE_DOWN:
						//printf( "mouse down: %d", pEvent->button_info.button );
						//TODO: MOUSE DOWN!


						break;

					case EVENT_TYPE_MOUSE_UP:	
						//printf( "mouse up: %d", pEvent->button_info.button );
						//TODO: MOUSE UP!

						break;

					default:
						fprintf( stderr, "unknown message type: %d\n", pEvent->event_t );
						fflush( stdout );
						break;
				}

				//flush events, if needed in macosx
			
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

