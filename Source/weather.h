#ifndef _WEATHER_H_
#define _WEATHER_H_

//#include "system.h"

/**
 * \file weather.h
 * Some easy to use weather functions.
 */
 
 VECTOR vecWindDirection;

#define DEGREE_IN_RADIANS 0.0174532925
#define EARTH_AXIAL_TILT 23.5			//earth axial tilt in degrees
#define EARTH_ORBITAL_PERIOD 365.25		//earth orbital period in days (length of a year)
#define EARTH_ROTATION_PERIOD 24.0		//earth rotation period in hours (length of a day)
#define LOCATION_TIME_SPRING_EQUINOX 81.0	//day number of the year when day and night cycle is exactly 12/12 hours
#define LOCATION_LATITUDE 48.0			//latitude of current location

/**
 * Sets the sun position to a geologically correct position. Thanks to oliver2s for this fantastic code.
 * \param	var	Day of the year
 * \param	var	Hour of day
 * \param	var	Minute of day
 * \param	var	Second of day
 * \param	var	Current position in latitude
 * \param	var	Day of the year regarding relation of night to day
 */
void set_sun_position(var _day,var _hour,var _minute,var _second,var _latitude,var _springEquinox);

/**
 * Simple version for setting day time
 * \param	var	Hour of day
 * \param	var	Minute of day
 */
void set_sun_position(var _hour,var _minute);




// Internal

// Lensflare

#define LENS_DISTANCE skill1

//void lensflareInitPosition();
void lensflare_init_flare(ENTITY* _entFlare);
void lensflare_place_flare(ENTITY* _entFlare);
void lensflare_create_lenses();
void lensflare_start();
void lensflare_stop();
void lensflare_toggle();
//void weatherChangeDayNight(int _dayTime);

// -1=Not initialized, 0=Not active, 1=active
var vLensFlareState = 0;
var vLensFlareAlpha = 70;
var vLensFlareFadeSpeed = 25;
var vLensFlareVisibility = 0;
var vCameraPan = 0;

VECTOR vecTemp1;
VECTOR vecTemp2;

ENTITY* entFlare1 =  NULL;
ENTITY* entFlare2 =  NULL;
ENTITY* entFlare4 =  NULL;
ENTITY* entFlare5 =  NULL;
ENTITY* entFlare6 =  NULL;
ENTITY* entFlare7 =  NULL;
ENTITY* entFlare8 =  NULL;
ENTITY* entFlare9 =  NULL;
ENTITY* entFlare10 = NULL;
ENTITY* entFlare11 = NULL;
ENTITY* entFlare12 = NULL;
ENTITY* entFlare13 = NULL;
ENTITY* entFlare14 = NULL;
ENTITY* entFlare15 = NULL;
ENTITY* entFlare16 = NULL;
ENTITY* entFlare17 = NULL;
ENTITY* entFlare18 = NULL;
ENTITY* entFlare19 = NULL;
ENTITY* entFlare20 = NULL;

// Weather

#define NIGHT 1
#define DAY 2
#define CHANGING 3

#define WEATHER_NONE 0
#define WEATHER_SNOW 1
#define WEATHER_RAIN 2


void pSnow(PARTICLE* p);
void pRain(PARTICLE* p);

void snow_start();
void snow_stop();
void snow_toggle();

void rain_start();
void rain_stop();
void rain_togle();

void clouds_start();
void clouds_stop();
void clouds_toggle();

void stop_fog();
void start_fog();

void set_sky_color(VECTOR* _color);

void init_weather();
void free_weather();

int get_day_time();
void day_night_change(int _dayTime);

void set_wind_direction(VECTOR* _dir);

BMAP* bmapRainDrop			= NULL;
SOUND* sndRain					= NULL;
SOUND* sndWind					= NULL;
SOUND* sndWolfHowling		= NULL;
SOUND* sndRoosterCrowing	= NULL;

var vWeatherSoundHandle;
int nWeatherType = -1;
ENTITY* entClouds = NULL;
ENTITY* entSkyDay = NULL;
ENTITY* entSkyNight = NULL;
int nWeatherDayTime = DAY;

#include "weather.c"

#endif