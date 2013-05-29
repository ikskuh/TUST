#ifndef _EFFECTS_H_
#define _EFFECTS_H_

/**
 * \file effects.h
 * \Some simple particle effects like smoke, flames or explosions.
 *
 */
 
 
 #include "list.h" // Needed to manage fog entities
 
#define FOG_MAX_ALPHA skill1
#define FOG_CAMERA_DISTANCE 300
#define FOG_SPEED 10

List* fogEntities = NULL;

VECTOR vecEffectsTemp;

BMAP* bmapStar = "..\\Ressources\\Graphics\\star.bmp";
BMAP* bmapFlame = "..\\Ressources\\Graphics\\flame.bmp";
BMAP* bmapSmoke = "..\\Ressources\\Graphics\\smoke.tga";

BMAP* bmapExplParticle = "..\\Ressources\\Graphics\\explParticle.tga";
BMAP* bmapExplSharpnel = "..\\Ressources\\Graphics\\explShrapnel.tga";

/**
 * Starts a complex explosion effect
 * \param	VECTOR*	Position of the effect
 */
void eff_explosion(VECTOR* _pos);

/**
 * Starts a complex smoke effect on a certain area
 * \param	STRING*	File to create smoke entities
 * \param	VECTOR*	Position to place the smoke
 * \param	VECTOR*	Size of the smoke
 * \param	var		Smoke density
 * \param	var		Effect's time to live
 */
void eff_complexSmoke(STRING *smoke, VECTOR* _pos, VECTOR* _size, var density, var time);

/**
 * Generate fog or dust that is always placed around the camera
 * \param	int	Fog density
 */
void eff_generate_fog(int _density);

void pFountain(PARTICLE *p);
void pDenseSmoke(PARTICLE *p);
void pStars(PARTICLE *p);
void pFlame(PARTICLE *p);
void pSmokeFlames(PARTICLE *p);

// Rain and snow effects can be found in weather.h/.c
void pElectro(PARTICLE *p);
void pLeaves(PARTICLE *p);
void pFlies(PARTICLE *p);
void pBubbles(PARTICLE *p);
void pWaterfall(PARTICLE *p);
void pExplosionFire(PARTICLE *p);
void pExplosionDust(PARTICLE *p);
void pExplosionScatter(PARTICLE *p);

// Internal

void pAlphaFade(PARTICLE *p);
void pAlphaFadeFlame(PARTICLE *p);
void vec_randomize(VECTOR* _vector, var _range);

#include "effects.c"

#endif