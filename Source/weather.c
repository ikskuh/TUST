#ifndef _WEATHER_C_
#define _WEATHER_C_

void lensflare_set_distance() {
	entFlare01.PIVOT_DIST = 1.278;
	entFlare02.PIVOT_DIST = 1.200;
   entFlare04.PIVOT_DIST = 0.522;
	entFlare05.PIVOT_DIST = 0.434;
	entFlare06.PIVOT_DIST = 0.348;
	entFlare07.PIVOT_DIST = 0.306;
	entFlare08.PIVOT_DIST = 0.262;	
	entFlare09.PIVOT_DIST = 0.174;
	entFlare10.PIVOT_DIST = 0.114;
	entFlare11.PIVOT_DIST = 0.044;
	entFlare12.PIVOT_DIST = 0.002;
	entFlare13.PIVOT_DIST = -0.128;
   entFlare14.PIVOT_DIST = -0.258;
	entFlare15.PIVOT_DIST = -0.304;
	entFlare16.PIVOT_DIST = -0.372;
	entFlare17.PIVOT_DIST = -0.390;
   entFlare18.PIVOT_DIST = -0.432;
	entFlare19.PIVOT_DIST = -0.654;
	entFlare20.PIVOT_DIST = -1.000;
}

void lensflare_init(ENTITY* _entFlare) {
	_entFlare.flags2 &= ~SHOW;
	set(_entFlare, BRIGHT | PASSABLE | TRANSLUCENT);
}

void lensflare_place(ENTITY* _entFlare) {

	_entFlare.flags2 |= SHOW;
	vCameraPan = camera.pan;
	while (vCameraPan < 0) vCameraPan +=360;
	while (vCameraPan > 360) vCameraPan -=360;
	_entFlare.x = vecTemp1.x*_entFlare.PIVOT_DIST + 0.5*screen_size.x;
	_entFlare.y = vecTemp1.y*_entFlare.PIVOT_DIST + 0.5*screen_size.y;
	_entFlare.z = 1000;
	vec_set(vecTemp2,sun_pos);
	you = player;
	c_trace(camera.x,vecTemp2.x,IGNORE_ME | IGNORE_YOU | IGNORE_PASSABLE | IGNORE_SPRITES | SCAN_TEXTURE);
	if(HIT_TARGET) {
		if(_entFlare.alpha > 0) {
			_entFlare.alpha -= nLensFlareFadeSpeed * time_step;
		} else {
			_entFlare.alpha = 0;
		}
	} else {
		if((vCameraPan > sun_angle.pan + 25) || (vCameraPan < sun_angle.pan - 25) || (camera.tilt < sun_angle.tilt - 20) || (camera.tilt > sun_angle.tilt + 20)) {
			if(_entFlare.alpha > 0) {
				_entFlare.alpha -= nLensFlareFadeSpeed * time_step;
			} else {
				_entFlare.alpha = 0;
			}
		} else {
			vecTemp2.x = sqrt(abs(((_entFlare.x - (0.5*screen_size.x))*(_entFlare.x - (0.5*screen_size.x)))+((_entFlare.y - (0.5*screen_size.y))*(_entFlare.y - (0.5*screen_size.y)))));
			if(abs(vecTemp2.x) > (0.5*screen_size.x)) {
				_entFlare.alpha = nLensFlareAlpha - (abs(vecTemp2.x) - (0.5*screen_size.x));
				if(_entFlare.alpha < 0) {
					_entFlare.alpha = 0;
				}
			} else {
				if(_entFlare.alpha < nLensFlareAlpha) {
					_entFlare.alpha += nLensFlareFadeSpeed * time_step;
				} else {
					_entFlare.alpha = nLensFlareAlpha;
				}
			}
		}
	}
	_entFlare.alpha = clamp(_entFlare.alpha, 0, nLensFlareAlpha);
	rel_for_screen(_entFlare.x,camera);
}

void lensflare_create() {
	lensflare_init(entFlare01);
	lensflare_init(entFlare02);
	lensflare_init(entFlare04);
	lensflare_init(entFlare05);
	lensflare_init(entFlare06);
	lensflare_init(entFlare07);
	lensflare_init(entFlare08);
	lensflare_init(entFlare09);
	lensflare_init(entFlare10);
	lensflare_init(entFlare11);
	lensflare_init(entFlare12);
	lensflare_init(entFlare13);
	lensflare_init(entFlare14);
	lensflare_init(entFlare15);
	lensflare_init(entFlare16);
	lensflare_init(entFlare17);
	lensflare_init(entFlare18);
	lensflare_init(entFlare19);
	lensflare_init(entFlare20);
	nLensFlare = 0;
}

