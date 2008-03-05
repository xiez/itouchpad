//itouchpadfunctions.h
//Will Dietz

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

#import "consts.h"
#import "inputevent.h"

//Handles the network stuff, and settings management


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init
 *  Description:  Initializes variables used in this code--and loads settings
 * =====================================================================================
 */
void init();


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hasConnected
 *  Description:  returns true iff a connection is/has been established
 * =====================================================================================
 */
bool hasConnected();


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_server
 *  Description:  Attempts to connect to the server and port stored in settings.  Returns 1 iff successful.
 * =====================================================================================
 */
int init_server();



/*-----------------------------------------------------------------------------
 *  Settings stuff
 *-----------------------------------------------------------------------------*/
NSString * getServer();
int	getPort();

void setServer( NSString * server );
void setPort( int port );


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendMouseMove
 *  Description:  Helper function that sends a mouse move delta input event.
 * =====================================================================================
 */
void sendMouseMove( int dx, int dy ); 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendMouseScroll
 *  Description:  Helper function that sends a scroll delta input event.
 * =====================================================================================
 */
void sendMouseScroll( int dx, int dy );


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendButtonPress
 *  Description:  sends a mouse event for the specified button, either up for down according to 'press'.
 * =====================================================================================
 */
void sendButtonPress( int b, bool down );

