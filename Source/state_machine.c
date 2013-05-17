
#define STMC_ACTIVE					(1<<8)
#define STMC_DELETE					(1<<9)
#define STMC_INDEX					(1<<10)

typedef struct STMCOL
{
// private:
	STMACHINE *stmFirst;
	int count;
	int capacity;
	var flags;
} STMCOL;

STMCOL *gblMachines = NULL;
STMCOL *gblNewMachines = NULL;

STMCOL *stmcol_create ()
{
	STMCOL *stmcol = sys_malloc(sizeof(STMCOL));
	stmcol->stmFirst = sys_malloc(sizeof(STMACHINE)*STMC_MEMSTEP);
	stmcol->count = 0;
	stmcol->capacity = STMC_MEMSTEP;
	return stmcol;
}

void *stm_add ( void *obj, void *remover, void *fnc, var state, int *index )
{
	#ifdef STMC_SAFEMODE
		if ( fnc == NULL )
		{
			#ifdef STMC_ERRORS
				error ( "null function in stm_add" );
			#endif
			return;
		}
	#endif
	if ( gblNewMachines->count == gblNewMachines->capacity )
	{
		int old_capacity = gblNewMachines->capacity;
		gblNewMachines->capacity += STMC_MEMSTEP;
		STMACHINE *stmTemp = gblNewMachines->stmFirst;
		gblNewMachines->stmFirst = sys_malloc(sizeof(STMACHINE)*gblNewMachines->capacity);
		memcpy ( gblNewMachines->stmFirst, stmTemp, sizeof(STMACHINE)*old_capacity );
		sys_free ( stmTemp );
	}
	STMACHINE *stmTemp = gblNewMachines->stmFirst + gblNewMachines->count;
	stmTemp->flags = STMC_ACTIVE;
	stmTemp->ptrMe = obj;
	stmTemp->remover = remover;
	if ( remover != NULL )
		stmTemp->flags |= STMC_DELETE;
	stmTemp->fncPtr = fnc;
	stmTemp->fncNext = fnc;
	stmTemp->state = state;
	stmTemp->stateNext = state;
	if ( index == NULL )
	{
		index = sys_malloc(sizeof(int));
		stmTemp->flags |= STMC_INDEX;
	}
	*index = gblNewMachines->count;
	stmTemp->index = index;
	gblNewMachines->count++;
	return obj;
}

void stm_remove ( STMCOL *stmcol )
{
	#ifdef STMC_SAFE_MODE
		if ( stmcol == NULL ) 
		{
			#ifdef STMC_ERRORS
				error ( "null stmcollection in stm_remove" );
			#endif
			return;
		}
	#endif
	STMACHINE *stmTemp = stmcol->stmFirst;
	STMACHINE *stmTempLast = stmTemp + stmcol->count;
	for ( ; stmTemp<stmTempLast; stmTemp++ )
	{
		if ( stmTemp->ptrMe != NULL )
			if ( stmTemp->flags & STMC_DELETE )
				stmTemp->remover ( stmTemp->ptrMe );
		
		sys_free ( stmTemp );
	}
	sys_free ( stmcol );
}

void stm_set_state ( STMACHINE *stm, void *fnc, var state )
{
	stm->fncNext = fnc;
	stm->stateNext = state;
}

void stm_set_me ( STMACHINE *stm, void *obj, void *remover )
{
	stm->ptrMe = obj;
	stm->remover = remover;
	if ( remover != NULL )
		stm->flags |= STMC_DELETE;
	else
		stm->flags &= ~STMC_DELETE;
}

void stm_stop_machine ( STMACHINE *stm )
{
	stm->flags &= ~STMC_ACTIVE;
}

void stm_stop_ptr ( void *ptr )
{
	STMACHINE *stmTemp = gblMachines->stmFirst;
	STMACHINE *stmTempLast = stmTemp + gblMachines->count;
	for ( ; stmTemp<stmTempLast; stmTemp++ )
	{
		if ( stmTemp->ptrMe == ptr )
			stm_set_state ( stmTemp, stm_stop_machine, 0 );
	}
}