void lensflare_start() {

	if(nLensFlare == -1) { lensflare_create(); } // Not initialized

	if(nLensFlare == 1) { return; } // Already running?

	wait(1);
	nLensFlare = 1;
	lensflare_set_distance();

	while(1) {
		while(nLensFlare == 0.5){wait(1);}
		
		vec_set(vecTemp1,sun_pos);
		
		vec_to_screen(vecTemp1,camera);
		
		vecTemp1.x -= 0.5 * screen_size.x;
		vecTemp1.y -= 0.5 * screen_size.y;
		
		lensflare_place(entFlare01);
		lensflare_place(entFlare02);
		lensflare_place(entFlare04);
		lensflare_place(entFlare05);
		lensflare_place(entFlare06);
		lensflare_place(entFlare07);
		lensflare_place(entFlare08);
		lensflare_place(entFlare09);
		lensflare_place(entFlare10);
		lensflare_place(entFlare11);
		lensflare_place(entFlare12);
		lensflare_place(entFlare13);
		lensflare_place(entFlare14);
		lensflare_place(entFlare15);
		lensflare_place(entFlare16);
		lensflare_place(entFlare17);
		lensflare_place(entFlare18);
		lensflare_place(entFlare19);
		lensflare_place(entFlare20);
		
		entFlare01.alpha = entFlare01.alpha - (entCloud3.alpha/10);
		entFlare02.alpha = entFlare01.alpha;
		entFlare04.alpha = entFlare01.alpha;
		entFlare05.alpha = entFlare01.alpha;
		entFlare06.alpha = entFlare01.alpha;
		entFlare07.alpha = entFlare01.alpha;
		entFlare08.alpha = entFlare01.alpha;
		entFlare09.alpha = entFlare01.alpha;
		entFlare10.alpha = entFlare01.alpha;
		entFlare11.alpha = entFlare01.alpha;
		entFlare12.alpha = entFlare01.alpha;
		entFlare13.alpha = entFlare01.alpha;
		entFlare14.alpha = entFlare01.alpha;
		entFlare15.alpha = entFlare01.alpha;
		entFlare16.alpha = entFlare01.alpha;
		entFlare17.alpha = entFlare01.alpha;
		entFlare18.alpha = entFlare01.alpha;
		entFlare19.alpha = entFlare01.alpha;
		entFlare20.alpha = entFlare01.alpha;
		wait(1);
	}

	// Remove lens flares
	//lensvisibility = 0;
	
	nLensFlare = 0;
}




void fade_colors(VECTOR* _vecColorTarget, VECTOR* _vecColor1, VECTOR* _vecColor2) {
	var i = 0;
	if(i < 100) {
		i = minv(i + 2*time_step, 100);
		vec_lerp(_vecColorTarget, _vecColor1, _vecColor2, i/100);
	}
}

void weather_set_particles() {
	while(1){
		
		if(nWeatherState == 1) {
			bmapWeatherParticle = bmapRain;
			vParticleVelocityX = vecRainDirection.x + ((vRainRandomMove/2)-random(vRainRandomMove));
			vParticleVelocityY = vecRainDirection.y + ((vRainRandomMove/2)-random(vRainRandomMove));	
			vParticleVelocityZ = -vecRainDirection.z;
			vParticleSize=random(2)+8;
			vParticleAlpha=random(10)+60;	
		}
		
		if(nWeatherState == 2) {
			bmapWeatherParticle = bmapSnow;
			vParticleVelocityX = vecSnowDirection.x + ((vSnowRandomMove/2)-random(vSnowRandomMove));
			vParticleVelocityY = vecSnowDirection.y + ((vSnowRandomMove/2)-random(vSnowRandomMove));	
			vParticleVelocityZ = -vecSnowDirection.z;
			vParticleSize=random(1)+2;
			vParticleAlpha=random(50)+40;	
		}
		wait(1);	
	}			
}

void weather_seed_particles(PARTICLE *p) {
	vec_set(p.x,vector(cycle(p.x,camera.x-vecParticleSeedBox.x,camera.x+vecParticleSeedBox.x),
	cycle(p.y,camera.y-vecParticleSeedBox.y,camera.y+vecParticleSeedBox.y),
	cycle(p.z,camera.z-vecParticleSeedBox.z,camera.z+vecParticleSeedBox.z)));
	if(p.z<camera.z-100 || p.z<nWaterLevel){p.lifespan=0;}
}

void weather_effect_seed(PARTICLE *p) {
	vec_set(p.x,vector(camera.x+random(vecParticleSeedBox.x*2)-vecParticleSeedBox.x,
	camera.y+random(vecParticleSeedBox.y*2)-vecParticleSeedBox.y,
	camera.z+random(vecParticleSeedBox.z*2)-vecParticleSeedBox.z));
	
	p.flags |= MOVE;
	
	if(entCloud3.alpha > 0) {
		p.bmap = bmapWeatherParticle;
		p.vel_x = vParticleVelocityX;
		p.vel_y = vParticleVelocityY;
		p.vel_z = vParticleVelocityZ;
		p.size = vParticleSize;
		p.alpha = vParticleAlpha;
		p.event  = weather_seed_particles;	
	}	
}

void weather_fade_lightning(PARTICLE *p) {
	p.lifespan = 0;
}

void weather_particle_lightning(PARTICLE *p) {
	VECTOR temp;
	temp.x = random(2) - 1;
	temp.y = random(2) - 1;
	temp.z = random(2) - 1;
	vec_set(p.vel_x, temp);
	p.bmap = bmapLightning;
	p.size = 3;
	p.flags |= (MOVE|BRIGHT|TRANSLUCENT);
	p.lifespan = 1;
	p.event  = weather_fade_lightning;
}

void weather_particle_segment() {
	vec_set(vLightningTemp, vecLightningSegmentEnd);
	vec_sub(vecLightningSegmentEnd, vecLightningSegmentStart);
	vLightningSegmentLength = vec_length(vecLightningSegmentEnd);
	vecLightningSegmentEnd.x = (vecLightningSegmentEnd.x * 2) / vLightningSegmentLength;
	vecLightningSegmentEnd.y = (vecLightningSegmentEnd.y * 2) / vLightningSegmentLength;
	vecLightningSegmentEnd.z = (vecLightningSegmentEnd.z * 2) / vLightningSegmentLength;
	while(vLightningSegmentLength > 0) {
		effect(weather_particle_lightning, 2, vecLightningSegmentStart.x, nullvector);	
		vec_add(vecLightningSegmentStart, vecLightningSegmentEnd);
		vLightningSegmentLength -= 2;
	}
}

