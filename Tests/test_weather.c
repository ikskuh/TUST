#include <acknex.h>
#include <default.c>
#include "..\\Source\\weather.h"

TEXT* txtDescription = {
	flags = SHOW | OUTLINE;
	pos_x = 10;
	pos_y = 10;
	font = "Arial#14";
	string("1 - Rain",
			 "2 - Snow",
			 "3 - Sun (Todo)",
			 "4 - Clouds",
			 "5 - Storm",
			 "6 - Day",
			 "7 - Night",
			 "8 - Fog");
	layer = 1;
}

void toggle_wind_speed() {
	clouds_start();
	if (vecWindDirection.x < 20) {
		set_wind_direction(vector(20,20,0));
	} else {
		set_wind_direction(vector(1,1,0));
	}
}

void start_day() { day_night_change(DAY); }
void start_night() { day_night_change(NIGHT); }

void main() {
	level_load("..\\Ressources\\Terrains\\small.hmp");
	
	init_weather();
	
	on_1 = rain_toggle;
	on_2 = snow_toggle;
	on_3 = lensflare_toggle;
	on_4 = clouds_toggle;
	on_5 = toggle_wind_speed;
	on_6 = start_day;
	on_7 = start_night;
	on_8 = start_fog;
	
	/*var day		= 0;
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
		
		draw_text("Day: ", 100, 10, vector(0,0,255));
		draw_text(str_for_num(NULL, day), 160, 10, vector(0,0,255));
		
		draw_text("Hour: ", 100, 30, vector(0,0,255));
		draw_text(str_for_num(NULL, hour), 160, 30, vector(0,0,255));
		
		
		set_sun_position(day,hour,minute,0,LOCATION_LATITUDE,LOCATION_TIME_SPRING_EQUINOX);
		//set_sun_position(hour, minute);
		wait(1);
	}*/
}