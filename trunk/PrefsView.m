//Will Dietz
//PrefsView.m

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

#import "PrefsView.h"
#import "ViewController.h"
#import "iTouchpadApp.h"
#import "itouchpadfunctions.h"
#import "consts.h"

static PrefsView * sharedInstance = nil;

@implementation PrefsView

+ (PrefsView *) sharedInstance
{
	if ( sharedInstance )
		return sharedInstance;

	//else

	struct CGRect rect = [UIHardware fullScreenApplicationContentRect];
	rect.origin. x = rect.origin.y = 0;
	sharedInstance = [[PrefsView alloc] initWithFrame: rect ]; 

	return sharedInstance;
}


- (id) initWithFrame: (CGRect) rect
{
	[super initWithFrame: rect];

	//TODO: make this relative to rect
	_prefTable = [[UIPreferencesTable alloc] initWithFrame: CGRectMake(0.0f, 48.0f,
	320.0f, 480.0f - 16.0f - 48.0f)  ];
	[_prefTable setDataSource: self];
	[_prefTable setDelegate: self];
	[_prefTable setBottomBufferHeight:44.0f];

	//init both arrays...
	_rows = [[NSMutableArray alloc] init];
	_rowsAbout = [[NSMutableArray alloc] init];

	UIPreferencesTextTableCell * _row;
	//server
	_row = [[UIPreferencesTextTableCell alloc] init ];
	[_row setTitle: L_SERVER ];
	[[_row textField] setText: @""];//blank for now, fill this in!!
	[_rows addObject: _row];

	//port
	_row = [[UIPreferencesTextTableCell alloc] init ];
	[_row setTitle: L_PORT ];
	[[_row textField] setText: @""];
	[_rows addObject: _row];

	//put information about iTouchpad and author here, copyright, etc
	//about
	//TODO: center this?
	UIPreferencesTableCell * _rowAbout = [[UIPreferencesTableCell alloc] init ];
	[ _rowAbout setTitle: RELEASE_AND_VERSION ];
	[ _rowsAbout addObject: _rowAbout ];

	_rowAbout = [[UIPreferencesTableCell alloc] init ];
	[ _rowAbout setTitle: AUTHOR ];
	[ _rowsAbout addObject: _rowAbout ];

	_rowAbout = [[UIPreferencesTableCell alloc] init ];
	[ _rowAbout setTitle: CONTACT_EMAIL ];
	[ _rowsAbout addObject: _rowAbout ];


	//tell the table "we got your data! come and get it!"
	[_prefTable reloadData];


	//initialize the headers for the various groups in the preferences table...
	_prefHeader = [[UIPreferencesTableCell alloc] init];
	[_prefHeader setTitle: L_TOUCHPAD_SETTINGS ];

	_prefAboutHeader = [[UIPreferencesTableCell alloc] init];
	[_prefAboutHeader setTitle: L_ABOUT ];

	//initialize the navbar
	UINavigationBar *nav = [[UINavigationBar alloc] initWithFrame: CGRectMake(
		0.0f, 0.0f, 320.0f, 48.0f)];
	_titleItem = [ [UINavigationItem alloc] initWithTitle: L_PREFERENCES ];
	[nav showLeftButton: L_QUIT withStyle: BUTTON_RED rightButton: L_DONE withStyle: BUTTON_NORMAL ]; 
	[nav pushNavigationItem: _titleItem];
	[nav setDelegate: self];
	[nav setBarStyle: 0];

	//add the views to ourself
	[self addSubview: nav];
	[self addSubview: _prefTable];
	
	//done!
	return self;
}

- (void) setDelegate: (id) delegate
{
	_delegate = delegate;

}
////////////////////////////////////////////////////////////////////////////////////////////////
//Preferences Table Stuff:
////////////////////////////////////////////////////////////////////////////////////////////////


// Start of Preference required methods
- (int) numberOfGroupsInPreferencesTable: (UIPreferencesTable*)table 
{
//	NSLog( @"numberOfGroups Called\n" );
	return 2; //TODO: update this as needed
}

