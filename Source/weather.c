#ifndef _WEATHER_C_
#define_WEATHER_C_

void set_sun_position(var _day,var _hour,var _minute,var _second,var _latitude,var _springEquinox)
{
	double B=(360.0/EARTH_ORBITAL_PERIOD) * (_day-_springEquinox);

	//Equation of Time (EoT)
	double EoT=9.87*sin(2*B*DEGREE_IN_RADIANS) - 7.53*cos(B*DEGREE_IN_RADIANS) - 1.5*sin(B*DEGREE_IN_RADIANS);

	//Time Correction Factor (TC)
	double TC=EoT;

	//Local Time (LT)
	double LT=_hour + (_minute/60.0) + (_second/3600.0);

	//Local Solar Time (LST)
	double LST=LT + TC/60.0;

	//Hour Angle (HRA)
	double HRA=15.0 * (LST - 12.0);

	//Number of days since the start of the year (d)
	double d=_day + LT/24.0;

	//Declination (D)
	double D=EARTH_AXIAL_TILT * sin((360.0/EARTH_ORBITAL_PERIOD) * (d - _springEquinox) * DEGREE_IN_RADIANS);

	//Sun Elevation (sun_angle.tilt)
	sun_angle.tilt=asin(cos(HRA*DEGREE_IN_RADIANS)*cos(D*DEGREE_IN_RADIANS)*cos(_latitude*DEGREE_IN_RADIANS)+sin(D*DEGREE_IN_RADIANS)*sin(_latitude*DEGREE_IN_RADIANS))/DEGREE_IN_RADIANS;

	//Sun Azimuth (sun_angle.pan)
	sun_angle.pan=acos((sin(D*DEGREE_IN_RADIANS)-sin(sun_angle.tilt*DEGREE_IN_RADIANS)*sin(_latitude*DEGREE_IN_RADIANS))/(cos(sun_angle.tilt*DEGREE_IN_RADIANS)*cos(_latitude*DEGREE_IN_RADIANS)))/DEGREE_IN_RADIANS;
	if(LST>12.0 && HRA >0.0){sun_angle.pan=360.0-sun_angle.pan;}
}

void set_sun_position(var _hour,var _minute) {
	set_sun_position(81, _hour, _minute, 0, LOCATION_LATITUDE, LOCATION_TIME_SPRING_EQUINOX);
}

#endif