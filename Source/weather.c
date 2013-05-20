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

void lensflare_init_flare(ENTITY* _entFlare) {
	_entFlare.flags2 &= ~SHOW;
	set(_entFlare, BRIGHT | PASSABLE | TRANSLUCENT);
}

void lensflare_place_flare(ENTITY* _entFlare) {

	_entFlare.flags2 |= SHOW;
	vCameraPan = camera.pan;
	while (vCameraPan < 0) vCameraPan +=360;
	while (vCameraPan > 360) vCameraPan -=360;
	_entFlare.x = vecTemp1.x*_entFlare.LENS_DISTANCE + 0.5*screen_size.x;
	_entFlare.y = vecTemp1.y*_entFlare.LENS_DISTANCE + 0.5*screen_size.y;
	_entFlare.z = 1000;
	vec_set(vecTemp2,sun_pos);
	you = player;
	c_trace(camera.x,vecTemp2.x,IGNORE_ME | IGNORE_YOU | IGNORE_PASSABLE | IGNORE_SPRITES | SCAN_TEXTURE);
	if(HIT_TARGET) {
		if(_entFlare.alpha > 0) {
			_entFlare.alpha -= vLensFlareFadeSpeed * time_step;
		} else {
			_entFlare.alpha = 0;
		}
	} else {
		if((vCameraPan > sun_angle.pan + 25) || (vCameraPan < sun_angle.pan - 25) || (camera.tilt < sun_angle.tilt - 20) || (camera.tilt > sun_angle.tilt + 20)) {
			if(_entFlare.alpha > 0) {
				_entFlare.alpha -= vLensFlareFadeSpeed * time_step;
			} else {
				_entFlare.alpha = 0;
			}
		} else {
			vecTemp2.x = sqrt(abs(((_entFlare.x - (0.5*screen_size.x))*(_entFlare.x - (0.5*screen_size.x)))+((_entFlare.y - (0.5*screen_size.y))*(_entFlare.y - (0.5*screen_size.y)))));
			if(abs(vecTemp2.x) > (0.5*screen_size.x)) {
				_entFlare.alpha = vLensFlareAlpha - (abs(vecTemp2.x) - (0.5*screen_size.x));
				if(_entFlare.alpha < 0) {
					_entFlare.alpha = 0;
				}
			} else {
				if(_entFlare.alpha < vLensFlareAlpha) {
					_entFlare.alpha += vLensFlareFadeSpeed * time_step;
				} else {
					_entFlare.alpha = vLensFlareAlpha;
				}
			}
		}
	}
	_entFlare.alpha = clamp(_entFlare.alpha, 0, vLensFlareAlpha);
	rel_for_screen(_entFlare.x,camera);
}

