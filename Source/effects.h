#ifndef _EFFECTS_H_
#define _EFFECTS_H_

/**
 * \file effects.h
 * \Some simple particle effects like smoke, flames or explosions.
 *
 */

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

void eff_complexSmoke(STRING *smoke, VECTOR* _pos, VECTOR* _size, var density, var time);

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