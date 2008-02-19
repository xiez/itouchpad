//Will Dietz
//iTouchpadApp.h

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
#import <UIKit/UIApplication.h>
#import <UIKit/UIPushButton.h>
#import <UIKit/UITableCell.h>
#import <UIKit/UIImageAndTextTableCell.h>

#import "MainTable.h"

@interface iTouchpadApp : UIApplication {
	UINavigationItem * _titleItem;//title
	UINavigationBar * _navTop;

	UIWindow * _window;
	UIView * _mainView;
	MainTable * _table;

	UIAlertSheet * _connect;
}

- (id)_initWithArgc:(int)fp8 argv:(const char **)fp12;	// IMP=0x323b6ab0

+ (iTouchpadApp *) sharedInstance;

- (void) connect;

- (UIView *) mainView;

- (void) exitMe;

@end


