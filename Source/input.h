#ifndef _INPUT_H_
#define _INPUT_H_

/**
 * \file input.h
 * Provides a simple input manager.
 */

/**
 * Initializes the input system.
 */
void input_init(void);

/**
 * Updates all input values. You need to call this function every frame to get updated input.
 */
void input_update(void);

/**
 * Adds a button to an input value. If the button is pressed, the value of the input get's increased by one.
 * \param name The name of the input value.
 * \param key  The scan code of the key. Can be obtained by using key_for_str or from the key mapping table in the gamestudio manual.
 */
void input_set_button(STRING *name, int key);

/**
 * Adds a button pair to an input value.
 * If the increasing button is pressed, the value of the input get's increased by one.
 * If the decreasing button is pressed, the value of the input get's decreased by one.
 * \param name   The name of the input value.
 * \param keyInc The scan code of increasing the key. Can be obtained by using key_for_str or from the key mapping table in the gamestudio manual.
 * \param keyDec The scan code of decreasing the key. Can be obtained by using key_for_str or from the key mapping table in the gamestudio manual.
 */
void input_set_buttons(STRING *name, int keyInc, int keyDec);

/**
 * Adds a axis to an input value. The value of the axis gets scaled by scale and deadzone is 
 * substracted to get a better input feeling.
 * \param axis     A pointer to a global variable that contains the axis value (eg. key_force.y, joy_force.z, mouse_force.x)
 * \param scale    The scale applied to the axis value. Allows you to adjust the axis to your needs.
 * \param deadzone A deadzone value. If the magnitute of axis is below deadzone, the input value will be 0.
 */
void input_set_axis(STRING *name, var *axis, var scale, var deadzone);

/**
 * Gets the value of an input.
 * \param  name The name of the input value.
 * \return      Value of the input.
 */
var input_get(STRING *name);

/**
 * Gets the hit value of an input.
 * \param  name The name of the input value.
 * \return      0 if the input value was unchanged or 0 in the last frame, else not 0.
 */
var input_hit(STRING *name);


#include "input.c"
#endif