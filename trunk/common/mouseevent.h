//mouseevent.h
//Will Dietz

//Defines some global network structs 


#ifndef _MOUSEEVENT_H_
#define _MOUSEEVENT_H_

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
