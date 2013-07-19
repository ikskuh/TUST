
Ftimer *ftimerCol = NULL;
var nFTPause = 0;
var ftimer_total_ticks = 0;

void ftimer_run ()
{
	wait(1);
	while (1)
	{
		while ( nFTPause == 0 )
		{
			if ( time_step < 32 / fps_max )
				ftimer_total_ticks += time_step;
			if ( ftimer_total_ticks > 1000000 )
			{
				ftimer_total_ticks -= 1000000;
				Ftimer *ftimerTemp = ftimerCol;
				while ( ftimerTemp != NULL )
				{
					ftimerTemp->ticks -= 1000000;
					ftimerTemp = ftimerTemp->next;
				}
			}
			Ftimer *ftimerTemp = ftimerCol;
			Ftimer *ftimerPrev = NULL;
			while ( ftimerCol != NULL )
			{
				if ( ftimerCol->ticks > ftimer_total_ticks )
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
		while ( nFTPause != 0 )
			wait(1);
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
	ftimer->ticks = ftimer_total_ticks + time_lapse;
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
		nFTPause = ftimer_total_ticks;
}

