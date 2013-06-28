
/* The Ultimate Script Library 
-----------------------------------------------------------------------------------
   Module: State Machines
   Function pointer based state machine collection manager.

   Read the documentation for further information: TUST\Documentation\index.html
----------------------------------------------------------------------------------- 
*/

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

/**
 * \defgroup  StateMachineModule State Machines
 * \brief     Function pointer based state machine collection manager.
 * 
 * A state machine is a code structure that changes the execution flow in reference to the 
 * state of a variable. A function pointer based state machine is the same concept but it 
 * references a function to be executed next frame instead of checking a variable value.
 *
 * This module offers the possibility of running state machines for any kind of data, even
 * without any data. It is conceived to ensure that the actual state of every object is 
 * executed before been changed by another machine. This characteristic avoids the inherent advantage of the
 * execution order. It has also the minor benefit of executing all the machines from a single
 * array pass inside a single while loop. The state machines will be executed in the same order
 * they were created
 *
 * # How to use these state machine
 * 
 * - Include TUST library into your project.
 * - Start the state machines manager.
 *   ~~~
 *   // Funtion to be executed by a state machine
 *   function myState1 ( STMACHINE *stm ) 
 *   {
 *      // Retrieve the object of the machine
 *      MyStruct *myStruct = stm_me ( stm ); 
 *      you = stm_by_index ( stm ); // Retrieve an object by its index
 *      stm_set_state ( stm, stEnt2, 2 );
 *      my->clock += 20+random(30);
 *   }
 *   ...
 *   stm_open ();
 *   ...
 *   ~~~
 * - 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 *
 * \ingroup   GenericScriptLibrary
 * \{
 *
 * \file  state_machine.h
 * \brief State machine collection manager header.
 *
 * \file  state_machine.c
 * \brief State machine collection manager code.
 *
 * \file  test_state_machine.c
 * \brief State machine collection manager test code.
 */

/* ----------------------------------------------------------------------------------------- */
/* SETTINGS
/* ----------------------------------------------------------------------------------------- */

/**
 * \brief    Check for NULL pointers 
 */
#define STMC_SAFE_MODE


/**
 * \brief    Try to describe the errors ;) 
 */
#define STMC_ERRORS


/**
 * \brief    Show global state machine collections count
 */
#define STMC_COUNT


/**
 * \brief    State machines count into each memory allocation step 
 */
#define STMC_MEMSTEP					4


/* ----------------------------------------------------------------------------------------- */
/* STMACHINE DATA STRUCT
/* ----------------------------------------------------------------------------------------- */

typedef struct STMACHINE
{
	var stateNext;
	void *fncNext;
	void *ptrMe;
	void remover ( void *ptr );
	int *index;
	var flags;
	var state;
	void fncPtr ( struct STMACHINE *machine );
} STMACHINE;



/* ----------------------------------------------------------------------------------------- */
/* GENERAL FUNCTIONS
/* ----------------------------------------------------------------------------------------- */

/**
 * \brief    Start the state machines automation 
 */
void stm_open ();


/**
 * \brief    Finish the state machines automation 
 */
void stm_close ();


/**
 * \brief    Add a new state machine
 * \param    obj        A pointer to an object. It can be NULL too.
 * \param    remover    A function that removes the object, 
 *                      If NULL the object will not be removed when the state machine is removed
 * \param    fnc        The state machine starting function. It can't be NULL!
 * \param    state      a number to identify the starting function. Just a helper. Not used.
 * \param    index      a pointer to an integer that will be filled 
 *	                     with the actual index in the global state machines array.
 *	                     Since the state machines memory location is dynamic,
 *	                     their index into the array and memory address changes with the flow
 *                      so this integer is used as a secure and fast reference
 *                      to the new state machine.
 * \returns             A pointer to the object.
 */
void *stm_add ( void *obj, void *remover, void *fnc, var state, int *index );



/* ----------------------------------------------------------------------------------------- */
/* GENERAL FUNCTION HELPER MACROS
/* ----------------------------------------------------------------------------------------- */


/**
 * \brief    Create an integer variable and cast its pointer to another type
 *
 * Use it when you want to use an entity or panel (etc) skill as state machine index allocator
 *
 * \param    i          Fixed variable (var) to use as pointer to an integer			
 */
#define stm_create_index(i)		i=(int*)sys_malloc(sizeof(int))


/**
 * \brief    Delete an integer variable created with stm_create_index
 *
 * It has to be called after using stm_create_index
 *
 * \param    i          Fixed variable (var) used as pointer to an integer			
 */
#define stm_delete_index(i)		sys_free(i)


/**
 * \brief    The pointer saved in the variable used as a pointer to an integer
 *
 * Use it into last parameter of stm_add function when stm_create_index is used
 *
 * \param    i          Fixed variable (var) used as pointer to an integer			
 */
#define stm_index_ptr(i)			(int*)i



/* ----------------------------------------------------------------------------------------- */
/* INSIDE MACHINE FUNCTIONS
/* ----------------------------------------------------------------------------------------- */

/**
 * \brief    Change a state of a machine
 * \param    stm        A pointer to a state machine
 * \param    fnc        Function to execute in the new state
 * \param    state      A number to identify the new state
 */
void stm_set_state ( STMACHINE *stm, void *fnc, var state );


/**
 * \brief    Change the object of a machine
 * \param    stm        A pointer to a state machine
 * \param    obj        A pointer to an object
 * \param    remover    A function that deletes the object
 */
void stm_set_me ( STMACHINE *stm, void *obj, void *remover );


/**
 * \brief    Stop all the state machines that point to a certain object
 * \param    obj        A pointer to an object
 */
void stm_stop_ptr ( void *obj );



/* ----------------------------------------------------------------------------------------- */
/* INSIDE MACHINE HELPER MACROS
/* ----------------------------------------------------------------------------------------- */

/**
 * \brief    A state identify number of a state machine
 * \param    stmt	      A pointer to a state machine			
 */
#define stm_state(stmt)				stmt->state


/**
 * \brief    The pointer to the object of a state machine 
 * \param    stmt       A pointer to a state machine			
 */
#define stm_me(stmt)					stmt->ptrMe


/**
 * \brief    Stop a state machine in the next frame 
 * \param    stmt       A pointer to a state machine			
 */
#define stm_stop(stmt)				stm_set_state(stmt,stm_stop_machine,NULL)


/**
 * \brief    The content of an integer pointed by a pointer casted into another type variable
 *
 * Use it when you used an entity or panel (etc) skill as state machine index allocator
 *
 * \param    i          Fixed variable (var) to be casted to an integer pointer			
 */
#define stm_index(i)					*((int*)i)


/**
 * \brief    The pointer to the nth state machine
 * \param    i          Index into the state machines array			
 */
#define stm_by_index(i)				(gblMachines->stmFirst+i)

/**
 * \}
 */

#include "state_machine.c"
#endif