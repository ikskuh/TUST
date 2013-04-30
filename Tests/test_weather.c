#include <acknex.h>
#include "..\\Source\\weather.h"

void main() {
	level_load("..\\Ressources\\Terrains\\small.hmp");
	
	var day		= 0;
	var hour		= 0;
	var minute	= 0;
	
	while(1) {
		
		minute++;
		if (minute == 60) {
			minute = 0;
			hour++;
			if (hour == 24) {
				hour = 0;
				day++;
				if (day == 366) {
					day = 0;
				}
			}
		}
		
		draw_text("Day: ", 10, 10, vector(0,0,255));
		draw_text(str_for_num(NULL, day), 70, 10, vector(0,0,255));
		
		draw_text("Hour: ", 10, 30, vector(0,0,255));
		draw_text(str_for_num(NULL, hour), 70, 30, vector(0,0,255));
		
		
		set_sun_position(day,hour,minute,0,LOCATION_LATITUDE,LOCATION_TIME_SPRING_EQUINOX);
		//set_sun_position(hour, minute);
		wait(1);
	}
}