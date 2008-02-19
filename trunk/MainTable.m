//MainTable.m
//Will Dietz

#import "MainTable.h"
#import <GraphicsServices/GraphicsServices.h>

@implementation MainTable

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
/*
	NSLog(@"GSEventIsChordingHandEvent: %d", GSEventIsChordingHandEvent(event));
	NSLog(@"GSEventGetClickCount: %d", GSEventGetClickCount(event));
	CGRect rect = GSEventGetLocationInWindow(event);
	NSLog(@"GSEventGetLocationInWindow: (%f, %f, %f, %f)", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	NSLog(@"GSEventGetDeltaX: %f", GSEventGetDeltaX(event));
	NSLog(@"GSEventGetDeltaY: %f", GSEventGetDeltaY(event));
	CGPoint inner = GSEventGetInnerMostPathPosition(event);
	NSLog(@"GSEventGetInnerMostPathPosition: (%f, %f)", inner.x, inner.y);
	CGPoint outer = GSEventGetOuterMostPathPosition(event);
	NSLog(@"GSEventGetOuterMostPathPosition: (%f, %f)", outer.x, outer.y);
	NSLog(@"GSEventGetSubType: %d", GSEventGetSubType(event));
	NSLog(@"GSEventGetType: %d", GSEventGetType(event));
	NSLog(@"GSEventDeviceOrientation: %d", GSEventDeviceOrientation(event));
*/
}


- (void)mouseDown: ( GSEvent * )event
{

	NSLog( @"mouse down!" );
	_dragged = false;

	[ self dumpGSEvent: event ];
}

- (void) mouseUp: (GSEvent *) event
{

	NSLog( @"mouse up!" );
	if ( !_dragged )
	{
		NSLog( @"click!" );
	}

	[ self dumpGSEvent: event ];

}

- (void) mouseDragged: ( GSEvent *)event
{
	NSLog( @"mouse dragged!" );
	_dragged = true;
}




@end
