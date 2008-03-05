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
#define debug true


typedef int SOCKET;



int main( int argc, char ** argv)
{
	CGSize mainScreenSize = CGDisplayScreenSize (CGMainDisplayID ());
	CGDirectDisplayID currentDisplayID = CGMainDisplayID ();
	CGSize currentDisplaySize = mainScreenSize;
	CGRect displayRect;
	CGPoint newloc;
	Point pt;
	CGEventRef eventRef;
	
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
						if(debug) printf("Mouse move\n");
						GetGlobalMouse( &pt );//get cursor pos
						//update x/y
						
						newloc.x = pt.h + pEvent->move_info.dx * 2;
						newloc.y = pt.v + pEvent->move_info.dy * 2;
						
						CGDisplayCount displayCount = 0;
						//check if we've crossed outside the x bounds
						
						CGDirectDisplayID tempDisplayID;
						
						CGGetDisplaysWithPoint (
							newloc,
							1,
							&tempDisplayID,
							&displayCount);
							
						
						if (displayCount != 0) {
							currentDisplayID = tempDisplayID;
						}
						
						displayRect = CGDisplayBounds(currentDisplayID);
						
						if (newloc.x < displayRect.origin.x) {
							newloc.x = displayRect.origin.x;
						}
						else if (newloc.x > displayRect.origin.x +
								 displayRect.size.width - 1) {
							newloc.x = displayRect.origin.x + displayRect.size.width - 1;
						}
						if (newloc.y < displayRect.origin.y) {
							newloc.y = displayRect.origin.y;
						}
						else if (newloc.y > displayRect.origin.y +
								 displayRect.size.height - 1) {
							newloc.y = displayRect.origin.y + displayRect.size.height - 1;
						}
												
						if(debug) 
						{
							printf("Current mouse location: {%d,%d}\n", pt.h, pt.v);
							printf("newloc: {%f,%f}\n", newloc.x, newloc.y);
							printf("Screen size: {%f,%f}\n", currentDisplaySize.width, currentDisplaySize.height);
							fflush( stdout );
						}
						//CGPostMouseEvent( newloc, true /*yes move there*/, 0 , false); 
						
						eventRef = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, newloc,
																   kCGMouseButtonCenter);
						CGEventSetType(eventRef, kCGEventMouseMoved); // Apple bug... need to set the type manually
											
						CGEventPost(kCGSessionEventTap, eventRef);
						CFRelease(eventRef);
						break;
					case EVENT_TYPE_MOUSE_SCROLL_MOVE:
						if(debug) 
						{
							printf( "Scrolling\n" );
							fflush( stdout );
						}
						
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
						if(debug) printf("Mouse down\n");
						GetGlobalMouse( &pt );//get cursor pos
						//update x/y
						
						
						
						if(debug) 
						{
							printf("Current mouse location: {%d,%d}", pt.h, pt.v);
							fflush( stdout );
						}
						//CGPostMouseEvent( newloc, true /*yes move there*/, 0 , false); 
						
						newloc.x = pt.h;
						newloc.y = pt.v;
						
						eventRef = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown , newloc,
																   kCGMouseButtonCenter);
						CGEventSetType(eventRef, kCGEventLeftMouseDown); // Apple bug... need to set the type manually
											
						CGEventPost(kCGSessionEventTap, eventRef);
						CFRelease(eventRef);
						break;

					case EVENT_TYPE_MOUSE_UP:	
						if(debug) printf("Mouse up\n");
						GetGlobalMouse( &pt );//get cursor pos
						//update x/y
						
						
						
						if(debug) 
						{
							printf("Current mouse location: {%d,%d}", pt.h, pt.v);
							fflush( stdout );
						}
						//CGPostMouseEvent( newloc, true /*yes move there*/, 0 , false); 
						
						newloc.x = pt.h;
						newloc.y = pt.v;
						
						eventRef = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp , newloc,
																   kCGMouseButtonCenter);
						CGEventSetType(eventRef, kCGEventLeftMouseUp); // Apple bug... need to set the type manually
											
						CGEventPost(kCGSessionEventTap, eventRef);
						CFRelease(eventRef);
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

