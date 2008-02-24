/*
 * =====================================================================================
 *
 *       Filename:  itp-client.c
 *
 *    Description:  Sample itp client for testing the servers
 *
 *        Version:  1.0
 *        Created:  02/19/2008 01:02:21 PM
 *
 *         Author:  Will Dietz (WD), wdietz2@uiuc.edu
 *        Company:  dtzTech
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


#include "mouseevent.h"
#include "mconnection.c"

#include <stdio.h> //perror

#define HOST "192.168.255.1"
#define PORT 5583

int main( int argc, char ** argv )
{

	int counter;

	MouseEvent event;
	pMouseEvent pEvent = &event;


	MConnection con;
	pMConnection pCon = &con;
	if ( init_connection( pCon, HOST, PORT ) < 0 )
	{
		perror( "Failed to establish connection to server!" );
		return -1;
	}

	event.event_t = EVENT_TYPE_MOUSE_DOWN;
	event.button_info.button = BUTTON_LEFT;
	sendEvent( &con, pEvent );

	sleep( 1 );

	//move down and to the right 100
	event.event_t = EVENT_TYPE_MOUSE_MOVE;
	event.move_info.dx = 1;
	event.move_info.dy = 1;

	for( counter = 0; counter < 100; counter++ )
	{
		if( sendEvent( &con, pEvent ) < 0 )
		{
			printf( "error!" );
		}
	}

	sleep( 1 );

	event.event_t = EVENT_TYPE_MOUSE_UP;
	event.button_info.button = BUTTON_LEFT;
	sendEvent( &con, pEvent );


	close_connection( pCon );


	return 0;

}

