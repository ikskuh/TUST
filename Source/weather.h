#ifndef _WEATHER_H_
#define _WEATHER_H_

/**
 * Sets the daytime to night
 */
void weather_night();

/**
 * Sets the daytime to day
 */
void weather_day();

/**
 * Sets the daytime to evening
 */
void weather_evening();

/**
 * Sets the daytime to morning
 */
void weather_morning();

/**
 * Change time of day dynamically
 */
void weather_daynight_dynamic();

/**
 * Sets a static time of day
 */
void weather_daynight_static();

/**
 * Let the sun shine
 */
void weather_sun();

/**
 * Sets it rain
 */
void weather_rain();

/**
 * Start rain with thunder
 */
void weather_thunder();

/**
 * Let it snow
 */
void weather_snow();


// ----------------------------------------------------------------------
// TWEAK HERE!
// ----------------------------------------------------------------------

int nDynamicDayNight						=	1;
var vSunAzimuth							=	190;	// Fixed position when no dynamic day night
var vSunElevation							=	20;	// Fixed position when no dynamic day night
var vDayTransitionTime					=	30;
var vNightTransitionTime				=	50;
int nAmbientSounds						=	1;
int nUnderwaterSounds					=	1;

int nRandomWeather						=	0;
int nRandomWeatherChangeFrequency	=	100;	// Higher value -> Longer states
int nRandomWeatherStateDuration		=	40;	// Higher values -> Lower change probability

int nWeatherState							=	0;		// 0 sun, 1 rain, 2 snow

int nWaterLevel							=	-10;		// z position of water


// Use to tweak frame rates
#define LAND_FOG_NEAR		200
#define LAND_FOG_FAR			4000

#define AQUA_FOG_NEAR		-300
#define AQUA_FOG_FAR			600

#define WEATHER_FOG_NEAR	100
#define WEATHER_FOG_FAR		1000

// Weather particle box around the camera
#define WEATHER_BOX_X		1000
#define WEATHER_BOX_Y		1000
#define WEATHER_BOX_Z		600

// Effect density
#define RAIN_DENSITY 10
#define SNOW_DENSITY 20




// ----------------------------------------------------------------------
// LENSFLARE
// ----------------------------------------------------------------------

#define PIVOT_DIST skill99

VECTOR vecTemp1;
VECTOR vecTemp2;
VECTOR ctemp;

int nLensFlare = -1;	// -1 not created, 0 off, 1 on
int nLensFlareFadeSpeed = 25;
int nLensFlareAlpha = 70;
var vCameraPan = 0;

ENTITY* entFlare01 = NULL;
ENTITY* entFlare02 = NULL;
ENTITY* entFlare04 = NULL;
ENTITY* entFlare05 = NULL;
ENTITY* entFlare06 = NULL;
ENTITY* entFlare07 = NULL;
ENTITY* entFlare08 = NULL;
ENTITY* entFlare09 = NULL;
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



// ----------------------------------------------------------------------
// WEATHER
// ----------------------------------------------------------------------

BMAP* bmapWeatherParticle	= NULL;
BMAP* bmapRain					= NULL;
BMAP* bmapSnow					= NULL;
BMAP* bmapLightning			= NULL;
var vParticleVelocityX		=	0;
var vParticleVelocityY		=	0;
var vParticleVelocityZ		=	0;
var vParticleSize				=	0;
var vParticleAlpha			=	0;
var vParticleNumber			=	0;
VECTOR* vecRainDirection	= { x = 2; y = 1; z = 25; }
VECTOR* vecSnowDirection	= { x = 4; y = 1; z = 25; }
var vRainRandomMove			= 1;
var vSnowRandomMove			= 8;
var vDisableThunder			= 0;
int nLightning					= 1;


// Skills and flags for triggers
#define TRIGGER_RAIN					FLAG1
#define TRIGGER_SNOW					FLAG2
#define TRIGGER_DISABLE_THUNDER	FLAG3
#define T_FOG_CHANGE					FLAG4

#define TRIGGER_RANGE				skill1
#define TRIGGER_RAIN_WIND_X		skill2
#define TRIGGER_RAIN_WIND_Y		skill3
#define TRIGGER_RAIN_FALLSPEED	skill4
#define TRIGGER_SNOW_WIND_X		skill5
#define TRIGGER_SNOW_WIND_Y		skill6
#define TRIGGER_SNOW_FALL_SPEED	skill7
#define T_RAIN_RANDOM_MOVE			skill8
#define T_SNOW_RANDOM_MOVE			skill9
#define T_FOG_NEAR					skill10
#define T_FOG_FAR						skill11
#define T_FOG_RED						skill12
#define T_FOG_GREEN					skill13
#define T_FOG_BLUE					skill14
#define TRIGGER_ID					skill100

int nTriggerCount		= 0;
var nActiveTriggerId	= -1;
var vWeatherFader		= 1;

VECTOR vecTempColor;
var vFogDistanceNearTemp;
var vFogDistanceFarTemp;

var vLightningTemp;
var vLightningSegmentLength;
VECTOR vecLightningSegmentStart;
VECTOR vecLightningSegmentEnd;
var vLightningStrokeLength;
VECTOR vecLightningStrokeStart;
VECTOR vecLightningStrokeEnd;

VECTOR vecParticleSeedBox;
VECTOR vecCurrentColor;

int nIsUnderWater;
int nLightningIsOn;

SOUND* sndRain				= NULL;
SOUND* sndWind				= NULL;
SOUND* sndDay				= NULL;
SOUND* sndNight			= NULL;
SOUND* sndThunder1		= NULL;
SOUND* sndThunder2		= NULL;
SOUND* sndThunder3		= NULL;
SOUND* sndThunder4		= NULL;
SOUND* sndThunder5		= NULL;
SOUND* sndUnderwater		= NULL;

ENTITY* entHorizon		= NULL;
ENTITY* entCloud1 		= NULL;
ENTITY* entCloud2			= NULL;
ENTITY* entCloud3			= NULL;
ENTITY* entSkyDay			= NULL;
ENTITY* entSkySun			= NULL;
ENTITY* entSkySunCorona	= NULL;
ENTITY* entSkySunshine	= NULL;
ENTITY* entSkyNight		= NULL;
ENTITY* entSkyMoon		= NULL;


#include "weather.c"

#endif