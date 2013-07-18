
/* The Ultimate Script Library 
-----------------------------------------------------------------------------------
   Module: Function Timer
   Execute a function in a time lapse

   Read the documentation for further information: TUST\Documentation\index.html
----------------------------------------------------------------------------------- 
*/

#ifndef _FUNCTION_TIMER_H_
#define _FUNCTION_TIMER_H_

/**
 * \defgroup  FunctionTimerModule Function Timer
 * \ingroup   GenericScriptLibrary
 * \brief     Execute a function in a time lapse.
 *
 * \{
 *
 * \file  function_timer.h
 * \brief Header of the module to execute a function in a time lapse.
 *
 * \file  function_timer.c
 * \brief Code of the module to execute a function in a time lapse.
 *
 * \file  test_ftimer.c
 * \brief Test code of the module to execute a function in a time lapse.
 */
 
/**
 * \struct   Ftimer
 * \brief    Function timer general struct.
 */
typedef struct Ftimer
{
	var ticks;
	void *ptrMe;
	void event ( struct Ftimer *ftimer );
	struct Ftimer *next;
} Ftimer;

/**
 * \brief    Stops all the running function timers.
 */
void ftimer_stop_all ();


/**
 * \brief    Stops all the running function timers that has the given object as Me pointer.
 * \param    prtMe    Object pointer to be stopped.
 */
void ftimer_stop_me ( void *ptrMe );


/**
 * \brief    Stops the given function timer.
 * \param    ftimer    Pointer to the function timer to be stopped.
 */
void ftimer_stop ( Ftimer *ftimer );


/**
 * \brief    Starts a new function timer.
 * \param    ptrMe         Pointer to an object that serves as Me pointer.
 * \param    event         Function to be executed at time end.
 * \param    time_lapse    Time to be elapsed before executing the function.
 * \returns  A pointer to the newly created function timer.
 */
Ftimer *ftimer_add ( void *ptrMe, void *event, var time_lapse );


/**
 * \brief    Pauses all the running function timers.
 * \param    ftimer    Pointer to the function timer to be stopped.
 */
void ftimer_pause ();

#include "function_timer.c"
#endif