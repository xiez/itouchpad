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

//Handles the network stuff, and settings management

char server[ MAX_SERVER ];
int port;

//methods:

void init();

bool hasConnected();

int init_server();

NSString * getServer();
int	getPort();

void setServer( NSString * server );
void setPort( int port );

void readSettingsFromFile();

void saveSettingsToFiles();

void sendMouseMove( int dx, int dy ); 

void sendButtonPress( bool down );

