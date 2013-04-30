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
			if (hour == 25) {
				hour = 0;
				day++;
				if (day == 366) {
					day = 0;
				}
			}
		}
		
		set_sun_position(day,hour,minute,0,LOCATION_LATITUDE,LOCATION_TIME_SPRING_EQUINOX);
		wait(1);
	}
}