void weather_lightning_effect() {
	vec_set(vLightningTemp, vecLightningStrokeStart);
	vec_sub(vecLightningStrokeEnd, vecLightningStrokeStart);
	vLightningStrokeLength = vec_length(vecLightningStrokeEnd);
	vecLightningStrokeEnd.x = (vecLightningStrokeEnd.x * 100) / vLightningStrokeLength;
	vecLightningStrokeEnd.y = (vecLightningStrokeEnd.y * 100) / vLightningStrokeLength;
	vecLightningStrokeEnd.z = (vecLightningStrokeEnd.z * 100) / vLightningStrokeLength;
	while(vLightningStrokeLength > 0) {
		vec_add(vecLightningStrokeStart, vecLightningStrokeEnd);
		vec_set(vecLightningSegmentStart, vLightningTemp);
		vec_set(vecLightningSegmentEnd, vecLightningStrokeStart);
		vecLightningSegmentEnd.x += random(60) - 30;
		vecLightningSegmentEnd.y += random(60) - 30;
		vecLightningSegmentEnd.z += random(60) - 30;
		weather_particle_segment();
		vLightningStrokeLength -= 100;
	}
}

void weather_increase_brightness() {
	
	nLightningIsOn = 1;
	nLightning = 255;
	wait(2+random(3));
	nLightning = 1;
	wait(2+random(2));
	nLightning = 255;
	wait(1+random(2));
	nLightning = 1;
	wait(2+random(2));
	nLightning = 255;
	wait(2+random(5));
	nLightning = 1;
	wait(random(50));	
	nLightningIsOn = 0;
}

void weather_thunder_strike() {
	var vSoundHandleThunder;
	var vRandomCount;
	var vRandomCountState;
	var vOuterRadius;
	var vInnerRadius;
	VECTOR vecLocalTemp;
	vOuterRadius = WEATHER_FOG_FAR;
	vInnerRadius = WEATHER_FOG_FAR-(WEATHER_FOG_FAR/8);
	while(1) {
		randomize();
		vRandomCount = integer(random(6));
			
	
		if(nWeatherState == 1 && vDisableThunder == 0) {	
			weather_increase_brightness();
			
			vec_set(vecLocalTemp,vector(vInnerRadius+random(vOuterRadius-vInnerRadius),0,0));
			vec_rotate(vecLocalTemp,vector(random(360),0,0));
			vec_add(vecLocalTemp,camera.x);
			vec_set(vecLightningStrokeStart,vecLocalTemp);
			vecLightningStrokeStart.z += 600;
			vec_set(vecLightningStrokeEnd,vector(vecLocalTemp.x+random(100)-200,vecLocalTemp.y+random(100)-200,vecLocalTemp.z));
			vecLightningStrokeEnd.z -= 200;
			
			weather_lightning_effect();
			while(nLightningIsOn>0) {
				wait(1);
			}
			
			if(vRandomCount == 1 && vRandomCountState != 1){
				vSoundHandleThunder = snd_play(sndThunder1, 100, 0);
				vRandomCountState = 1;
			}
			if(vRandomCount == 2 && vRandomCountState != 2) {
				vSoundHandleThunder = snd_play(sndThunder2, 100, 0);
				vRandomCountState = 2;
			}
			if(vRandomCount == 3 && vRandomCountState != 3) {
				vSoundHandleThunder = snd_play(sndThunder3, 100, 0);
				vRandomCountState = 3;	
			}
			if(vRandomCount == 4 && vRandomCountState != 4) {
				vSoundHandleThunder = snd_play(sndThunder4, 100, 0);
				vRandomCountState = 4;	
			}
			if(vRandomCount == 5 && vRandomCountState != 5) {
				vSoundHandleThunder = snd_play(sndThunder5, 100, 0);
				vRandomCountState = 5;	
			}	
		}
		
		wait(-random(5));
	}
	
}


void weather_random() {
	randomize();
	while(1) {
		if(nRandomWeather == 1) {
			var i;
			i = random(nRandomWeatherChangeFrequency);
			
			if(i>(nRandomWeatherChangeFrequency/1.3)) {
				nWeatherState = 1;
				wait(-(random(nRandomWeatherStateDuration/2)+nRandomWeatherStateDuration/2));	
			}
			if(i>(nRandomWeatherChangeFrequency/1.3)) {
				nWeatherState = 2;
				wait(-(random(nRandomWeatherStateDuration/2)+nRandomWeatherStateDuration/2));
			}	
			if(i>(nRandomWeatherChangeFrequency/6)) {
				nWeatherState = 0;
				wait(-(random(nRandomWeatherStateDuration/2)+nRandomWeatherStateDuration/2));
			}
		}	
		wait(1);
	}	
}

void weather_toggle() {
	nWeatherState += 1;

	if(nWeatherState > 2){
		nWeatherState = 0;
	}
}