void stm_open ()
{
	gblMachines = stmcol_create ();
	gblNewMachines = stmcol_create ();
	gblMachines->flags |= STMC_ACTIVE;
	while ( gblMachines->flags & STMC_ACTIVE )
	{
		proc_mode = PROC_LATE;
		#ifdef STMC_COUNT
			draw_text ( str_for_int(NULL,gblMachines->count), 400, 20, COLOR_WHITE );
			draw_text ( str_for_int(NULL,gblMachines->capacity), 400, 40, COLOR_WHITE );
			draw_text ( str_for_int(NULL,gblNewMachines->count), 460, 20, COLOR_WHITE );
			draw_text ( str_for_int(NULL,gblNewMachines->capacity), 460, 40, COLOR_WHITE );
		#endif
		STMACHINE *stmTemp = gblMachines->stmFirst;
		STMACHINE *stmTempLast = stmTemp + gblMachines->count;
		for ( ; stmTemp<stmTempLast; stmTemp++ )
		{
			if ( stmTemp->flags & STMC_ACTIVE )
				stmTemp->fncPtr ( stmTemp );
		}
		int tail = 0;
		for ( stmTemp=gblMachines->stmFirst; stmTemp<stmTempLast; stmTemp++ )
		{
			if ( stmTemp->flags & STMC_ACTIVE )
			{
				stmTemp->fncPtr = stmTemp->fncNext;
				stmTemp->state = stmTemp->stateNext;
				if ( tail == 0 )
					continue;
				int ibytes = (long)stmTempLast - (long)stmTemp;
				//int ibytes = (long)(gblMachines->stmFirst+gblMachines->capacity) - (long)stmTemp;
				memcpy ( stmTemp-tail, stmTemp, ibytes ); // It copies to a previous memory overlapped for sure ;)
				stmTemp -= tail;
				stmTempLast -= tail;
				gblMachines->count -= tail;
				STMACHINE *stmTemp2 = stmTemp;
				for ( ; stmTemp2<stmTempLast; stmTemp2++ )
					*stmTemp2->index -= tail;
				tail = 0;
				continue;
			}
			if ( stmTemp->ptrMe != NULL )
			{
				if ( stmTemp->flags & STMC_DELETE )
				{
					stmTemp->remover ( stmTemp->ptrMe );
					stmTemp->flags &= ~STMC_DELETE;
				}
				stmTemp->ptrMe = NULL;
			}
			if ( stmTemp->flags & STMC_INDEX )
			{
				sys_free ( stmTemp->index );
				stmTemp->flags &= ~STMC_INDEX;
			}
			tail ++;
		}
		gblMachines->count -= tail;
		if ( gblNewMachines->count > 0 )
		{
			if ( gblMachines->count + gblNewMachines->count > gblMachines->capacity )
			{
				int old_capacity = gblMachines->capacity;
				gblMachines->capacity += gblNewMachines->count + STMC_MEMSTEP;
				STMACHINE *stmTemp = gblMachines->stmFirst;
				gblMachines->stmFirst = sys_malloc(sizeof(STMACHINE)*gblMachines->capacity);
				memcpy ( gblMachines->stmFirst, stmTemp, sizeof(STMACHINE)*old_capacity );
				sys_free ( stmTemp );
			}
			stmTemp = gblMachines->stmFirst + gblMachines->count;
			int ibytes = sizeof(STMACHINE)*gblNewMachines->count;
			memcpy ( stmTemp, gblNewMachines->stmFirst, ibytes );
			memset ( gblNewMachines->stmFirst, 0, ibytes );
			gblMachines->count += gblNewMachines->count;
			gblNewMachines->count = 0;
		}
		wait(1);
	}
	stm_remove ( gblMachines );
	stm_remove ( gblNewMachines );
}

void stm_close ()
{
	gblMachines->flags &= ~STMC_ACTIVE;
}
