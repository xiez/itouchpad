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
#include <process.h>
#include <queue>

#include "../../../common/inputevent.h"

//dang windows lack of compliance
#ifndef MSG_WAITALL
//#define MSG_WAITALL 0x08
//we define it as zero, since I can't get it to work.
#define MSG_WAITALL 0
#endif

//foredeclarations
void socketListen( void * pParam );
void handleEvent( pInputEvent pEvent );
void combineMoveEvents();

//cs for updates...locks the update queue
CRITICAL_SECTION cs_q;
//signals that an update has been made
HANDLE update_event;

std::queue<InputEvent> q_events;

int _tmain(int argc, _TCHAR* argv[])
{
	update_event = CreateEvent( NULL, FALSE, FALSE, TEXT("UpdateEvent") );

	::InitializeCriticalSection( &cs_q );
	::_beginthread( socketListen, 0, NULL );

	while( 1 )
	{
		//wait for an event to enter the queue to be processed
		::WaitForSingleObject( update_event, INFINITE ); 

		//process all events in the queue
		::EnterCriticalSection( &cs_q );
		combineMoveEvents();
		while( q_events.size() != 0 )
		{
			InputEvent ev = q_events.front();
			q_events.pop();

			handleEvent( &ev );

		}
		::LeaveCriticalSection( &cs_q );

		/*
		 * The following code doesn't seem to do
		 * what I expected it to.
		 * The problem is that the events seem to be grouped
		 * together, or otherwise not processed at all
		 * when expected.
		 * Something like flushMouseEvents() would be wonderful.
		 */

		//force the event to be processed.. we wait until it's done.
		MSG msg;
		while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( ::GetMessage( &msg, NULL, 0, 0 ) )
			{
				::TranslateMessage( &msg );
				::DispatchMessage( &msg );
			}
			else
			{
				break;
			}
		}

		//guarantee that 20ms are between bursts of events.. hopefully this helps!!!!
		Sleep( 0 );

	}
	
	return 0;
	

}

void socketListen( void * pParam )
{
	InputEvent ev;
	pInputEvent pEvent = &ev;
	
	SOCKET s, s_accept;
	struct sockaddr_in s_add; //from anyone!
	struct sockaddr s_client;
	int s_client_size = sizeof( struct sockaddr );
	int port = PORT;
	int recvsize;

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
			return exit( 4 );
		}

		while( 1 )
		{
			recvsize = recv( s_accept, (char *)pEvent, sizeof( InputEvent ), MSG_WAITALL );
			if ( recvsize == sizeof( InputEvent ) )//got data
			{
				
				::EnterCriticalSection( &cs_q );
				q_events.push( *pEvent );
				::LeaveCriticalSection( &cs_q );
				//signal other thread that an update has occurred
				::SetEvent( update_event );
				
				
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

}


//#define DONT_USE_ACCEL
void handleEvent( pInputEvent pEvent )
{
#ifdef DONT_USE_ACCEL
	bool accelChanged;
#endif
	SYSTEMTIME time;
	::GetSystemTime( &time );
	

	switch( pEvent->event_t )
	{
		case EVENT_TYPE_MOUSE_MOVE:
			fprintf( stderr, "%2d:%02d:%02d.%04d :: mouse move!\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
			fflush( stderr );
			POINT pt;
			::GetCursorPos( &pt );
			pt.x += pEvent->move_info.dx;
			pt.y += pEvent->move_info.dy;
			::SetCursorPos( pt.x, pt.y );

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

}

//assumes already in cs over the queue
void combineMoveEvents()
{
	//here we make a pass through pending events
	//and combine all the mouse moves
	//this way, we only send one move/timeinterval
	//idea is that this would reduce lag in windows server
	InputEvent e;
	e.event_t = EVENT_TYPE_MOUSE_MOVE;
	e.move_info.dx = 0;
	e.move_info.dy = 0;

	bool hasMouseMove = false;
	bool didSomething = false;

	std::queue<InputEvent> q_copy;
	while( q_events.size() != 0 )
	{
		InputEvent cur_ev = q_events.front();
		q_events.pop();

		if ( cur_ev.event_t == EVENT_TYPE_MOUSE_MOVE )
		{
			if (hasMouseMove) didSomething = true;
			hasMouseMove = true;
			e.move_info.dx += cur_ev.move_info.dx;
			e.move_info.dy += cur_ev.move_info.dy;

		}
		else
		{
			q_copy.push( cur_ev );
		}
	}

	if( hasMouseMove )
	{
		if ( didSomething )
		{
			//DEBUG to see if this method is worthwhile
		//	fprintf( stdout, "combined! we're doing something!!!" );
		//	fflush( stdout );
		}
		q_copy.push( e );
	}

	q_events = q_copy;

}

