//Will Dietz
//ViewController.h

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
#import <UIKit/UIKit.h>


@interface ViewController: UITransitionView
{
	UIView * _curView;//keeps track of current view, so we /can/ go back :)
	UIView * _prevView;//keeps track of view /before/ this one so we can return to it
	bool _prevDir;


}

+ (ViewController *) sharedInstance; //singleton

//transition to the specified view, in the specified direction
- (void) setView: (UIView *) view slideFromLeft: (bool) left;


//used by apps to figure out where they were called from, and what direction
//so we can return appropriately
- (UIView *) getPrevView;

- (bool) getPrevDir;


//used just once to tell us what our /first/ view is, since we don't use
//the setView method the first time 'round
- (void) setCurView: (UIView *) view;

@end
