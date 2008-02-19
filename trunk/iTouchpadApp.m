//Will Dietz
//iTouchpadApp.m
// The heart of the application 

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


//TODO: are all these #import's really necessary?
#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <UIKit/CDStructures.h>
#import <UIKit/UIPushButton.h>
#import <UIKit/UIThreePartButton.h>
#import <UIKit/UINavigationBar.h>
#import <UIKit/UIWindow.h>
#import <UIKit/UIView-Hierarchy.h>
#import <UIKit/UIHardware.h>
#import <UIKit/UITable.h>
#import <UIKit/UITableCell.h>
#import <UIKit/UITableColumn.h>
#import <UIKit/UIAlertSheet.h>
#import <GraphicsServices/GraphicsServices.h>
#import "iTouchpadApp.h"
#import "itouchpadfunctions.h"
#import "ViewController.h"
#import "consts.h"
#import "PrefsView.h"

static iTouchpadApp * sharedInstance = nil;


@implementation iTouchpadApp
- (id)_initWithArgc:(int)fp8 argv:(const char **)fp12
{
	
	return (sharedInstance = [super _initWithArgc: fp8 argv: fp12 ] );	

}

+ (iTouchpadApp *) sharedInstance
{
	if ( sharedInstance )
	{
		return sharedInstance;
	}
	NSLog( @"ERROR: sharedInstance called /before/ obj init'd.." );

	return nil;//we /want/ things to die, this shouldn't ever happen
}

- (void) applicationDidFinishLaunching: (id) unused
{
	
	struct CGRect rect = [UIHardware fullScreenApplicationContentRect];
	rect.origin. x = rect.origin.y = 0;
	_window = [[UIWindow alloc] initWithContentRect: rect];


	_mainView = [[ UIView alloc] initWithFrame: rect];
	[ [ViewController sharedInstance ] addSubview: _mainView ];	
	[ [ViewController sharedInstance ] setCurView: _mainView ];

	[_window orderFront: self];
	[_window makeKey: self];
	[_window _setHidden: NO];

///	[_window setContentView: [ ViewController sharedInstance ] ];
	[ _window setContentView: _mainView ];

	//top navigation bar.
	_navTop = [[UINavigationBar alloc] initWithFrame: CGRectMake(
		0.0f, 0.0f, 320.0f, 48.0f)];
	_titleItem = [ [UINavigationItem alloc] initWithTitle: ITOUCHPAD ];
	[_navTop showLeftButton: L_PREFS withStyle: BUTTON_BACK rightButton: L_QUIT withStyle: BUTTON_RED ];
	[_navTop pushNavigationItem: _titleItem];
	[_navTop setDelegate: self];	
	[_navTop setBarStyle: 3];

	//init the table--it's always going to be empty, but it seemed the simplest way to get a blank white space
	_table = [[ MainTable alloc] initWithFrame: CGRectMake(0.0f, 48.0f,
		320.0f, 480.0f - 16.0f - 48.0f )];

	_connect = [[UIAlertSheet alloc]initWithTitle: L_CONNECTING_MSG buttons:nil defaultButtonIndex:1 delegate:self context:self];
	[_connect setDimsBackground: YES];

	init();

	[self connect ];

	[_mainView addSubview:  _navTop ];
	[_mainView addSubview: _table ];

	NSLog( @"Done with applicationDidFinishLaunching" ); 
}

- (BOOL)respondsToSelector:(SEL)aSelector
{
//	NSLog(@"respondsToSelector: %@|| %d", NSStringFromSelector(aSelector), [ super respondsToSelector: aSelector ] );
	return [super respondsToSelector: aSelector];
}

- (void) connect
{
	[ _connect setTitle: L_CONNECTING_MSG ];
	[_connect presentSheetInView: [ViewController sharedInstance ] ];	 

	[NSTimer scheduledTimerWithTimeInterval: REFRESH_TIME target:self selector:@selector(delayedInit) userInfo:nil repeats:NO];	

}

- (UIView *) mainView
{
	return _mainView;
}


- (void)dealloc {
	  
	[ _titleItem release];
	[ _mainView release ];
	[ _connect release ];

	[super dealloc];

}

- (void) delayedInit
{
	if(	!init_server() )
	{//if fail.. just go to prefs page
		NSLog( @"connection failed... showing prefs view" );
		[ NSTimer scheduledTimerWithTimeInterval: REFRESH_TIME target: self selector:@selector( gotoPrefs ) userInfo: nil repeats: NO ];
		[ _connect dismiss ];
	}
	else
  	{
		// yay \o/
		[ _connect dismiss ];
	}
}

- (void) gotoPrefs
{
	[ [ViewController sharedInstance] setView: [PrefsView sharedInstance ] slideFromLeft: NO ];
}


- (void) exitMe
{
	[self terminateWithSuccess ];
}

- (void)navigationBar:(UINavigationBar*)bar buttonClicked:(int) which;
{
//	NSLog( @"button pressed, which: %d", which );
	if ( which == 0 ) //right
	{
		[self exitMe ];
		//TODO: close the app gracefully.
	}
	else
	{
		[ [ViewController sharedInstance] setView: [PrefsView sharedInstance] slideFromLeft: NO ];

	}

}

@end