void weather_change() {

	weather_set_particles();
	weather_thunder_strike();
	weather_random();

	var vSndHandleRain			= 0;
	var vSndHandleWind			= 0;
	var vSndHandleBgDay			= 0;
	var vSndHandleBgNight		= 0;
	var vSndHandleUnderwater	= 0;

	var vVolumeRain				= 2;
	var vVolumeWind				= 2;
	var vVolumeBgDay				= 2;
	var vVolumeBgNight			= 2;
	var vVolumeUnderwater		= 0;
	var vEffectDensity = 1;
	
	vSndHandleWind = snd_loop(sndWind,vVolumeWind,0);
	vSndHandleRain = snd_loop(sndRain,vVolumeRain,0);

	if(nAmbientSounds == 1) {
		vSndHandleBgDay = snd_loop(sndDay,vVolumeBgDay,0);
		vSndHandleBgNight = snd_loop(sndNight,vVolumeBgNight,0);
	}
	if(nUnderwaterSounds == 1) {
		vSndHandleUnderwater = snd_loop(sndUnderwater,vVolumeUnderwater,0);
	}
	
	nWeatherState = 0;
	
	
	while(1) {
		
		vecParticleSeedBox.x=WEATHER_BOX_X/2;
		vecParticleSeedBox.y=WEATHER_BOX_Y/2;
		vecParticleSeedBox.z=WEATHER_BOX_Z/2;
		
		if(nUnderwaterSounds == 1){
			if(camera.z > nWaterLevel) { // > 0
				vVolumeUnderwater = 2;	
			} else {
				vVolumeWind = 2;
				vVolumeRain = 2;
				vVolumeBgDay = 2;
				vVolumeBgNight = 2;
				vVolumeUnderwater = 100;
			}
		} else {
			snd_stop(vVolumeUnderwater);
		}	
		
		if(nWeatherState>0) {
			
			entCloud3.flags2 &=~SHOW;
			if (entCloud3.alpha < 90){
				entCloud3.alpha += 0.2*time_step;
			}
			
			if(entCloud3.alpha>40){
				if(vWeatherFader<100 && nWeatherState>0) {
					vWeatherFader += 10/20*time_step;
				}
				if(nWeatherState == 0) {
					if(vWeatherFader>1) {
						vWeatherFader -= 10/20*time_step;
					}
				}
				
				vEffectDensity = (vWeatherFader*vParticleNumber);
			}
			
			if(nWeatherState == 1) {
				
				vParticleNumber = RAIN_DENSITY;
				
				if(vVolumeBgDay>2 || vVolumeBgNight>2) {
					vVolumeBgDay -= 0.6*time_step;
					vVolumeBgNight -= 0.6*time_step;
				}	
				
				if(entCloud3.alpha>40) {
					
					if(vVolumeRain<50){vVolumeRain += 0.6*time_step;}
					if(vVolumeWind<10){vVolumeWind += 0.6*time_step;}
					else{vVolumeWind -= 0.6*time_step;}
					
					effect(weather_effect_seed,vEffectDensity,nullvector,nullvector);
				}
			}	
			
			if(nWeatherState == 2) {
				
				vParticleNumber = SNOW_DENSITY;
				
				if(entCloud3.alpha>40){
					
					if(vVolumeRain>2){vVolumeRain -= 0.6*time_step*2;}
					if(vVolumeWind<40){vVolumeWind += (vEffectDensity/50)*0.6*time_step;}
					
					if(vVolumeBgDay>2 || vVolumeBgNight>2){
						vVolumeBgDay -= 0.6*time_step;
						vVolumeBgNight -= 0.6*time_step;
					}
					
					effect(weather_effect_seed,vEffectDensity,nullvector,nullvector);
				}
			}
			
			if(nActiveTriggerId < 0) {
				
				if(camera.fog_start>WEATHER_FOG_NEAR) {
					camera.fog_start -= 10*time_step;
				} else {
					camera.fog_start += 10*time_step;
				}
				if(camera.fog_end>WEATHER_FOG_FAR){
					camera.fog_end -= 10*time_step;
				} else {
					camera.fog_end += 10*time_step;
				}
			}
		}
		
		if(nWeatherState == 0) {
			
			
			if(entCloud3.alpha > 1) {
				entCloud3.alpha -= 0.6*time_step;
			}
			if(entCloud3.alpha<1) {
				entCloud3.flags2 &=~SHOW;
			}
			
			if(entCloud3.alpha > 40) {
				
				if(vVolumeRain>2 || vVolumeWind>2) {
					vVolumeRain -= 0.6*time_step;
					vVolumeWind -= 0.6*time_step;
					effect(weather_effect_seed,vEffectDensity/4,nullvector,nullvector);
				}		
			}
			
			if(entCloud3.alpha < 60) {
				
				if(sun_angle.pan > 160 && sun_angle.pan < 360) {
					if(vVolumeBgNight<80){
						vVolumeBgNight += 0.6*time_step;
						if(vVolumeBgDay > 2) {
							vVolumeBgDay -= 0.6*time_step;
						}
					}			
				}
				
				if(sun_angle.pan>0 && sun_angle.pan<160) {
					
					if(vVolumeBgDay<80) {
						vVolumeBgDay += 0.6*time_step;
						if(vVolumeBgNight > 2){
							vVolumeBgNight -= 0.6*time_step;
						}
					}	
				}
			}
			
			if(nActiveTriggerId < 0) {
				if(camera.fog_start>LAND_FOG_NEAR) {
					camera.fog_start -= 10*time_step;
				} else {
					camera.fog_start += 10*time_step;
				}
				if(camera.fog_end>LAND_FOG_FAR) {
					camera.fog_end -= 10*time_step;
				} else {
					camera.fog_end += 10*time_step;
				}
			}	
		}
		
		entSkySun.alpha = clamp((90-entCloud3.alpha),2,100);
		snd_tune(vSndHandleRain,vVolumeRain,0,0);
		snd_tune(vSndHandleWind,vVolumeWind,0,0);	
		snd_tune(vSndHandleBgDay,vVolumeBgDay,0,0);
		snd_tune(vSndHandleBgNight,vVolumeBgNight,0,0);
		snd_tune(vSndHandleUnderwater,vVolumeUnderwater,0,0);
		
		wait(1);		
	}	
}


void fade_in_sky_day() {
	if (entCloud1.alpha < 80){entCloud1.alpha += 0.03*time_step*vDayTransitionTime;} 
	if (entCloud2.alpha < 60){entCloud2.alpha += 0.03*time_step*vDayTransitionTime;}
	if (entSkyDay.alpha < 60){entSkyDay.alpha += 0.03*time_step*vDayTransitionTime;}
	if (entSkyNight.alpha > 1){entSkyNight.alpha -= 0.03*time_step*vDayTransitionTime;}	
}

void set_sky_day() {
	entCloud1.alpha = 80;
	entCloud2.alpha = 60;
	entSkyDay.alpha = 60;
	entSkyNight.alpha = 1;
	//Todo Sound here
}

