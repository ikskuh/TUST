#ifndef _WEATHER_H_
#define _WEATHER_H_

/**
 * \file weather.h
 * Some easy to use weather functions.
 */

#define EARTH_AXIAL_TILT 23.5	//earth axial tilt in degrees
#define EARTH_ORBITAL_PERIOD 365.25	//earth orbital period in days (length of a year)
#define EARTH_ROTATION_PERIOD 24.0	//earth rotation period in hours (length of a day)
#define LOCATION_TIME_SPRING_EQUINOX 81	//day number of the year when day and night cycle is exactly 12/12 hours
#define LOCATION_LATITUDE 48		//latitude of current location

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

#include "weather.c"

#endif