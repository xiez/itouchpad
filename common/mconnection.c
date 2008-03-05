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
#include <netinet/tcp.h> //TCP_NODELAY
#include <sys/socket.h>
#include <unistd.h> //close
#include <string.h> //memset
#include <fcntl.h>
#include <errno.h>

#define CONNECT_TIMEOUT 5

#define USE_NAGLE_DELAY 0

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_connection
 *  Description:  Attempts to connect to a itp-server at the specified hostname and port
 *                Fills in pCon with the needed state information for use elsewhere
 *                BLOCKING.
 *      Returns:  0 on success
 * =====================================================================================
 */
int init_connection( pMConnection pCon, const char * serverip, int port ) 
{
	//makes assumptions about pCon, etc, being valid.
	//will probably die if you give it bad parameters 
	
	int sockd;
	fd_set fdset; 
	struct timeval tv;
	int arg, res, optval;
	unsigned int optlen = sizeof( int );
	int dont_use_nagle = ! USE_NAGLE_DELAY;


	if ( ( sockd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
	{
		return -1;
	}

	//non-blocking code
	if ( ( arg = fcntl( sockd, F_GETFL, NULL ) ) < 0 )
	{
		close( sockd );
		return -2;
	}

	arg |= O_NONBLOCK; 
	
	if( fcntl( sockd, F_SETFL, arg ) < 0 )
	{
		close( sockd );
		return -2;
	} 

	if ( setsockopt( sockd, IPPROTO_TCP, TCP_NODELAY, (char *)&dont_use_nagle, sizeof( dont_use_nagle ) ) == -1 )
	{
		close ( sockd );
		return -2;
	}

	memset( &(pCon->s_add), 0, sizeof( struct sockaddr_in ) );
	pCon->s_add.sin_family = AF_INET;
	pCon->s_add.sin_port = htons( port );
	pCon->s_add.sin_addr.s_addr = inet_addr( serverip );//must be an IP!

	res = connect( sockd, (struct sockaddr *)&(pCon->s_add), sizeof( struct sockaddr_in) );
	if ( res < 0 )
	{
		if ( errno == EINPROGRESS )
		{//it's not blocking like we told it (not) to....
			tv.tv_sec = CONNECT_TIMEOUT;
			tv.tv_usec = 0;
			FD_ZERO( &fdset );
			FD_SET( sockd, &fdset );
			res = select( sockd + 1, NULL, &fdset, NULL, &tv );//wait until connect finishes or timeout

			if ( res < 0 && errno != EINTR )
			{
				close( sockd );
				return -3;//error connecting
			}
			else if ( res == 0 )
			{
				close( sockd );
				return -4;//timeout :(
			}
			//res > 0

			//make sure things went smoothly...

			if ( getsockopt( sockd, SOL_SOCKET, SO_ERROR, (void*)(&optval), &optlen ) < 0 ) 
			{
				close( sockd );
				return -3;//error in getsockopt, treat as connect error
			}

			if ( optval )
			{
				//connect didn't go so well
				close( sockd );
				return -3;
			}
		}
		else
		{
			//not the errorcode we expected..so error
			close( sockd );
			return -3;
		}
	}

	//back to blocking since that's what we want
	if ( ( arg = fcntl( sockd, F_GETFL, NULL ) ) < 0 )
	{
		close( sockd );
		return -2;
	}

	arg &= ~O_NONBLOCK;//make it blocking again..

	if ( fcntl( sockd, F_SETFL, arg ) < 0 )
	{
		close( sockd );
		return -2;
	}

	//yay done


	pCon->sockd = sockd;

	return 0;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendEvent
 *  Description:  sends the specified event using the specified connection.  Blocking.
 * =====================================================================================
 */
int sendEvent( pMConnection pCon, pInputEvent pEvent )
{
	/* TODO:
	 * Instead of sending all the deltas as they occur, just add the delta to a running total, and
	 * send that delta on a timer/select.  This way you don't end up spamming the network with
	 * packets to achieve a granularity that isn't necessary.
	 * Might make the experience smoother, worth investigating.
	 */
	size_t size = sizeof( InputEvent );

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