void fade_in_sky_night(){
	if (entCloud1.alpha > 10){entCloud1.alpha -= 0.03*time_step*vDayTransitionTime;} 
	if (entCloud2.alpha > 10){entCloud2.alpha -= 0.03*time_step*vDayTransitionTime;} 
	if (entSkyNight.alpha < 100){entSkyNight.alpha += 0.03*time_step*vDayTransitionTime;}
	if (entSkyDay.alpha > 1){entSkyDay.alpha -= 0.03*time_step*vDayTransitionTime;}	
}

void set_sky_night() {
	entCloud1.alpha = 10;
	entCloud2.alpha = 10;
	entSkyDay.alpha = 10;
	entSkyNight.alpha = 100;
}

void sky_create_resources() {
	
	bmapRain				= bmap_create("..\\Ressources\\Graphics\\rain.tga");
	bmapSnow				= bmap_create("..\\Ressources\\Graphics\\snow.tga");
	bmapLightning		= bmap_create("..\\Ressources\\Graphics\\lightning.tga");

	sndRain				= snd_create("..\\Ressources\\Audio\\Sounds\\rain.wav");
	sndWind				= snd_create("..\\Ressources\\Audio\\Sounds\\wind.wav");
	sndDay				= snd_create("..\\Ressources\\Audio\\Sounds\\ambiente_day.wav");
	sndNight				= snd_create("..\\Ressources\\Audio\\Sounds\\ambiente_night.wav");
	sndThunder1			= snd_create("..\\Ressources\\Audio\\Sounds\\thunder1.wav");
	sndThunder2			= snd_create("..\\Ressources\\Audio\\Sounds\\thunder2.wav");
	sndThunder3			= snd_create("..\\Ressources\\Audio\\Sounds\\thunder3.wav");
	sndThunder4			= snd_create("..\\Ressources\\Audio\\Sounds\\thunder4.wav");
	sndThunder5			= snd_create("..\\Ressources\\Audio\\Sounds\\thunder5.wav");
	sndUnderwater		= snd_create("..\\Ressources\\Audio\\Sounds\\underwater.wav");
	
	entHorizon			= ent_createlayer("..\\Ressources\\Graphics\\horizon.tga", SKY | SCENE | SHOW, 5); entHorizon.scale_x = 0.25; entHorizon.scale_y = 0.25; entHorizon.tilt = -60; entHorizon.alpha = 100; set(entHorizon, TRANSLUCENT);
	entCloud1			= ent_createlayer("..\\Ressources\\Graphics\\clouds.tga", SKY | DOME | SHOW, 2); entCloud1.scale_x = 1.5; entCloud1.scale_y = 1.5; entCloud1.tilt = -12; entCloud1.alpha = 80; set(entCloud1, TRANSLUCENT);
	entCloud2			= ent_createlayer("..\\Ressources\\Graphics\\clouds.tga", SKY | DOME | SHOW, 3); entCloud2.u = 1; entCloud2.v = 5; entCloud2.scale_x = 2.5; entCloud2.scale_y = 2.5; entCloud2.tilt = -10; entCloud2.alpha = 60; set(entCloud2, TRANSLUCENT);
	entCloud3			= ent_createlayer("..\\Ressources\\Graphics\\clouds_bad.tga", SKY | DOME | SHOW, 4); entCloud3.u = 1; entCloud3.v = 5; entCloud3.scale_x = 2; entCloud3.scale_y = 2; entCloud3.tilt = -10; entCloud3.alpha = 0; set(entCloud3, TRANSLUCENT);
	entSkyDay			= ent_createlayer("..\\Ressources\\Graphics\\sky_day.tga", SKY | SCENE | SHOW, 1); entSkyDay.scale_x = 0.25; entSkyDay.tilt = -20; vec_set(entSkyDay.blue, vector(255,255,255)); entSkyDay.alpha = 60; set(entSkyDay, TRANSLUCENT);
	entSkySun			= ent_createlayer("..\\Ressources\\Graphics\\sky_sun.tga", SKY | SHOW, 6); entSkySun.alpha = 65; set(entSkySun, TRANSLUCENT | BRIGHT);
	entSkySunCorona	= ent_createlayer("..\\Ressources\\Graphics\\sky_suncorona.tga", SKY | SHOW, 3); entSkySunCorona.alpha = 40; set(entSkySunCorona, TRANSLUCENT | BRIGHT);
	entSkySunshine		= ent_createlayer("..\\Ressources\\Graphics\\sky_sunshine.tga", SKY | SHOW, 7); entSkySunshine.alpha = 50; set(entSkySunshine, TRANSLUCENT | BRIGHT);
	entSkyNight			= ent_createlayer("..\\Ressources\\Graphics\\sky_night.tga", SKY | DOME | SHOW, 1); entSkyNight.alpha = 0; set(entSkyNight, TRANSLUCENT);
	entSkyMoon			= ent_createlayer("..\\Ressources\\Graphics\\sky_moon.tga", SKY | SHOW, 2); entSkyMoon.alpha = 100; set(entSkyMoon, TRANSLUCENT);
	
	entFlare01			= ent_createlayer("..\\Ressources\\Graphics\\lens01.tga", 0, 8); entFlare01.scale_x = 2; entFlare01.scale_y = 2; entFlare01.client_id = camera;
	entFlare02			= ent_createlayer("..\\Ressources\\Graphics\\lens02.tga", 0, 8); entFlare02.scale_x = 2; entFlare02.scale_y = 2; entFlare02.client_id = camera;
	entFlare04			= ent_createlayer("..\\Ressources\\Graphics\\lens04.tga", 0, 8); entFlare04.scale_x = 2; entFlare04.scale_y = 2; entFlare04.client_id = camera;
	entFlare05			= ent_createlayer("..\\Ressources\\Graphics\\lens05.tga", 0, 8); entFlare05.scale_x = 2; entFlare05.scale_y = 2; entFlare05.client_id = camera;
	entFlare06			= ent_createlayer("..\\Ressources\\Graphics\\lens06.tga", 0, 8); entFlare06.scale_x = 2; entFlare06.scale_y = 2; entFlare06.client_id = camera;
	entFlare07			= ent_createlayer("..\\Ressources\\Graphics\\lens07.tga", 0, 8); entFlare07.scale_x = 2; entFlare07.scale_y = 2; entFlare07.client_id = camera;
	entFlare08			= ent_createlayer("..\\Ressources\\Graphics\\lens08.tga", 0, 8); entFlare08.scale_x = 2; entFlare08.scale_y = 2; entFlare08.client_id = camera;
	entFlare09			= ent_createlayer("..\\Ressources\\Graphics\\lens09.tga", 0, 8); entFlare09.scale_x = 2; entFlare09.scale_y = 2; entFlare09.client_id = camera;
	entFlare10			= ent_createlayer("..\\Ressources\\Graphics\\lens10.tga", 0, 8); entFlare10.scale_x = 2; entFlare10.scale_y = 2; entFlare10.client_id = camera;
	entFlare11			= ent_createlayer("..\\Ressources\\Graphics\\lens11.tga", 0, 8); entFlare11.scale_x = 2; entFlare11.scale_y = 2; entFlare11.client_id = camera;
	entFlare12			= ent_createlayer("..\\Ressources\\Graphics\\lens12.tga", 0, 8); entFlare12.scale_x = 2; entFlare12.scale_y = 2; entFlare12.client_id = camera;
	entFlare13			= ent_createlayer("..\\Ressources\\Graphics\\lens13.tga", 0, 8); entFlare13.scale_x = 2; entFlare13.scale_y = 2; entFlare13.client_id = camera;
	entFlare14			= ent_createlayer("..\\Ressources\\Graphics\\lens14.tga", 0, 8); entFlare14.scale_x = 2.5; entFlare14.scale_y = 2.5; entFlare14.client_id = camera;
	entFlare15			= ent_createlayer("..\\Ressources\\Graphics\\lens15.tga", 0, 8); entFlare15.scale_x = 2; entFlare15.scale_y = 2; entFlare15.client_id = camera;
	entFlare16			= ent_createlayer("..\\Ressources\\Graphics\\lens16.tga", 0, 8); entFlare16.scale_x = 2; entFlare16.scale_y = 2; entFlare16.client_id = camera;
	entFlare17			= ent_createlayer("..\\Ressources\\Graphics\\lens17.tga", 0, 8); entFlare17.scale_x = 2; entFlare17.scale_y = 2; entFlare17.client_id = camera;
	entFlare18			= ent_createlayer("..\\Ressources\\Graphics\\lens18.tga", 0, 8); entFlare18.scale_x = 2; entFlare18.scale_y = 2; entFlare18.client_id = camera;
	entFlare19			= ent_createlayer("..\\Ressources\\Graphics\\lens19.tga", 0, 8); entFlare19.scale_x = 2; entFlare19.scale_y = 2; entFlare19.client_id = camera;
	entFlare20			= ent_createlayer("..\\Ressources\\Graphics\\lens20.tga", 0, 8); entFlare20.scale_x = 2.8; entFlare20.scale_y = 2.8; entFlare20.client_id = camera;
}

