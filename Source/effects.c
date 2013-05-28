#ifndef _EFFECTS_C_
#define _EFFECTS_C_

// Explosion

void p_eff_expl_shrapnel_fade(PARTICLE *p) {
    p.alpha += 6 *time_step;
    if(p.alpha > 30) p.lifespan = 0;
}

void p_eff_expl_shrapnel(PARTICLE *p) {
	p.bmap = bmapExplSharpnel;
	p.vel_x = random(32)-16;
	p.vel_y = random(32)-16;
	p.vel_z = random(32)-16;
	p.size = 5;
	p.alpha = 0;
	p.flags |=  MOVE | TRANSLUCENT;
	p.event = p_eff_expl_shrapnel_fade;
}

void p_eff_expl_particle_fade(PARTICLE *p) {
   p.alpha -= random(4) * time_step;
   if(p.alpha < 0) p.lifespan = 0;
   p.vel_x += random(0.6)-0.3;
	p.vel_y += random(0.6)-0.3;
}

void p_eff_expl_particle(PARTICLE *p) {
	p.bmap = bmapExplParticle;
	p.angle = random(360);
	
	vec_for_angle(vecEffectsTemp, vector(p.angle, 0, 0 ));
	vec_rotate(vecEffectsTemp, vector(camera.pan, camera.tilt-90, 0));
	vec_normalize(vecEffectsTemp, 10+random(50));
	vec_add(p.x, vecEffectsTemp);
	if (random(2) > 1) {
		vec_set(vecEffectsTemp, vector( 1, 0, 0 ));
		vec_rotate(vecEffectsTemp, vector(camera.pan, camera.tilt+90, 0));
		vec_normalize(vecEffectsTemp, 10+random(5));
		vec_set(p.vel_x, vecEffectsTemp);
	}
	p.vel_z *= random(1);
	p.size = 5 ;
	p.alpha = 50 ;
	p.flags |= TRANSLUCENT | MOVE;
	p.event = p_eff_expl_particle_fade;
}

void eff_expl_smoke2 () {
	set(me, TRANSLUCENT | ZNEAR | PASSABLE | LIGHT);
	my.roll = random(360);
	my.alpha = 100;
	my.scale_x = 0.2;
	my.scale_y = 0.7;
	my.skill1 = random(3)+2;
	vec_fill(my.blue, 30);
	while(my.alpha > 0) {
		if(my.alpha > 93 ) {
			vec_for_angle(vecEffectsTemp, vector( my.roll,0,0));
			vec_rotate(vecEffectsTemp, vector(camera.pan, camera.tilt+90, 0));
			vec_normalize(vecEffectsTemp, time_step * (my.alpha/my.skill1) * 0.4);
			vec_add(my.x, vecEffectsTemp);
			my.scale_y += vec_length(vecEffectsTemp)/46;
			my.alpha -= time_step * 3;
		} else {
			vec_for_angle(vecEffectsTemp, vector(my.roll, 0, 0));
			vec_rotate(vecEffectsTemp, vector(camera.pan, camera.tilt+90, 0));
			vec_normalize(vecEffectsTemp, time_step * (my.alpha/(my.skill1*2)));
			vec_add(my.x, vecEffectsTemp);
			
			my.scale_y += vec_length(vecEffectsTemp) / 5000;
			
			my.alpha -= time_step * 9;
		}
		wait(1);
	}
	ent_remove(me);
	return;
}

void eff_expl_wave() {
	set(me, TRANSLUCENT | DECAL | PASSABLE);
	my.alpha = 70;
	vec_fill(my.scale_x, 0.1);
	my.tilt = 90;
	my.pan = random(360);
	vec_fill(my.blue, 0);
	while(my.alpha > 0) {
		vec_fill(vecEffectsTemp, time_step * 2);
		vec_add(my.scale_x, vecEffectsTemp);
		my.alpha -= time_step * 7;
		wait (1);
	}
	ent_remove(me);
}

void eff_expl_smoke() {
	set(me, TRANSLUCENT | LIGHT | ZNEAR | PASSABLE);
	vec_set(my.blue, vector(200,255,255));
	my.alpha = 40;
	my.roll = random(360);
	vec_fill(my.scale_x, 0.4);
	while(my.alpha > 0) {
		my.alpha -= time_step * 2;
		my.roll += time_step * sign(ang(my.roll))*1;
		vec_fill(vecEffectsTemp, time_step *0.01);
		vec_add(my.scale_x, vecEffectsTemp);
		wait(1);
	} 
	ent_remove ( me );
}

