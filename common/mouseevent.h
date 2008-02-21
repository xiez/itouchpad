//mouseevent.h
//Will Dietz

//Defines some global network structs 

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

#ifndef _MOUSEEVENT_H_
#define _MOUSEEVENT_H_

#define PORT 5583

typedef enum
{
	EVENT_TYPE_MOUSE_DOWN,
	EVENT_TYPE_MOUSE_UP,
	EVENT_TYPE_MOUSE_MOVE
} eventtype;

typedef enum
{
	BUTTON_LEFT = 1, //TODO: check
	BUTTON_RIGHT = 2
} buttons;

typedef struct
{
	int dx;
	int dy;
} mousemove_info;

typedef struct
{
	buttons button;
} mousebutton_info;



typedef struct
{
	eventtype event_t;
	//these are only used in MOUSE_MOVE, else just do the down/up where mouse currently is
	union
	{
		mousemove_info move_info;
		mousebutton_info button_info;
	};

} MouseEvent, *pMouseEvent;


#endif // _MOUSEEVENT_H_