void sky_init() {
   
   VECTOR vecTemp;
	wait(10);
	
	vec_set(mtl_sky.ambient_blue, vector(1,1,1));
	
	sky_create_resources();

	// If no lensflare desired --> comment out!
	lensflare_start();

	weather_change();

	//camera.arc = 60;

	sky_curve = 2;
	sky_clip = -10;

	sun_light = 100;

	entSkySun.scale_x = 3;
	entSkySun.scale_y = 3;

	camera.fog_start = LAND_FOG_NEAR;//-50;
	camera.fog_end = LAND_FOG_FAR;//300;

	vec_set(d3d_fogcolor1,vector(140,170,160));
	vec_set(d3d_fogcolor2,vector(140,120,70)); 

	while(1){
		
		if(camera.z<nWaterLevel) {
			fog_color = 2;
			vec_set(sun_color.blue,vector(d3d_fogcolor2.blue*0.5,d3d_fogcolor2.green*0.5,d3d_fogcolor2.red*0.5));
			vec_set(sky_color.blue,d3d_fogcolor2.blue);	
			vec_set(entHorizon.blue,d3d_fogcolor2.blue);
			camera.fog_start = AQUA_FOG_NEAR;
			camera.fog_end = AQUA_FOG_FAR;
			nIsUnderWater = 1;
		} else {
			
			if(nIsUnderWater == 1) {
				camera.fog_start = LAND_FOG_NEAR;
				camera.fog_end = LAND_FOG_FAR;
				nIsUnderWater = 0;
			}
			
			fog_color = 1;
			vec_set(sun_color.blue,vector(d3d_fogcolor1.blue*0.5,d3d_fogcolor1.green*0.5,d3d_fogcolor1.red*0.5));
			vec_set(sky_color.blue,d3d_fogcolor1.blue);	
			vec_set(entHorizon.blue,d3d_fogcolor1.blue);
			
			if(nWeatherState == 0 && nActiveTriggerId < 0) {
				
				/*if(nDynamicDayNight==0) {
					fade_colors(d3d_fogcolor1,vecCurrentColor,vector(160,170,160)); // Constant fog when there is no day night change
				}*/
				
				if(camera.fog_start>LAND_FOG_NEAR) {
					camera.fog_start -= 10*time_step;
				} else {
					camera.fog_start += 10*time_step;
				}
				if(camera.fog_end>LAND_FOG_FAR){
					camera.fog_end -= 10*time_step;
				} else {
					camera.fog_end += 10*time_step;
				}	
			}	
		}
		
		vec_set(vecCurrentColor,d3d_fogcolor1);
		
		camera.clip_near = 0; 
		camera.clip_far = LAND_FOG_FAR+10;
		
		vec_set(vecTemp,sun_pos); 
		vec_set(entSkySun.x,vecTemp);
		vec_sub(vecTemp,camera.x);
		vec_normalize(vecTemp,LAND_FOG_FAR-1000);
		
		vec_set(entSkySun.x,vecTemp);
		
		// If no moon desired --> disble
		// Set moon to sun position
		vec_set(vecTemp,entSkySun.x); 
		vec_inverse(vecTemp);
		vec_set(entSkyMoon.x,vecTemp);
		
		if(nDynamicDayNight==1) {
			
			if(sun_angle.pan > 230 && sun_angle.pan < 360){sun_angle.pan += 0.01*vNightTransitionTime*time_step;}
			else{sun_angle.pan += 0.01*vDayTransitionTime*time_step;}
			
			sun_angle.pan %= 360;
			sun_angle.tilt = fsin(sun_angle.pan, 50);
			
			if(entSkySun.z<1500){
				if(entSkySun.scale_x<3+1 && entSkySun.scale_y<4) {
					entSkySun.scale_x += 0.01*time_step*vDayTransitionTime/10;
					entSkySun.scale_y += 0.01*time_step*vDayTransitionTime/10;
				}	
			} else {
				if(entSkySun.scale_x>3 && entSkySun.scale_y>3){
					entSkySun.scale_x -= 0.01*time_step*vDayTransitionTime/10;
					entSkySun.scale_y -= 0.01*time_step*vDayTransitionTime/10;
				}
			}
		} else {
			sun_angle.pan = vSunAzimuth;
			sun_angle.tilt = vSunElevation;
		}
		
		if(sun_angle.pan > 0 && sun_angle.pan < 40){
			fade_in_sky_day();
		}	
		if(sun_angle.pan > 10 && sun_angle.pan < 160){
			fade_colors(d3d_fogcolor1,vecCurrentColor,vector(140,170,160)); // Day
		}
		if(sun_angle.pan > 40 && sun_angle.pan < 160){	
			set_sky_day();
		}	
		if(sun_angle.pan > 160 && sun_angle.pan < 190){
			fade_in_sky_night();
			fade_colors(d3d_fogcolor1,vecCurrentColor,vector(72,135,240)); // Evening
		}
		if(sun_angle.pan > 190 && sun_angle.pan < 340){
			fade_colors(d3d_fogcolor1,vecCurrentColor,vector(30,20,20)); // Night
		}
		if(sun_angle.pan > 190 && sun_angle.pan < 340){	
			set_sky_night();
		}
		if(sun_angle.pan > 340 && sun_angle.pan < 360){
			fade_colors(d3d_fogcolor1,vecCurrentColor,vector(111,190,250)); // Morning
		}
		
		if(sun_angle.pan > 190 && sun_angle.pan < 350){
			if(nWeatherState>0) {
				fade_colors(d3d_fogcolor1,vecCurrentColor,vector(15,15,15)); // Fog weather night
			}
		} else {
			if(nWeatherState>0) {
				fade_colors(d3d_fogcolor1,vecCurrentColor,vector(50,50,50)); // Fog weather day
			}
		}
		
		vec_set(entSkySunshine.x,entSkySun.x);	
		vec_set(entSkySunCorona.x,entSkySun.x);
		
		entSkySunCorona.scale_x = 15;
		entSkySunCorona.scale_y = 12;
		
		entSkySunshine.scale_x = 30;
		entSkySunshine.scale_y = 12;
		
		entSkyMoon.scale_x = 4.5;
		entSkyMoon.scale_y = 4.5;	 
		
		entSkyNight.scale_x = 0.5;
		entSkyNight.scale_y = 0.5;
		
		wait(1);
	}	
}