void eff_expl_flash_2() {
	set(me, BRIGHT | TRANSLUCENT | ZNEAR | PASSABLE);
	my.blue = 150;
	my.green = 235;
	my.red = 255;
	my.alpha = 0;
	my.roll = random(360);
	vec_fill(my.scale_x, 3);
	vec_for_angle(vecEffectsTemp, vector( my.roll, 0, 0));
	vec_rotate(vecEffectsTemp, vector(camera.pan, camera.tilt+90, 0));
	vec_normalize(vecEffectsTemp, 40);
	vec_add(vecEffectsTemp, my.x);
	vec_set(my.x, vecEffectsTemp);
	
	while(my.alpha < 100) {
		if (my.lightrange < 1000 ) my.lightrange += 2000 * time_step;
		my.alpha += time_step * (random(20)+20);
		my.roll += time_step * sign(ang(my.roll));
		vec_normalize ( my.blue, 150 + random(105) );
		wait(1);
	}
	ent_create("..\\Ressources\\Graphics\\explSmoke02.tga", my.x, eff_expl_smoke);
	while(my.alpha > 0) {
		if (my.lightrange > 0) my.lightrange -= 500 * time_step;
		my.alpha -= time_step * 20;
		my.roll += time_step * sign(ang(my.roll))*5;
		wait(1);
	}
	while (my.lightrange > 0) {
		my.lightrange -= 500 * time_step;
		wait(1);
	}
	ent_remove(me);	
}

void eff_expl_flash() {
	int i;
	set(me, BRIGHT | TRANSLUCENT | LIGHT | ZNEAR | PASSABLE);
	my.alpha = 50;
	my.roll = random(360);
	while(my.alpha < 100) {
		my.alpha += time_step * (random(20)+20);
		my.roll += time_step * sign(ang(my.roll));
		vec_fill ( my.scale_x, my.alpha/13 );
		wait(1);		
	}
	for(i=0; i<4; i++) {
		ent_create("..\\Ressources\\Graphics\\explFlash01.tga", my.x, eff_expl_flash_2);
		you = ent_create("..\\Ressources\\Graphics\\explFlash02.tga", my.x, eff_expl_flash_2);		
		if (i == 0) set(you, LIGHT);
	}
	for(i=0; i<5; i++) {
		ent_create ("explSmoke01.tga", my.x, eff_expl_smoke2);
	}
	effect(p_eff_expl_particle, 200, my.x, NULL);
	vec_add(my.scale_x, vector(3,3,3));
	ent_create("..\\Ressources\\Graphics\\explSmoke02.tga", my.x, eff_expl_smoke);
	while(my.alpha > 0) {
		my.alpha -= time_step * 25;
		my.roll += time_step * sign(ang(my.roll));
		wait(1);
	} 
	ent_remove(me);
}

void eff_explosion(VECTOR* _pos) {
	ent_create("..\\Ressources\\Graphics\\explFlash01.tga", _pos, eff_expl_flash);
	ent_create ("..\\Ressources\\Graphics\\explFlash02.tga", _pos, eff_expl_flash);
	ent_create ("..\\Ressources\\Graphics\\explWave01.tga", _pos, eff_expl_wave);
	effect(p_eff_expl_shrapnel, 200, _pos, NULL);
}



// General functions

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





// Further effects

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

void eff_complexSmokeSprite()
{
	my->flags = TRANSLUCENT;
	my->skill1 = 50;
	wait(1);
	while(my->alpha < my->skill1)
	{
		vec_diff(&my->pan, &my->x, &camera->x);
		vec_to_angle(&my->pan, &my->pan);
		my->roll = total_ticks;
		my->alpha += 0.2 * time_step;
		wait(1);
	}
	while(my->skill2 >= total_ticks || my->skill2 < 0)
	{
		vec_diff(&my->pan, &my->x, &camera->x);
		vec_to_angle(&my->pan, &my->pan);
		my->roll = total_ticks;
		wait(1);
	}
	while(my->alpha > 0)
	{
		vec_diff(&my->pan, &my->x, &camera->x);
		vec_to_angle(&my->pan, &my->pan);
		my->roll = total_ticks;
		my->alpha -= time_step;
		wait(1);
	}
}

void eff_complexSmoke(STRING *smoke, VECTOR* pos, VECTOR* size, var density, var time)
{
	int i;
	var volume = abs(size.x * size.y * size.z);
	for(i = 0; i < 3 * density * (volume / 200000); i++)
	{
		VECTOR spawn;
		spawn.x = random(size.x) - 0.5 * size.x;
		spawn.y = random(size.y) - 0.5 * size.y;
		spawn.z = random(size.z) - 0.5 * size.z;
		vec_add(&spawn, pos);
		
		you = ent_create(smoke, &spawn, eff_complexSmokeSprite);
		you->skill1 = density;
		if(time >= 0)
			you->skill2 = total_ticks + time;
		else
			you->skill2 = -1;
	}
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