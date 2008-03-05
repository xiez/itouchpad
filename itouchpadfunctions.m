//itouchpadfunctions.m
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

#import <Foundation/Foundation.h>
#import <Foundation/NSHost.h>
#import <UIKit/UIKit.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#import <dns_sd.h>
#import <sys/select.h>
#import <unistd.h>

#import "itouchpadfunctions.h"
#import "NetworkController.h"
#import "mconnection.h"
#import "consts.h"
#import "PrefsView.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init
 *  Description:  Initializes variables used in this code--and loads settings
 * =====================================================================================
 */
void init()
{
	NSMutableDictionary * defs = [[ NSMutableDictionary alloc ] init ];
	[ defs setObject: DEFAULT_SERVER forKey: K_SERVER ];
	[ defs setObject: [ NSNumber numberWithInt: DEFAULT_PORT ] forKey: K_PORT ];
	[ [ NSUserDefaults standardUserDefaults ] registerDefaults: defs ];

	[ [ PrefsView sharedInstance ] loadSettings ];
}


/*-----------------------------------------------------------------------------
 *  Local variables representing our connection and its state
 *-----------------------------------------------------------------------------*/
static bool m_hasConnected;
static MConnection con;
static pMConnection pCon = &con;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hasConnected
 *  Description:  returns true iff a connection is/has been established
 * =====================================================================================
 */
bool hasConnected()
{
	return m_hasConnected;
}


NSString * getServer()
{
	NSString * nsServer = [ [ NSUserDefaults standardUserDefaults ] stringForKey: K_SERVER ];
	if ( nsServer == nil )
	{
		return @"";
	}

	return nsServer;
}

int getPort()
{
	NSNumber * nPort = [ [ NSUserDefaults standardUserDefaults ] objectForKey: K_PORT ];
	return [ nPort intValue ];
}

void setServer( NSString * nsServer )
{
	[ [ NSUserDefaults standardUserDefaults ] setObject: nsServer forKey: K_SERVER ];
}

void setPort( int newport )
{
	[ [ NSUserDefaults standardUserDefaults ] setInteger: newport forKey: K_PORT ];
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  resolveHostname
 *  Description:  attempts to resolve the hostname using apple's API.  returns string containing the ip if successful, else nil.
 * =====================================================================================
 */
NSString * resolveHostname( const char * hostname )
{
	NSString * name = [NSString stringWithFormat: @"%s", hostname ];

	if ( inet_addr( hostname ) != INADDR_NONE )
	{//it's already in a format we like/can use
		return name;
	}

	NSLog( @"Resolving for %@", name );
	DNSServiceErrorType error;
	DNSServiceRef service;

	error = DNSServiceQueryRecord( &service, 0 /*no flags*/,
		0 /*all network interfaces */,
		hostname,
		kDNSServiceType_A, /* we want the ipv4 addy */ 
		kDNSServiceClass_IN, /* internet */
		0, /* no callback */ 
		NULL /* no context */ );

	if ( error == kDNSServiceErr_NoError )//good so far...
	{
		int dns_sd_fd = DNSServiceRefSockFD(service);
		int nfds = dns_sd_fd + 1;
		fd_set readfds;
		struct timeval tv;
	
		FD_ZERO(&readfds);
		FD_SET(dns_sd_fd, &readfds);
		tv.tv_sec = DNS_RESOLV_TIMEOUT;
		tv.tv_usec = 0;
		NSString * ret = nil;
		int result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
		if (result > 0)
		{
			if (FD_ISSET(dns_sd_fd, &readfds))
			{
				//remove this if you want to compile in c, not obj-c
				NSLog( @"resolved %s to %@", hostname, [ [ NSHost hostWithName: name ] address ] );
				ret = [ NSString stringWithString: [ [ NSHost hostWithName: name] address ] ];
			}
		}
		//clean up and return accordingly
		DNSServiceRefDeallocate( service );
		return ret;

	}
	//clean up....
	DNSServiceRefDeallocate( service );

	NSLog( @"dns error: %d", error );

	return nil;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_server
 *  Description:  Attempts to connect to the server and port stored in settings.  Returns 1 iff successful.
 * =====================================================================================
 */
int init_server()
{
	NSString * serverip;
	NSLog( @"Checking connection....");

	if(!([[NetworkController sharedInstance]isNetworkUp]))
	{
		if(![[NetworkController sharedInstance]isEdgeUp])
		{
			[[NetworkController sharedInstance]keepEdgeUp];									
			[[NetworkController sharedInstance]bringUpEdge];
			NSLog( @"Bringing edge up....");//update message
			sleep( 3 );
		}
	}

	if ( getServer() == DEFAULT_SERVER )
	{
		return false; //never want the default
	}

	if ( ( serverip = resolveHostname( [ getServer() cString ]) ) == nil  )
		return false; //DNS failed :(

	if ( init_connection( pCon, [ serverip cString ], getPort() ) < 0 )
	{
		m_hasConnected = false;
		return false;
	}

	m_hasConnected = true;
	return true;//it worked!!! \o/
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendMouseMove
 *  Description:  Helper function that sends a mouse move delta input event.
 * =====================================================================================
 */
void sendMouseMove( int dx, int dy )
{
	InputEvent e;
	e.event_t = EVENT_TYPE_MOUSE_MOVE;
	e.move_info.dx = dx * MOUSE_SPEED;
	e.move_info.dy = dy * MOUSE_SPEED;

	sendEvent( pCon, &e );

	NSLog( @"Sending move!" );

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendMouseScroll
 *  Description:  Helper function that sends a scroll delta input event.
 * =====================================================================================
 */
void sendMouseScroll( int dx, int dy )
{
	InputEvent e;
	e.event_t = EVENT_TYPE_MOUSE_SCROLL_MOVE;
	e.move_info.dx = dx * MOUSE_SPEED;
	e.move_info.dy = dy * MOUSE_SPEED;

	sendEvent( pCon, &e );

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sendButtonPress
 *  Description:  sends a mouse event for the specified button, either up for down according to 'press'.
 * =====================================================================================
 */
void sendButtonPress( int button, bool down )
{
	InputEvent e;
	e.event_t = down? EVENT_TYPE_MOUSE_DOWN : EVENT_TYPE_MOUSE_UP;
	e.button_info.button = button;

	sendEvent( pCon, &e );

}