void lensflare_create_lenses() {
	entFlare1 = ent_createlayer("graphics\\weather\\lens01.tga", 0, 8); entFlare1.client_id = camera; entFlare1.scale_x = 2; entFlare1.scale_y = 2; entFlare1.alpha = 40;
	entFlare2 = ent_createlayer("graphics\\weather\\lens02.tga", 0, 8); entFlare2.client_id = camera; entFlare2.scale_x = 2; entFlare2.scale_y = 2;
	entFlare4 = ent_createlayer("graphics\\weather\\lens04.tga", 0, 8); entFlare4.client_id = camera; entFlare4.scale_x = 2; entFlare4.scale_y = 2;
	entFlare5 = ent_createlayer("graphics\\weather\\lens05.tga", 0, 8); entFlare5.client_id = camera; entFlare5.scale_x = 2; entFlare5.scale_y = 2;
	entFlare6 = ent_createlayer("graphics\\weather\\lens06.tga", 0, 8); entFlare6.client_id = camera; entFlare6.scale_x = 2; entFlare6.scale_y = 2;
	entFlare7 = ent_createlayer("graphics\\weather\\lens07.tga", 0, 8); entFlare7.client_id = camera; entFlare7.scale_x = 2; entFlare7.scale_y = 2;
	entFlare8 = ent_createlayer("graphics\\weather\\lens08.tga", 0, 8); entFlare8.client_id = camera; entFlare8.scale_x = 2; entFlare8.scale_y = 2;
	entFlare9 = ent_createlayer("graphics\\weather\\lens09.tga", 0, 8); entFlare9.client_id = camera; entFlare9.scale_x = 2; entFlare9.scale_y = 2;
	entFlare10 = ent_createlayer("graphics\\weather\\lens10.tga", 0, 8); entFlare10.client_id = camera; entFlare10.scale_x = 2; entFlare10.scale_y = 2;
	entFlare11 = ent_createlayer("graphics\\weather\\lens11.tga", 0, 8); entFlare11.client_id = camera; entFlare11.scale_x = 2; entFlare11.scale_y = 2;
	entFlare12 = ent_createlayer("graphics\\weather\\lens12.tga", 0, 8); entFlare12.client_id = camera; entFlare12.scale_x = 2; entFlare12.scale_y = 2;
	entFlare13 = ent_createlayer("graphics\\weather\\lens13.tga", 0, 8); entFlare13.client_id = camera; entFlare13.scale_x = 2; entFlare13.scale_y = 2;
	entFlare14 = ent_createlayer("graphics\\weather\\lens14.tga", 0, 8); entFlare14.client_id = camera; entFlare14.scale_x = 2.5; entFlare14.scale_y = 2.5;
	entFlare15 = ent_createlayer("graphics\\weather\\lens15.tga", 0, 8); entFlare15.client_id = camera; entFlare15.scale_x = 2; entFlare15.scale_y = 2;
	entFlare16 = ent_createlayer("graphics\\weather\\lens16.tga", 0, 8); entFlare16.client_id = camera; entFlare16.scale_x = 2; entFlare16.scale_y = 2;
	entFlare17 = ent_createlayer("graphics\\weather\\lens17.tga", 0, 8); entFlare17.client_id = camera; entFlare17.scale_x = 2; entFlare17.scale_y = 2;
	entFlare18 = ent_createlayer("graphics\\weather\\lens18.tga", 0, 8); entFlare18.client_id = camera; entFlare18.scale_x = 2; entFlare18.scale_y = 2;
	entFlare19 = ent_createlayer("graphics\\weather\\lens19.tga", 0, 8); entFlare19.client_id = camera; entFlare19.scale_x = 2; entFlare19.scale_y = 2;
	entFlare20 = ent_createlayer("graphics\\weather\\lens20.tga", 0, 8); entFlare20.client_id = camera; entFlare20.scale_x = 2.8; entFlare20.scale_y = 2.8;

	lensflare_init_flare(entFlare1);
	lensflare_init_flare(entFlare2);
	lensflare_init_flare(entFlare4);
	lensflare_init_flare(entFlare5);
	lensflare_init_flare(entFlare6);
	lensflare_init_flare(entFlare7);
	lensflare_init_flare(entFlare8);
	lensflare_init_flare(entFlare9);
	lensflare_init_flare(entFlare10);
	lensflare_init_flare(entFlare11);
	lensflare_init_flare(entFlare12);
	lensflare_init_flare(entFlare13);
	lensflare_init_flare(entFlare14);
	lensflare_init_flare(entFlare15);
	lensflare_init_flare(entFlare16);
	lensflare_init_flare(entFlare17);
	lensflare_init_flare(entFlare18);
	lensflare_init_flare(entFlare19);
	lensflare_init_flare(entFlare20);

	vLensFlareState = 0;
}