- (int) preferencesTable: (UIPreferencesTable*)table numberOfRowsInGroup: (int)group 
{
//	NSLog( @"numberOfRowsInGroup, current row count is: %d\n", [_rows count] ); 
	switch( group )
	{
	//I don't really understand why the header needs its own group, particular looking at how
	//the 'preferencesTable: cellforGroup: method works....
	//but haven't had a chance to look into it properly
		case 0: return 0;
		case 1: return [_rows count];
		case 2: return 0;
		case 3: return [_rowsAbout count];
		default:
			//ERROR :-(
			NSLog( @"WTF: invalid group count in prefstable" );
			return 0;
	}
}

- (UIPreferencesTableCell*) preferencesTable: (UIPreferencesTable*)table cellForGroup: (int)group 
{

	switch (group)
	{
	//I don't understand why a header needs to be the header for 2 groups (and I don't even know that
	//it has to be this way), and haven't had a chance to look into it
		case 0: return _prefHeader;
		case 1: return _prefHeader;
		case 2: return _prefAboutHeader;
		case 3: return _prefAboutHeader;
		default: return nil;
	}


}

- (BOOL) preferencesTable: (UIPreferencesTable*)table isLabelGroup: (int)group 
{
	switch (group)
	{
		case 0: return true;
		case 1: return false;
		case 2: return true;
		case 3: return false;
		default:
			NSLog( @"WTF: invalid group count in prefstable" );
			return true;
	}

}


- (UIPreferencesTableCell*) preferencesTable: (UIPreferencesTable*)table cellForRow: (int)row inGroup: (int)group 
{
	switch( group )
	{
		case 0: return _prefHeader;
		case 1: return [[_rows objectAtIndex: row] retain];
		case 2:	return _prefAboutHeader;
		case 3:	return [[_rowsAbout objectAtIndex: row] retain ];
		default:
			NSLog( @"WTF: invalid group count in prefstable" );
			return nil;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

//call this to tell us to fetch/load our values
- (void) loadSettings
{
	[ [ [ _rows objectAtIndex: SERVER_ROW ] textField ] setText: (id)getServer() ]; 
	[ [ [ _rows objectAtIndex: PORT_ROW ] textField ] setText: (id)[ NSString stringWithFormat: @"%d", getPort() ] ];

	[ _prefTable reloadData ];
}

//call this to tell us to commmit/save our values
- (void) saveSettings
{
	setServer( [ [ [ _rows objectAtIndex: SERVER_ROW ] textField ] text ] );
	setPort( [ [ [ [ _rows objectAtIndex: PORT_ROW ] textField ] text ] intValue ] );

}


//clean-up time....
- (void) dealloc
{
	[super dealloc];
	[_prefTable release];
	while( [_rows count] > 0 )
	{
		id row = [_rows objectAtIndex: 0 ];
		[_rows removeObjectAtIndex: 0 ];
		[row release ];	
	}

	while( [_rowsAbout count] > 0 )
	{
		id row = [_rowsAbout objectAtIndex: 0 ];
		[_rowsAbout removeObjectAtIndex: 0 ];
		[row release ];	

	}

	[_rows release];
	[_rowsAbout release];


	[_prefHeader release];
	[_prefAboutHeader release];	

	[_titleItem release];

}

////////////////////////////////////////////////////////////////////////////////////////////////

//Navigation bar handler


- (void)navigationBar:(UINavigationBar*)bar buttonClicked:(int)which;
{
	if ( which == 0 ) //right, go back
	{
		//commit changes
		[self saveSettings ];

		//go back
		[ [ ViewController sharedInstance ] setView: [ [iTouchpadApp sharedInstance] mainView ] slideFromLeft: YES ];

		
		if( !hasConnected() )
		{//maybe this made things better..? (now we have good auth, a valid server, etc)
			[ [ iTouchpadApp sharedInstance] connect ];
		}
	}
	
	else
	{//left
		//quit
		[ [ iTouchpadApp sharedInstance ] exitMe ];
	}

}
@end


