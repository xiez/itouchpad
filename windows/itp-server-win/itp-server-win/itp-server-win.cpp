/*
 * =====================================================================================
 *
 *	  Filename:  itp-server-win.cpp
 *
 *    Description:  itp-server --listens for a itp client and accepts mouse requests..
 *
 *	   Version:  1.0
 *	   Created:  02/21/2008 05:01AM
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

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>

#include "../../../common/inputevent.h"

#define USE_ACCEL 1

//dang windows lack of compliance
#ifndef MSG_WAITALL
//#define MSG_WAITALL 0x08
//we define it as zero, since I can't get it to work.
#define MSG_WAITALL 0
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	InputEvent event;
	pInputEvent pEvent = &event;
	//POINT pt;
	
	SOCKET s, s_accept;
	struct sockaddr_in s_add; //from anyone!
	struct sockaddr s_client;
	int s_client_size = sizeof( struct sockaddr );
	int port = PORT;
	int recvsize;

	//mouse-relative move:
	int oldSpeed[4];
	bool accelChanged;


//network stuff
	//WSA \o/
	WSADATA WsaDat;
	if (WSAStartup(MAKEWORD(1, 1), &WsaDat) != 0)
	{
		perror("WSA Initialization failed.");
	} 

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
			recvsize = recv( s_accept, (char *)pEvent, sizeof( InputEvent ), MSG_WAITALL );
			if ( recvsize == sizeof( InputEvent ) )//got data
			{
				switch( pEvent->event_t )
				{
					case EVENT_TYPE_MOUSE_MOVE:
//						GetCursorPos( &pt );
//						SetCursorPos( pt.x + pEvent->move_info.dx, pt.y + pEvent->move_info.dy );
						//the mouse-accel related code is from synergy, ty
						// save mouse speed & acceleration
						#ifdef USE_ACCEL
						oldSpeed[4];
						accelChanged =
									SystemParametersInfo(SPI_GETMOUSE,0, oldSpeed, 0) &&
									SystemParametersInfo(SPI_GETMOUSESPEED, 0, oldSpeed + 3, 0);
					
						// use 1:1 motion
						if (accelChanged) {
							int newSpeed[4] = { 0, 0, 0, 1 };
							accelChanged =
									SystemParametersInfo(SPI_SETMOUSE, 0, newSpeed, 0) ||
									SystemParametersInfo(SPI_SETMOUSESPEED, 0, newSpeed + 3, 0);
						}
					
						// move relative to mouse position
						mouse_event(MOUSEEVENTF_MOVE, pEvent->move_info.dx, pEvent->move_info.dy, 0, 0);
					
						// restore mouse speed & acceleration
						if (accelChanged) {
							SystemParametersInfo(SPI_SETMOUSE, 0, oldSpeed, 0);
							SystemParametersInfo(SPI_SETMOUSESPEED, 0, oldSpeed + 3, 0);
						}
						#else
							mouse_event( MOUSEEVENTF_MOVE, pEvent->move_info.dx, pEvent->move_info.dy, 0, 0 );
						#endif

						break;
					case EVENT_TYPE_MOUSE_SCROLL_MOVE:
						mouse_event( MOUSEEVENTF_WHEEL, 0, 0, -pEvent->move_info.dy, 0 );
						//HWHEEL doesn't seem to work?
						mouse_event( MOUSEEVENTF_HWHEEL, 0, 0, pEvent->move_info.dx, 0 );

						break;
					
						//NOTE: this assumes the mouse events are lbutton. fine for now, but needs to change!
					case EVENT_TYPE_MOUSE_DOWN:
						mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
						break;

					case EVENT_TYPE_MOUSE_UP:	
						mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
						break;

					default:
						fprintf( stderr, "unknown message type: %d\n", pEvent->event_t );
						break;
				}

				//XFlush( dpy );
			
			}
			else if ( recvsize > 0 )
			{
				fprintf( stderr, "partial recv!" );
			}
			else if ( recvsize == 0 )
			{
				//connection terminated
				closesocket( s_accept );
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

