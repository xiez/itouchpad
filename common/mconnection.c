/*
 * =====================================================================================
 *
 *       Filename:  mconnection.c
 *
 *    Description:  Common connection code
 *
 *        Version:  1.0
 *        Created:  02/19/2008 01:36:48 PM
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

#include "mconnection.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h> //close
#include <string.h> //memset

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_connection
 *  Description:  Attempts to connect to a itp-server at the specified hostname and port
 *                Fills in pCon with the needed state information for use elsewhere
 *      Returns:  0 on success
 * =====================================================================================
 */
int init_connection( pMConnection pCon, char * server, int port ) 
{
	//makes assumptions about pCon, etc, being valid.
	//will probably die if you give it bad parameters 
	
	int sockd;

	if ( ( sockd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
	{
		return -1;
	}

	memset( &(pCon->s_add), 0, sizeof( struct sockaddr_in ) );
	pCon->s_add.sin_family = AF_INET;
	pCon->s_add.sin_port = htons( port );
	pCon->s_add.sin_addr.s_addr = inet_addr( server );//must be an IP!

	if ( connect( sockd, (struct sockaddr *)&(pCon->s_add), sizeof( struct sockaddr_in) ) < 0 )
	{
		return -1;
	}

	pCon->sockd = sockd;

	return 0;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendEvent
 *  Description:  sends the specified event using the specified connection.  Blocking.
 * =====================================================================================
 */
int sendEvent( pMConnection pCon, pMouseEvent pEvent )
{
	size_t size = sizeof( MouseEvent );

	if ( send( pCon->sockd, pEvent, size, 0 ) != size )
	{
		return -1;
	}

	return 0;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  close_connection
 *  Description:  cleans up after the connection, closing the socket, etc
 * =====================================================================================
 */
void close_connection( pMConnection pCon )
{
	close( pCon->sockd );
}


