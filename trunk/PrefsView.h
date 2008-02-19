//Will Dietz
//PrefsView.h

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
#import <UIKit/UIPreferencesTable.h>
#import <UIKit/UIPreferencesTextTableCell.h>

//View to allow the user to set/update/modify their preferences

//local defines for our rows...
#define SERVER_ROW 0
#define PORT_ROW 1

@interface PrefsView: UIView
{
	UIPreferencesTable * _prefTable;
	UIPreferencesTableCell * _prefHeader;
	UIPreferencesTableCell * _prefAboutHeader;

	//array holding the preferences entries
	NSMutableArray * _rows;

	//array holding the arrays displaying the 'about' information
	NSMutableArray * _rowsAbout;
	UINavigationItem * _titleItem;
	id _delegate; //go back to main view! :)
} 

//singleton
+ (PrefsView *) sharedInstance;

- (id) initWithFrame: (CGRect) rect;

- (void) setDelegate: (id) delegate;

//call this to tell us to fetch/loud our values
- (void) loadSettings;

//call this to tell us to commmit/save our values
- (void) saveSettings;


@end