void act_mystymood_trigg_label1();

//title:MystyMood Sky Template
//image:loopix_logo.pcx
//action:act_mystymood_trigg  
//skill1:TRIGGER_RANGE 300.000
//help: sets  range within the mood-trigger is activated
//skill2:TRIGGER_RAIN_WIND_X 2.000
//help: wind strenght on direction x
//skill3:TRIGGER_RAIN_WIND_Y 0.000
//help:  wind strenght on direction y
//skill4:TRIGGER_RAIN_FALLSPEED 20.000
//help: fallingspeed of the raindrops
// section:
//skill5:TRIGGER_SNOW_WIND_X 4.000
//help: wind strenght on direction x
//skill6:TRIGGER_SNOW_WIND_Y 0.000
//help: wind strenght on direction y
//skill7:TRIGGER_SNOW_FALL_SPEED 5.000
//help: fallingspeed of the snowflakes
// section:
//skill8:T_RAIN_RANDOM_MOVE 2.000
//help: random-xy movement of the rain particle
//skill9:T_SNOW_RANDOM_MOVE 4.000
//help: random-xy movement of the snow particle
// section:
//section:
//skill11:T_FOG_NEAR 0.000
//help: sets the fog_near distance...use negative values for starting the fog behind the camera
//skill12:T_FOG_FAR 0.000
//help: sets the fog_far distance
//skill13: T_FOG_RED 140.000
//help: fog color red
//skill14: T_FOG_GREEN 170.000
//help: fog color green
//skill15: T_FOG_BLUE 160.000
//help: fog color blue
//section:
//text: flag to enable rain
//flag1:TRIGGER_RAIN 0.000
//text: flag to enable snow
//flag2:TRIGGER_SNOW 0.000
//text: flag to disable lightning and thunder
//flag3:TRIGGER_DISABLE_THUNDER
// section:
// section:
//text: flag to enable fog distance/density change
//flag6:T_FOG_CHANGE 1.000
//uses:TRIGGER_RANGE,trigg_rain,trigg_snow,TRIGGER_RAIN_WIND_X,TRIGGER_RAIN_WIND_Y,TRIGGER_RAIN_FALLSPEED
//uses:TRIGGER_SNOW_WIND_X,TRIGGER_SNOW_WIND_Y,TRIGGER_SNOW_FALL_SPEED
//uses:T_RAIN_RANDOM_MOVE,T_SNOW_RANDOM_MOVE
//uses:TRIGGER_DISABLE_THUNDER
//uses:T_FOG_CHANGE,T_FOG_NEAR,T_FOG_FAR,T_FOG_RED,T_FOG_GREEN,T_FOG_BLUE
action act_mystymood_trigg()
{
	//ent_mystymood_trigg = me;
	
	//	my.invisible = on;
	set(my,PASSABLE);
	
	//set default skills
	if(!my.TRIGGER_RANGE){my.TRIGGER_RANGE = 300;}
	if(!my.TRIGGER_RAIN_FALLSPEED){my.TRIGGER_RAIN_FALLSPEED = 20;}
	if(!my.TRIGGER_SNOW_FALL_SPEED){my.TRIGGER_SNOW_FALL_SPEED = 5;}
	
	

	my.TRIGGER_ID = nTriggerCount;
	nTriggerCount += 1;

	my.skill70 = nWeatherState;	
	my.skill72 = vRainRandomMove;
	my.skill73 = vecRainDirection.x;
	my.skill74 = vecRainDirection.y;
	my.skill75 = vecRainDirection.z;
	my.skill76 = vDisableThunder;
	my.skill78 = vSnowRandomMove;
	my.skill79 = vecSnowDirection.x;
	my.skill80 = vecSnowDirection.y;
	my.skill81 = vecSnowDirection.z;
	my.skill82 = 10;	

	while(1)
	{
		if(vec_dist(my.x,camera.x) < my.TRIGGER_RANGE)
		{
			nActiveTriggerId = my.TRIGGER_ID;
			
			if(is(my,TRIGGER_RAIN))
			{
				nWeatherState = 1;	
			}
			if(is(my,TRIGGER_SNOW))
			{
				nWeatherState = 2;	
			}
			if(!is(my,TRIGGER_SNOW) && !is(my,TRIGGER_RAIN))
			{
				nWeatherState = 0;	
			}
			
			if(nWeatherState > 0)
			{
				
				vRainRandomMove = my.T_RAIN_RANDOM_MOVE;
				
				vecRainDirection.x = my.TRIGGER_RAIN_WIND_X;
				vecRainDirection.y = my.TRIGGER_RAIN_WIND_Y;
				
				vecRainDirection.z = my.TRIGGER_RAIN_FALLSPEED;
				
				vSnowRandomMove = my.T_SNOW_RANDOM_MOVE;
				
				vecSnowDirection.x = my.TRIGGER_SNOW_WIND_X;
				vecSnowDirection.y = my.TRIGGER_SNOW_WIND_Y;
				
				vecSnowDirection.z = my.TRIGGER_SNOW_FALL_SPEED;		
				
				vDisableThunder = is(my,TRIGGER_DISABLE_THUNDER);
			}
			
			if(is(my,T_FOG_CHANGE))
			{
				fade_colors(d3d_fogcolor1,vecCurrentColor,vecTempColor);
				
				vFogDistanceNearTemp = my.T_FOG_NEAR;
				vFogDistanceFarTemp= my.T_FOG_FAR;
				
				if(camera.fog_start>vFogDistanceNearTemp){
					camera.fog_start -= 10*time_step;
				}
				else{
					camera.fog_start += 10*time_step;
				}
				if(camera.fog_end>vFogDistanceFarTemp){
					camera.fog_end -= 10*time_step;
				}
				else{
					camera.fog_end += 10*time_step;
				}
				
				vecTempColor.z = my.T_FOG_RED;
				vecTempColor.y = my.T_FOG_GREEN;
				vecTempColor.x = my.T_FOG_BLUE;
				
				
			}
		}else
		{
			if(nActiveTriggerId == my.TRIGGER_ID)
			{
				nActiveTriggerId = -1;
				
				act_mystymood_trigg_label1();	
				break;
			}
		}
		
		wait(1);	
	}
}

