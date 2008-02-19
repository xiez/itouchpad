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

#import <dns_sd.h>
#import <sys/select.h>
#import <unistd.h>

#import "itouchpadfunctions.h"
#import "NetworkController.h"


void init()
{
	port = -1;
	server[0] = '\0';
}

bool m_hasConnected;

bool hasConnected()
{
	return m_hasConnected;
}


NSString * getServer()
{
	if ( server && strlen( server ) > 0 )
		return [NSString stringWithCString: server ];
	return @"";
}

int getPort()
{
	return port;
}

//bool weCreatedNNTPServer = false;
void setServer( NSString * nsServer )
{

	strncpy( server, [nsServer cString], MAX_SERVER - 1 );

}

void setPort( int newport )
{
	port = newport;
}

void readSettingsFromFile()
{
	//TODO: actually read them in!

	strcpy( server, "192.168.255.1" );
	port = 5583;

}

void saveSettingsToFiles()
{
	//TODO: IMPLEMENT THIS!
}








//dns request
bool resolveHostname( char * hostname )
{
	NSString * name = [NSString stringWithFormat: @"%s", hostname ];

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
		bool ret = false;
		int result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
		if (result > 0)
		{
			if (FD_ISSET(dns_sd_fd, &readfds))
			{
				//remove this if you want to compile in c, not obj-c
				NSLog( @"resolved %s to %@", hostname, [ [ NSHost hostWithName: name ] address ] );
				ret = true;
			}
		}
		//clean up and return accordingly
		DNSServiceRefDeallocate( service );
		return ret;

	}
	//clean up....
	DNSServiceRefDeallocate( service );

	NSLog( @"dns error: %d", error );

	return false;
}

int init_server()
{
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
	

	readSettingsFromFile();
	

//	we don't do dns for now,
//	since we're only supporting actual ip addresses.
//
//	if ( !resolveHostname( (char *)server ) )
//		return false; //DNS failed :(


	m_hasConnected = true;
	return true;//it worked!!! \o/
}


void sendGesture( int dx, int dy )
{
	if ( m_hasConnected )
	{
		//TODO: add code that sends the movement to the server


	}

}


