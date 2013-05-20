#ifndef _EFFECTS_C_
#define _EFFECTS_C_

void vec_randomize (VECTOR* _vector, var _range) {
   vec_set(_vector,vector(random(1)-0.5,random(1)-0.5,random(1)-0.5));
   vec_normalize(_vector,random(_range));
}

void pAlphaFade(PARTICLE *p) {
	p.alpha -= p.skill_a*time_step;
	if (p.alpha <= random(10)) {
		p.lifespan = 0;
	}
}

void pAlphaFadeFlame(PARTICLE *p) {
	VECTOR vecTemp;
	vec_lerp(vecTemp, vector(128, 50, 50), vector(200, 180, 100), p.lifespan / 10);
	p.blue = vecTemp.z;
	p.green = vecTemp.y;
	p.blue = vecTemp.x;
	p.alpha = p.lifespan / 10 * 100 * 0.3;
	if (p.alpha <= 0) p.lifespan = 0;
}

void pFountain(PARTICLE *p) {
	VECTOR vecTemp;
	vec_randomize(vecTemp, 2);
	vec_add(p.vel_x, vecTemp);
	vec_set(p.blue, vector(random(255), random(255), 255));
	set(p, MOVE | BRIGHT | TRANSLUCENT);
	p.alpha = 100;
	p.size = 2;
	p.gravity = 0.2;
	p.skill_a = 3;
	p.event = pAlphaFade;
}

void pDenseSmoke(PARTICLE *p) {
	VECTOR vecTemp;
	vec_randomize(vecTemp, 2);
	vec_add(p.vel_x, vector(-1+random(2), -1+random(2), 1));
	vec_set(p.blue, vector(140, 140, 140));
	set(p, MOVE | TRANSLUCENT);
	p.alpha = 60 + random(20);
	p.size = 20;
	p.gravity = 0;
	p.skill_a = 1;
	p.event = pAlphaFade;
}

void pStars(PARTICLE *p) {
	VECTOR vecTemp;
	vec_randomize(vecTemp, -1);
	vec_add(p.vel_x, vecTemp);
	vec_set(p.blue, vector(random(255), random(255), 255));
	set(p, MOVE | BRIGHT | TRANSLUCENT);
	p.alpha = 100;
	p.size = 3;
	p.gravity = 1;
	p.skill_a = 3;
	p.bmap = bmapStar;
	p.event = pAlphaFade;
}

void pFlame(PARTICLE *p) {
	VECTOR vecTemp;
	vec_randomize(vecTemp, 2);
	vec_add (p.vel_x, vecTemp);
	p.bmap = bmapFlame;
	set(p, BRIGHT | MOVE | TRANSLUCENT);
	p.lifespan = 10;
	p.size = 10;
	p.event = pAlphaFadeFlame;
}

void pSmokeFlames(PARTICLE *p) {
	VECTOR vecTemp;
	vec_randomize(vecTemp, 2);
	vec_add (p.vel_x, vecTemp);
	p.bmap = bmapSmoke;
	set(p, MOVE | TRANSLUCENT);
	p.lifespan = 10;
	p.size = 10;
	p.event = pAlphaFade;
}

void pElectro(PARTICLE *p);
void pLeaves(PARTICLE *p);
void pFlies(PARTICLE *p);
void pBubbles(PARTICLE *p);
void pWaterfall(PARTICLE *p);
void pExplosionFire(PARTICLE *p);
void pExplosionDust(PARTICLE *p);
void pExplosionScatter(PARTICLE *p);

#endif