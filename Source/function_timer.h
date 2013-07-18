
/* The Ultimate Script Library 
-----------------------------------------------------------------------------------
   Module: Function Timer
   Execute a function in a time lapse

   Read the documentation for further information: TUST\Documentation\index.html
----------------------------------------------------------------------------------- 
*/

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
 * \brief    Function timer general struct
 */
typedef struct Ftimer
{
	var ticks;
	void *ptrMe;
	void event ( struct Ftimer *ftimer );
	struct Ftimer *next;
} Ftimer;

/**
 * \brief    Stops all the running function timers
 */
void ftimer_stop_all ()
{
	while ( ftimerCol != NULL )
	{
		Ftimer *ftimerTemp = ftimerCol;
		ftimerCol = ftimerCol->next;
		sys_free ( ftimerTemp );
	}
}

/**
 * \brief    Stops all the running function timers that has the given object as Me pointer.
 * \param    prtMe    Object pointer to be stopped.
 */
void ftimer_stop_me ( void *ptrMe )
{
	Ftimer *ftimer = ftimerCol;
	Ftimer *ftimerPrev = NULL;
	while ( ftimer != NULL )
	{
		if ( ftimer->ptrMe == ptrMe )
		{
			if ( ftimerPrev == NULL )
				ftimerCol = ftimer->next;
			else
				ftimerPrev->next = ftimer->next;
			Ftimer *ftimerTemp = ftimer;
			ftimer = ftimer->next;
			sys_free ( ftimerTemp );
		}
		else
		{
			ftimerPrev = ftimer;
			ftimer = ftimer->next;
		}
	}
}

/**
 * \brief    Stops all the given function timer.
 * \param    ftimer    Pointer to the function timer to be stopped.
 */
void ftimer_stop ( Ftimer *ftimer )
{
	Ftimer *ftimerTemp = ftimerCol;
	Ftimer *ftimerPrev = NULL;
	while ( ftimerTemp != NULL )
	{
		if ( ftimer != ftimerTemp )
		{
			ftimerPrev = ftimerTemp;
			ftimerTemp = ftimerTemp->next;
			continue;
		}
		
		if ( ftimerPrev == NULL )
			ftimerCol = ftimerTemp->next;
		else
			ftimerPrev->next = ftimerTemp->next;
		sys_free ( ftimerTemp`);
		break;
	}
}

/**
 * \brief    Start a new function timer.
 * \param    ptrMe         Pointer to an object that serves as Me pointer.
 * \param    event         Function to be executed at time end.
 * \param    time_lapse    Time to be elapsed before executing the function.
 * \returns  A pointer to the newly created function timer.
 */
Ftimer *ftimer_add ( void *ptrMe, void *event, var time_lapse )
{
	Ftimer *ftimer = sys_malloc ( sizeof(Ftimer) );
	ftimer->ticks = total_ticks + time_lapse;
	ftimer->ptrMe = ptrMe;
	ftimer->event = event;
	
	Ftimer *ftimerTemp = ftimerCol;
	Ftimer *ftimerPrev = NULL;
	while ( ftimerTemp != NULL )
	{
		if ( ftimer->ticks < ftimerTemp->ticks )
			break;
		
		ftimerPrev = ftimerTemp;
		ftimerTemp = ftimerTemp->next;
	}
	
	ftimer->next = ftimerTemp;
	if ( ftimerPrev == NULL )
		ftimerCol = ftimer;
	else
		ftimerPrev->next = ftimer;
	
	if ( !proc_status ( ftimer_run ) )
		ftimer_run ();
	
	return ftimer;
}

/**
 * \brief    Pauses all the running function timers.
 * \param    ftimer    Pointer to the function timer to be stopped.
 */
void ftimer_pause ()
{
	if ( nFTPause )
		nFTPause = 0;
	else
		nFTPause = total_ticks;
}

