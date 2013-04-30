#ifndef _WEATHER_C_
#define_WEATHER_C_

void set_sun_position(var _day,var _hour,var _minute,var _second,var _latitude,var _springEquinox)
{
	var B=(360/EARTH_ORBITAL_PERIOD) * (_day-_springEquinox);
	
	//Equation of Time (EoT)
	var EoT=9.87*sinv(2*B) - 7.53*cosv(B) - 1.5*sinv(B);
	
	//Time Correction Factor (TC)
	var TC=EoT;
	
	//Local Time (LT)
	var LT=_hour + (_minute/60.0) + (_second/3600.0);
	
	//Local Solar Time (LST)
	var LST=LT + TC/60;
	
	//Hour Angle (HRA)
	var HRA=15 * (LST - 12);
	
	//Number of days since the start of the year (d)
	var d=_day + LT/24;
	
	//Declination (D)
	var D=EARTH_AXIAL_TILT * sinv((360/EARTH_ORBITAL_PERIOD) * (d - _springEquinox));
	
	//Sun Elevation (sun_angle.tilt)
	sun_angle.tilt=asinv((cosv(HRA) * cosv(D) * cosv(_latitude)) + (sinv(D) * sinv(_latitude)));
	
	//Sun Azimuth (sun_angle.pan)
	sun_angle.pan=acosv( (sinv(D) - sinv(sun_angle.tilt) * sinv(_latitude)) / (cosv(sun_angle.tilt) * cosv(_latitude)) );
	if(LST>12 && HRA >0){sun_angle.pan=360-sun_angle.pan;}
}

#endif