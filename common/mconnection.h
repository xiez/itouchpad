/*
 * =====================================================================================
 *
 *       Filename:  mconnection.h
 *
 *    Description:  Connection struct, and methods to connect to a itp server
 *
 *        Version:  1.0
 *        Created:  02/19/2008 01:08:03 PM
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

#ifndef  MCONNECTION_H_INC
#define  MCONNECTION_H_INC


#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "mouseevent.h"

typedef struct
{
	int sockd;
	struct sockaddr_in s_add;

} MConnection, * pMConnection;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_connection
 *  Description:  Attempts to connect to a itp-server at the specified hostname and port
 *                Fills in pCon with the needed state information for use elsewhere
 *      Returns:  0 on success
 * =====================================================================================
 */
int init_connection( pMConnection pCon, char * server, int port ); 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendEvent
 *  Description:  sends the specified event using the specified connection.  Blocking.
 * =====================================================================================
 */
int sendEvent( pMConnection pCon, pMouseEvent pEvent );


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  close_connection
 *  Description:  cleans up after the connection, closing the socket, etc
 * =====================================================================================
 */
void close_connection( pMConnection pCon );

#endif   /* ----- #ifndef MCONNECTION_H_INC  ----- */