void act_mystymood_trigg_label1()
{
	vWeatherFader = 1;//reset this var

	nWeatherState = my.skill70;
	vRainRandomMove = my.skill72;
	vecRainDirection.x = my.skill73;
	vecRainDirection.y = my.skill74;
	vecRainDirection.z = my.skill75;
	vDisableThunder = my.skill76;
	vSnowRandomMove = my.skill78;
	vecSnowDirection.x = my.skill79;
	vecSnowDirection.y = my.skill80;
	vecSnowDirection.z = my.skill81;
	//weather_fade_speed = my.skill82;
	
	act_mystymood_trigg();
}

void weather_sun() {
	nWeatherState = 0;
}

void weather_rain() {
	vDisableThunder = 1;
	nWeatherState = 1;
}

void weather_thunder() {
	vDisableThunder = 0;
	nWeatherState = 1;
}

void weather_snow() {
	nWeatherState = 2;
}

void weather_daynight_dynamic() {
	nDynamicDayNight = 1;
}

void weather_daynight_static() {
	nDynamicDayNight = 0;
}

void weather_night() {
	nDynamicDayNight = 0;
	vSunAzimuth = 191;
	vec_set(d3d_fogcolor1, vector(30,20,20));
	set_sky_night();
}

void weather_day() {
	nDynamicDayNight = 0;
	vSunAzimuth = 100;
	vec_set(d3d_fogcolor1, vector(140,170,160));
	set_sky_day();
}

void weather_evening() {
	nDynamicDayNight = 0;
	vSunAzimuth = 161;
	vec_set(d3d_fogcolor1, vector(72,135,240));
	set_sky_night();
}

void weather_morning() {
	nDynamicDayNight = 0;
	vSunAzimuth = 341;
	vec_set(d3d_fogcolor1, vector(111,190,250));
	set_sky_day();	
}

#endif