void lensflare_start() {
	if (get_day_time() == NIGHT) {
		sun_angle.pan = 235;
		sun_angle.tilt = 6;
	} else {
		sun_angle.pan = 60;
		sun_angle.tilt = 25;
	}

	if(vLensFlareState == 1) {
		return;
	}

	wait(1);
	vLensFlareState = 1;
	camera.ambient = 30;
	set_sky_color(vector(230,230,230));

	while(vLensFlareState == 1) {
		vec_set(vecTemp1,sun_pos);		
		vec_to_screen(vecTemp1,camera);
		
		vecTemp1.x -= 0.5 * screen_size.x;
		vecTemp1.y -= 0.5 * screen_size.y;
		
		lensflare_place_flare(entFlare1);
		lensflare_place_flare(entFlare2);
		lensflare_place_flare(entFlare4);
		lensflare_place_flare(entFlare5);
		lensflare_place_flare(entFlare6);
		lensflare_place_flare(entFlare7);
		lensflare_place_flare(entFlare8);
		lensflare_place_flare(entFlare9);
		lensflare_place_flare(entFlare10);
		lensflare_place_flare(entFlare11);
		lensflare_place_flare(entFlare12);
		lensflare_place_flare(entFlare13);
		lensflare_place_flare(entFlare14);
		lensflare_place_flare(entFlare15);
		lensflare_place_flare(entFlare16);
		lensflare_place_flare(entFlare17);
		lensflare_place_flare(entFlare18);
		lensflare_place_flare(entFlare19);
		lensflare_place_flare(entFlare20);
		
		entFlare2.alpha = entFlare1.alpha;
		entFlare4.alpha = entFlare1.alpha;
		entFlare5.alpha = entFlare1.alpha;
		entFlare6.alpha = entFlare1.alpha;
		entFlare7.alpha = entFlare1.alpha;
		entFlare8.alpha = entFlare1.alpha;
		entFlare9.alpha = entFlare1.alpha;
		entFlare10.alpha = entFlare1.alpha;
		entFlare11.alpha = entFlare1.alpha;
		entFlare12.alpha = entFlare1.alpha;
		entFlare13.alpha = entFlare1.alpha;
		entFlare14.alpha = entFlare1.alpha;
		entFlare15.alpha = entFlare1.alpha;
		entFlare16.alpha = entFlare1.alpha;
		entFlare17.alpha = entFlare1.alpha;
		entFlare18.alpha = entFlare1.alpha;
		entFlare19.alpha = entFlare1.alpha;
		entFlare20.alpha = entFlare1.alpha;
		wait(1);
	}

	vLensFlareVisibility = 0;

	entFlare1.flags2 &=~SHOW;
	entFlare2.flags2 &=~SHOW;	
	entFlare4.flags2 &=~SHOW;
	entFlare5.flags2 &=~SHOW;
	entFlare6.flags2 &=~SHOW;
	entFlare7.flags2 &=~SHOW;
	entFlare8.flags2 &=~SHOW;
	entFlare9.flags2 &=~SHOW;
	entFlare10.flags2 &=~SHOW;
	entFlare11.flags2 &=~SHOW;
	entFlare12.flags2 &=~SHOW;
	entFlare13.flags2 &=~SHOW;
	entFlare14.flags2 &=~SHOW;
	entFlare15.flags2 &=~SHOW;
	entFlare16.flags2 &=~SHOW;
	entFlare17.flags2 &=~SHOW;
	entFlare18.flags2 &=~SHOW;
	entFlare19.flags2 &=~SHOW;
	entFlare20.flags2 &=~SHOW;
}

void lensflare_stop() {
	vLensFlareState = 0;
	set_sky_color(vector(200,200,200));
	camera.ambient = 0;
}

void lensflare_toggle() {
	snd_stop(vWeatherSoundHandle);
	fog_color = 0;
	nWeatherType = 4;
	switch(vLensFlareState) {
		case -1: lensflare_start(); break;
		case 0: lensflare_start(); break;
		case 1: lensflare_stop(); break;
	}
}

void pSnow(PARTICLE* p) {
	p.size = 4;
	p.alpha = 100;
	p.red = 128;
	p.green = 128;
	p.blue = 128;
	p.skill_c = 0.5;
	vec_add(p.x,vector(random(1000)-500, random(1000)-500,0));
	p.lifespan = 30;
	p.gravity = 0.2;
	p.flags = BRIGHT | TRANSLUCENT | MOVE;
	p.event = NULL;
}

void pRain(PARTICLE* p) {
	p.bmap = bmapRainDrop;
	p.alpha = 100;
	vec_add(p.x,vector(random(1000)-500, random(1000)-500,0));
	p.lifespan = 30;
	p.gravity = 7;
	p.flags = BRIGHT | TRANSLUCENT | MOVE;
	p.event = NULL;
}

void snow_start() {
	lensflare_stop();
	entClouds.u = 1;
	entClouds.v = 1;
	fog_color = 1;
	camera.fog_start = 10;
	camera.fog_end = 400;
	camera.ambient = -30;
	snd_stop(vWeatherSoundHandle);
	vWeatherSoundHandle = snd_loop(sndWind, 20, 0);
	VECTOR vecTemp;
	nWeatherType = WEATHER_SNOW;
	while(nWeatherType == WEATHER_SNOW) {
		vecTemp.x = camera.x + random(2)+(-1);
		vecTemp.y = camera.y + random(2)+(-1);
		vecTemp.z = camera.z + 200;
		effect(pSnow,maxv(1,random(40)),vecTemp,vector(0,0,-4));
		wait(1);
	}
}

