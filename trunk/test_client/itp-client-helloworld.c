/*
 * =====================================================================================
 *
 *       Filename:  itp-client-helloworld.c
 *
 *    Description:  Sample itp client for testing the servers
 *
 *        Version:  1.0
 *        Created:  03/04/2008 09:13:44 PM
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


#include "inputevent.h"
#include "mconnection.c"

#include <stdio.h> //perror

#define HOST "127.0.0.1"
#define PORT 5583

int main( int argc, char ** argv )
{

	int counter;
	char * msg = "hello world";

	InputEvent event;
	pInputEvent pEvent = &event;


	MConnection con;
	pMConnection pCon = &con;
	if ( init_connection( pCon, HOST, PORT ) < 0 )
	{
		perror( "Failed to establish connection to server!" );
		return -1;
	}

	//send each character in the message
	for( counter = 0; counter < strlen( msg ); counter++ )
	{
		event.event_t = EVENT_TYPE_KEY_DOWN;
		event.key_info.keycode = (int)msg[counter];
		sendEvent( &con, pEvent );
		
		event.event_t = EVENT_TYPE_KEY_UP;
		sendEvent( &con, pEvent );

	}

	//send ctrl-shift-esc
	/*
	event.event_t = EVENT_TYPE_KEY_DOWN;
	event.key_info.keycode = kKeyEscape;
	event.key_info.modifier = kModControl | kModShift;
	sendEvent( &con, pEvent );

	event.event_t = EVENT_TYPE_KEY_UP;
	sendEvent( &con, pEvent );
	*/

	close_connection( pCon );

	return 0;

}

