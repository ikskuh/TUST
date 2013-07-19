#include <acknex.h>
#include <default.c>

#define PRAGMA_POINTER

#include "..\\Source\\function_timer.h"

SOUND *sndSnare = "..\\Ressources\\Audio\\Sounds\\snaredrum.wav";
SOUND *sndBass = "..\\Ressources\\Audio\\Sounds\\bassdrum.wav";
SOUND *sndHihat = "..\\Ressources\\Audio\\Sounds\\hihat.wav";
SOUND *sndHihatOpen = "..\\Ressources\\Audio\\Sounds\\hihat_open.wav";

void fncSnare ( Ftimer *ftimer )
{
	SOUND *snd = ftimer->ptrMe;
	snd_play ( snd, 100, 0 );
}

int SDCounter = 0;
void fncSnareLoop ( Ftimer *ftimer )
{
	SOUND *snd = ftimer->ptrMe;
	snd_play ( snd, 100, 30 );
	ftimer_add ( snd, fncSnareLoop, 16 );
	
	if ( SDCounter%8 == 5 )
	{
		ftimer_add ( snd, fncSnare, 10 );
		if ( SDCounter%16 == 13 )
		{
			int i = 24;
			for ( ; i<38; i+=2 )
				ftimer_add ( snd, fncSnare, i );
		}
	}
	SDCounter += 1;
}

int HHCounter = 0;
void fncHihat ( Ftimer *ftimer )
{
	SOUND *snd = ftimer->ptrMe;
	snd_play ( snd, 30, 0 );
	if ( HHCounter%8 == 1 )
		ftimer_add ( sndHihatOpen, fncHihat, 8 );
	else 
		ftimer_add ( sndHihat, fncHihat, 8 );
	HHCounter += 1;
}

int BDTasa[6] = { 16, 4, 12, 12, 8, 12 };
int BDCounter = 0;
void fncBass ( Ftimer *ftimer )
{
	SOUND *snd = ftimer->ptrMe;
	snd_play ( snd, 100, -30 );
	ftimer_add ( snd, fncBass, BDTasa[BDCounter%6] );
	BDCounter += 1;
}

void main ()
{
//	fps_max = 32;
	wait(2);
	
	Ftimer *myHihat = ftimer_add ( sndHihat, fncHihat, 4 );
	Ftimer *mySnare = ftimer_add ( sndSnare, fncSnareLoop, 24 );
	Ftimer *myBass = ftimer_add ( sndBass, fncBass, 14 );
	
	on_space = ftimer_pause;
	
	while(!key_esc)
	{
		Ftimer *ftimerTemp = ftimerCol;
		int i = 0;
		while ( ftimerTemp != NULL )
		{
			draw_text ( str_for_num(NULL,ftimerTemp->ticks-ftimer_total_ticks), 10, 30+(i*30), COLOR_WHITE );
			switch ( ftimerTemp->event )
			{
				case fncBass:
					draw_text ( "Bass drum", 80, 30+(i*30), COLOR_BLUE ); break;
				case fncHihat:
					draw_text ( "Hihat", 80, 30+(i*30), COLOR_WHITE ); break;
				case fncSnareLoop:
				case fncSnare:
					draw_text ( "Snare drum", 80, 30+(i*30), COLOR_GREEN ); break;
			}
			ftimerTemp = ftimerTemp->next;
			i ++;
		}
		draw_text ( str_for_num(NULL,ftimer_total_ticks), 80, 0, COLOR_WHITE );
		draw_text ( str_for_int(NULL,i), 10, 0, COLOR_WHITE );
		wait(1);
	}
	
	ftimer_stop_all ();
	
	sys_exit ( NULL );
}
