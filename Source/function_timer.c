
Ftimer *ftimerCol = NULL;
var nFTPause = 0;

void ftimer_run ()
{
	while (1)
	{
		while ( nFTPause == 0 )
		{
			Ftimer *ftimerTemp = ftimerCol;
			Ftimer *ftimerPrev = NULL;
			while ( ftimerCol != NULL )
			{
				if ( ftimerCol->ticks > total_ticks )
					break;
				
				ftimerPrev = ftimerCol;
				ftimerCol = ftimerCol->next;
			}
			if ( ftimerPrev != NULL )
			{
				ftimerPrev->next = NULL;
				while ( ftimerTemp != NULL )
				{
					ftimerPrev = ftimerTemp;
					ftimerTemp = ftimerTemp->next;
					ftimerPrev->event ( ftimerPrev );
					sys_free ( ftimerPrev );
				}
			}
			
			wait(1);
		}
		var nFTPauseOld = nFTPause;
		while ( nFTPause != 0 )
			wait(1);
		
		nFTPauseOld = total_ticks - nFTPauseOld;
		Ftimer *ftimerTemp = ftimerCol;
		while ( ftimerTemp != NULL )
		{
			ftimerTemp->ticks += nFTPauseOld;
			ftimerTemp = ftimerTemp->next;
		}
	}
}

void ftimer_stop_all ()
{
	while ( ftimerCol != NULL )
	{
		Ftimer *ftimerTemp = ftimerCol;
		ftimerCol = ftimerCol->next;
		sys_free ( ftimerTemp );
	}
}

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

void ftimer_pause ()
{
	if ( nFTPause )
		nFTPause = 0;
	else
		nFTPause = total_ticks;
}

