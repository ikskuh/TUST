#include <acknex.h>
#include <stdio.h>
#include "input.h"
#include "list.h"

typedef struct InputController
{
	var getValue(struct InputController *controller);
} InputController;

typedef struct InputButtonController
{
	var getValue(struct InputButtonController *controller);
	int keyIncrease;
	int keyDecrease;
} InputButtonController;

typedef struct InputAxisController
{
	var getValue(struct InputAxisController *controller);
	var *axisData;
	var scale;
	var deadZone;
} InputAxisController;

typedef struct InputDevice
{
	char name[100];
	var value;
	int pressed;
	List *controllerList;
} InputDevice;

List *inputDeviceList;

void input_init()
{
	inputDeviceList = list_create();
}

void input_update()
{
	if(inputDeviceList == NULL) return;
	ListIterator *it = list_begin_iterate(inputDeviceList);
	InputDevice *dev;
	for(dev = list_iterate(it); dev != NULL; dev = list_iterate(it))
	{
	    int value = 0;
		if(dev->controllerList == NULL) continue;
		ListIterator *itController = list_begin_iterate(dev->controllerList);
		InputController *ctrl;
		for(ctrl = list_iterate(itController); ctrl != NULL; ctrl = list_iterate(itController))
		{
			var getValue(InputController *ctrl);
			getValue = ctrl->getValue;
			if(getValue == NULL) continue;
			var v = getValue(ctrl);
			value += v;
		}
		list_end_iterate(itController);
		
		dev->pressed = 0;
		if(dev->value != value)
		{
			if(dev->value != 0) // This will cause problems with axis, but for buttons it works well
				dev->pressed = 1;
		}
		dev->value = value;
	}
	list_end_iterate(it);
}

InputDevice *input_find_device(STRING *name, bool create)
{
	if(inputDeviceList == NULL) return;
	if(name == NULL) return;
	ListIterator *it = list_begin_iterate(inputDeviceList);
	InputDevice *dev;
	for(dev = list_iterate(it); dev != NULL; dev = list_iterate(it))
	{
		if(!str_cmpi(name, dev->name)) continue;
		list_end_iterate(it);
		return dev;
	}
	list_end_iterate(it);
	
	if(create)
	{
		dev = sys_malloc(sizeof(InputDevice));
		strcpy(dev->name, _chr(name));
		dev->value = 0;
		dev->controllerList = list_create();
		
		list_add(inputDeviceList, dev);
		
		return dev;
	}
	
	return NULL;
}

var input_get(STRING *name)
{
	InputDevice *dev = input_find_device(name, false);
	if(dev == NULL) return 0;
	return dev->value;
}

var input_hit(STRING *name)
{
	InputDevice *dev = input_find_device(name, false);
	if(dev == NULL) return 0;
	return dev->pressed;
}

void input_set_button(STRING *name, int key)
{
	input_set_buttons(name, key, 0);
}

var input_get_buttons_value(InputButtonController *ctrl)
{
	return key_pressed(ctrl->keyIncrease) - key_pressed(ctrl->keyDecrease);
}

void input_set_buttons(STRING *name, int keyInc, int keyDec)
{
	InputDevice *dev = input_find_device(name, true);
	
	InputButtonController *ctrl = sys_malloc(sizeof(InputButtonController));
	ctrl->getValue = input_get_buttons_value;
	ctrl->keyIncrease = keyInc;
	ctrl->keyDecrease = keyDec;
	list_add(dev->controllerList, ctrl);
}

var input_get_axis_value(InputAxisController *ctrl)
{
	if(ctrl->axisData == NULL)
		return;
	var value = *ctrl->axisData;
	return sign(value) * maxv(0, abs(value) - ctrl->deadZone) * ctrl->scale;
}

void input_set_axis(STRING *name, var *axis, var scale, var deadzone)
{
	if(name == NULL) return;
	if(axis == NULL) return;
	if(scale == NULL) return;
	InputDevice *dev = input_find_device(name, true);
	
	InputAxisController *ctrl = sys_malloc(sizeof(InputAxisController));
	ctrl->getValue = input_get_axis_value;
	ctrl->axisData = axis;
	ctrl->scale = scale;
	ctrl->deadZone = deadzone;
	list_add(dev->controllerList, ctrl);
}

















