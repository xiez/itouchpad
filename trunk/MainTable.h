//MainTable.h
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
#import <UIKit/UIKit.h>

typedef enum
{
	STATE_NONE,
	STATE_MOUSE_DOWN,
	STATE_MOUSE_DOWN_CHORDING,
	STATE_MOUSE_DOWN_MOVE,
	STATE_MOUSE_DOWN_CHORDING_MOVE
} states;

@interface MainTable : UITable
{

states _curState;	
int _xPrev;
int _yPrev;

}

@end
