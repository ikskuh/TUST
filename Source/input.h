#ifndef _INPUT_H_
#define _INPUT_H_

/**
 * \file input.h
 * Input Manager
 */

void input_init();

void input_update(void);

void input_set_button(STRING *name, int key);

void input_set_buttons(STRING *name, int keyInc, int keyDec);

void input_set_axis(STRING *name, var *axis, var scale, var deadzone);

var input_get(STRING *name);


#include "input.c"
#endif