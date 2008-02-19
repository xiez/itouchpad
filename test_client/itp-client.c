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

#include "mouseevent.h"
#include "mconnection.c"

#include <stdio.h> //perror

#define HOST "127.0.0.1"
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


	//move down and to the right 100
	event.event_t = EVENT_TYPE_MOUSE_MOVE;
	event.move_info.dx = 1;
	event.move_info.dy = 1;


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

	event.event_t = EVENT_TYPE_MOUSE_DOWN;
	event.button_info.button = BUTTON_LEFT;
	sendEvent( &con, pEvent );

	event.event_t = EVENT_TYPE_MOUSE_UP;
	sendEvent( &con, pEvent );
	sendEvent( &con, pEvent );


	close_connection( pCon );


	return 0;

}

