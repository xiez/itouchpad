//MainTable.m
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

#import "MainTable.h"
#import <GraphicsServices/GraphicsServices.h>
#import "itouchpadfunctions.h"

@implementation MainTable

- initWithFrame: (CGRect) rect
{
	_xPrev = -1;
	_yPrev = -1;
	_curState = STATE_NONE;

	return [super initWithFrame: rect ];

}

- (BOOL)respondsToSelector:(SEL)aSelector
{
//	NSLog(@"respondsToSelector: %@|| %d", NSStringFromSelector(aSelector), [ super respondsToSelector: aSelector ] );
	return [super respondsToSelector: aSelector];
}

- (void)dumpGSEvent: (GSEvent *)event
{
	/*
		http://www.cocoadev.com/index.pl?GraphicsServices
	*/

//	NSLog(@"GSEventIsChordingHandEvent: %d", GSEventIsChordingHandEvent(event));
//	NSLog(@"GSEventGetClickCount: %d", GSEventGetClickCount(event));
//	CGRect rect = GSEventGetLocationInWindow(event);
//	NSLog(@"GSEventGetLocationInWindow: (%f, %f, %f, %f)", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
//	NSLog(@"GSEventGetDeltaX: %f", GSEventGetDeltaX(event));
//	NSLog(@"GSEventGetDeltaY: %f", GSEventGetDeltaY(event));
//	CGPoint inner = GSEventGetInnerMostPathPosition(event);
//	NSLog(@"GSEventGetInnerMostPathPosition: (%f, %f)", inner.x, inner.y);
//	CGPoint outer = GSEventGetOuterMostPathPosition(event);
//	NSLog(@"GSEventGetOuterMostPathPosition: (%f, %f)", outer.x, outer.y);
//	NSLog(@"GSEventGetSubType: %d", GSEventGetSubType(event));
//	NSLog(@"GSEventGetType: %d", GSEventGetType(event));
//	NSLog(@"GSEventDeviceOrientation: %d", GSEventDeviceOrientation(event));

}

- (void)setCoords: (GSEvent *)event
{
	CGRect rect = GSEventGetLocationInWindow( event );

	_xPrev = rect.origin.x;
	_yPrev = rect.origin.y;

}

- (void)updateCoords: (GSEvent *)event
{
	CGRect rect = GSEventGetLocationInWindow( event );
	
	if ( _xPrev != -1 && _yPrev != -1 ) //if valid previous coords
	{
		switch ( _curState )
		{
			case STATE_MOUSE_DOWN_MOVE:
				sendMouseMove( rect.origin.x - _xPrev, rect.origin.y - _yPrev );
				break;
			case STATE_MOUSE_DOWN_CHORDING_MOVE:
				sendMouseScroll( rect.origin.x - _xPrev, rect.origin.y - _yPrev );
				break;
			default:
				break;
		}
	}

	[ self setCoords: event ];

}


- (void)mouseDown: ( GSEvent * )event
{

	switch ( _curState )
	{
		case STATE_NONE:
			_curState = STATE_MOUSE_DOWN;
			[ self setCoords: event ];
			break;
		case STATE_MOUSE_DOWN: //mouse already down!
			if ( GSEventIsChordingHandEvent( event ) )//if > 1 finger.. (maybe?)
			{
				_curState = STATE_MOUSE_DOWN_CHORDING;
			}
			break;
		default:
			break;
	}

//	[ self dumpGSEvent: event ];
}

- (void) mouseUp: (GSEvent *) event
{

	switch( _curState )
	{
		case STATE_MOUSE_DOWN:
			NSLog( @"click!" );
			//since user didn't drag, we simulate full click
			sendButtonPress( true );
			sendButtonPress( false );
			_curState = STATE_NONE;
			break;
		case STATE_MOUSE_DOWN_CHORDING:
			if ( GSEventIsChordingHandEvent( event ) )
			{
				NSLog( @"drag!" );
			}
			sendButtonPress( true );
			break;

		case STATE_MOUSE_DOWN_CHORDING_MOVE: //end of scroll	
		case STATE_MOUSE_DOWN_MOVE: //end of normal move, or drag
			sendButtonPress( false );//can't hurt....?
			_curState = STATE_NONE;
		default:
			break;
	}

	[ self dumpGSEvent: event ];

}

- (void) mouseDragged: ( GSEvent *)event
{

	switch( _curState )
	{
		case STATE_MOUSE_DOWN:
			_curState = STATE_MOUSE_DOWN_MOVE;
			if ( GSEventIsChordingHandEvent( event ) )
			{
				_curState = STATE_MOUSE_DOWN_CHORDING;
				break;
			}
		case STATE_MOUSE_DOWN_MOVE:	
			[ self updateCoords: event ];
			break;
		case STATE_MOUSE_DOWN_CHORDING:
			if ( GSEventIsChordingHandEvent( event ) )
			{//if still has > 1 finger down. scroll
				NSLog( @"scroll!" );
				_curState = STATE_MOUSE_DOWN_CHORDING_MOVE;
			}
			else
			{
				NSLog( @"drag!" );
				sendButtonPress( true );
				_curState = STATE_MOUSE_DOWN_MOVE;
			}
		case STATE_MOUSE_DOWN_CHORDING_MOVE:	
			[ self updateCoords: event ];
			break;
		default:
			break;


	}

	

	[ self dumpGSEvent: event ];
}




@end
