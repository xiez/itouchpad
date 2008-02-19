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

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

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