void snow_stop() {
	if (nWeatherType == WEATHER_SNOW) {
		nWeatherType = WEATHER_NONE;
		fog_color = 0;
		camera.fog_start = 100;
		camera.fog_end = 1000;
		camera.ambient = 0;
		snd_stop(vWeatherSoundHandle);
	}
}

void snow_toggle() {
	if (nWeatherType == WEATHER_SNOW) {
		snow_stop();
	}  else {
		snow_start();
	}
}

void rain_start() {
	lensflare_stop();
	entClouds.u = 1;
	entClouds.v = 1;
	fog_color = 5;
	camera.fog_start = 500;
	camera.fog_end = 3000;
	camera.ambient = -10;
	set_sky_color(vector(100,100,100));
	snd_stop(vWeatherSoundHandle);
	vWeatherSoundHandle = snd_loop(sndRain, 20, 0);
	VECTOR vecTemp;
	nWeatherType = WEATHER_RAIN;
	while(nWeatherType == WEATHER_RAIN)
	{
		vecTemp.x = camera.x + random(2)+(-1);
		vecTemp.y = camera.y + random(2)+(-1);
		vecTemp.z = camera.z + 100;
		effect(pRain,maxv(1,random(40)),vecTemp,vector(0-vecWindDirection.x,0-vecWindDirection.y,-4-vecWindDirection.z));
		wait(1);
	}
}

void rain_stop() {
	if (nWeatherType == WEATHER_RAIN) {
		nWeatherType = WEATHER_NONE;
		fog_color = 0;
		camera.fog_start = 100;
		camera.fog_end = 1000;
		camera.ambient = 0;
		snd_stop(vWeatherSoundHandle);
		set_sky_color(vector(200,200,200));
	}
}

void rain_toggle() {
	if (nWeatherType == WEATHER_RAIN) {
		rain_stop();
	} else {
		rain_start();
	}
}

void clouds_start() {
	lensflare_stop();
	entClouds.flags2 |=SHOW;
	set_sky_color(vector(150,150,150));
}

void clouds_stop() {
	entClouds.flags2 &=~SHOW;
	set_sky_color(vector(200,200,200));
}

void clouds_toggle() {
	if (entClouds.flags2 & SHOW) {
		clouds_stop();		
	} else {
		clouds_start();
	}
}

void set_sky_color(VECTOR* _color) {
	vec_set(mtl_sky.ambient_blue, _color);
}

int get_day_time() {
	return nWeatherDayTime;
}

void day_night_toggle() {
	if (get_day_time() == NIGHT) {
		day_night_change(DAY);
	} else {
		day_night_change(NIGHT);
	}
}

void day_night_change(int _dayTime) {
	if (nWeatherDayTime == CHANGING) return;
	var vAmbientSub = 1.5;
	if (_dayTime == NIGHT) {
		
		#ifdef _SYSTEM_H_
			snd_play(sndWolfHowling, vGameEffectsVolume, 0);
		#else
			snd_play(sndWolfHowling, 100, 0);
		#endif
		nWeatherDayTime = CHANGING;
		set(entSkyNight, TRANSLUCENT);
		set(entSkyDay, TRANSLUCENT);
		lensflare_stop();
		clouds_stop();
		while(sun_light > 20) {
			sun_light -=1;
			set_sky_color(vector(mtl_sky.ambient_blue-vAmbientSub,mtl_sky.ambient_green-vAmbientSub,mtl_sky.ambient_red-vAmbientSub));
			entSkyDay.alpha -=1;
			entSkyNight.alpha +=1;
			wait(5);
		}
		reset(entSkyNight, TRANSLUCENT);
		set(entSkyDay, TRANSLUCENT);
		sky_cube_level = entSkyDay;
		nWeatherDayTime = NIGHT;
	} else {
		
		#ifdef _SYSTEM_H
			snd_play(sndRoosterCrowing, vGameEffectsVolume, 0);
		#else
			snd_play(sndRoosterCrowing, 100, 0);
		#endif
		nWeatherDayTime = CHANGING;
		set(entSkyNight, TRANSLUCENT);
		set(entSkyDay, TRANSLUCENT);
		while(sun_light < 100) {
			sun_light +=1;
			set_sky_color(vector(mtl_sky.ambient_blue+vAmbientSub,mtl_sky.ambient_green+vAmbientSub,mtl_sky.ambient_red+vAmbientSub));
			entSkyNight.alpha -=1;
			entSkyDay.alpha +=1;
			wait(5);
		}
		set(entSkyNight, TRANSLUCENT);
		reset(entSkyDay, TRANSLUCENT);
		sky_cube_level = entSkyNight;
		nWeatherDayTime = DAY;
	}
}

void start_fog() {
	fog_color = 1;
	camera.fog_start = 100;
	camera.fog_end = 5000;
}

void stop_fog() {
	fog_color = 0;
	camera.fog_start = 100;
	camera.fog_end = 5000;
}

void set_wind_direction(VECTOR* _dir) {
	vec_set(vecWindDirection, _dir);
	if (entClouds != NULL) {
		entClouds.u = vecWindDirection.x;
		entClouds.v = vecWindDirection.y;
	}
}

void init_weather() {
	entClouds = ent_createlayer("..\\Ressources\\Graphics\\clouds.tga", SKY | DOME, 3);
	entClouds.u = 1;
	entClouds.v = 1;
	clouds_stop();

	entSkyDay = ent_createlayer("..\\Ressources\\Graphics\\skyday+6.tga", SKY | CUBE, 1);
	if (d3d_shaderversion > 0) entSkyDay.material = mtl_sky;
	entSkyDay.alpha = 100;
	entSkyNight = ent_createlayer("..\\Ressources\\Graphics\\skynight+6.tga", SKY | CUBE, 1);
	if (d3d_shaderversion > 0) entSkyNight.material = mtl_sky;
	entSkyNight.alpha = 100;
	sky_cube_level = entSkyDay;

	sun_angle.pan = 60;
	sun_angle.tilt = 25;

	lensflare_create_lenses();
	
	entFlare1.LENS_DISTANCE = 1.278;
	entFlare2.LENS_DISTANCE = 1.200;
   entFlare4.LENS_DISTANCE = 0.522;
	entFlare5.LENS_DISTANCE = 0.434;
	entFlare6.LENS_DISTANCE = 0.348;
	entFlare7.LENS_DISTANCE = 0.306;
	entFlare8.LENS_DISTANCE = 0.262;	
	entFlare9.LENS_DISTANCE = 0.174;
	entFlare10.LENS_DISTANCE = 0.114;
	entFlare11.LENS_DISTANCE = 0.044;
	entFlare12.LENS_DISTANCE = 0.002;
	entFlare13.LENS_DISTANCE = -0.128;
   entFlare14.LENS_DISTANCE = -0.258;
	entFlare15.LENS_DISTANCE = -0.304;
	entFlare16.LENS_DISTANCE = -0.372;
	entFlare17.LENS_DISTANCE = -0.390;
   entFlare18.LENS_DISTANCE = -0.432;
	entFlare19.LENS_DISTANCE = -0.654;
	entFlare20.LENS_DISTANCE = -1.000;
	
	bmapRainDrop		= bmap_create("..\\Ressources\\Graphics\\rain.tga");
	sndRain				= snd_create("..\\Ressources\\Audio\\Sounds\\rain.wav");
	sndWind				= snd_create("..\\Ressources\\Audio\\Sounds\\wind.wav");
	sndWolfHowling		= snd_create("..\\Ressources\\Audio\\Sounds\\wolfHowling.ogg");
	sndRoosterCrowing	= snd_create("..\\Ressources\\Audio\\Sounds\\roosterCrowing.ogg");
}

void free_weather() {
	if (entClouds != NULL) {
		ptr_remove(entClouds);
		entClouds = NULL;
	}
	if (entSkyDay != NULL) {
		ptr_remove(entSkyDay);
		entSkyDay = NULL;
	}
	if (entSkyNight != NULL) {
		ptr_remove(entSkyNight);
		entSkyNight = NULL;
	}
	if (bmapRainDrop != NULL) {
		ptr_remove(bmapRainDrop);
		bmapRainDrop = NULL;
	}
	if (sndRain != NULL) {
		ptr_remove(sndRain);
		sndRain = NULL;
	}
	if (sndWind != NULL) {
		ptr_remove(sndWind);
		sndWind = NULL;
	}
}

#endif