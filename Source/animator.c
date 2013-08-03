#include <acknex.h>
#include <stdio.h>
#include "animator.h"

Animator *animator_attach(ENTITY *ent, int animations)
{
	if(ent == NULL) return NULL;
	if(animations <= 0) return NULL;
	Animator *anim = sys_nxalloc(sizeof(Animator));
	
	anim->target = ent;
	anim->current = -1;
	anim->progress = 0;
	anim->animations = sys_nxalloc(animations * sizeof(Animation));
	anim->size = animations;
	return anim;
}

void animator_update(Animator *animator)
{
	if(animator == NULL) return;
	if(animator->current < 0 || animator->current >= animator->size) return;
	
	int mode = 0;
	if((animator->animations)[animator->current].isLooped)
		mode = ANM_CYCLE;
	
	ent_animate(animator->target, (animator->animations)[animator->current].scene, animator->progress, mode);
	if(animator->blend > 0 && animator->previous > 0)
	{
		ent_blendframe(animator->target, animator->target, (animator->animations)[animator->previous].scene, animator->progress, animator->blend); 
		animator->blend -= animator->blendSpeed * time_step;
	}
	
	animator->progress += (animator->animations)[animator->current].speed * time_step;
}

void animator_set(Animator *animator, int animation, STRING *scene, var speed, bool isLooped)
{
	if(animator == NULL) return;
	if(scene == NULL) return;
	if(animation < 0 || animation >= animator->size) return;
	
	strcpy((animator->animations)[animation].scene, _chr(scene));
	(animator->animations)[animation].speed = speed;
	(animator->animations)[animation].isLooped = isLooped;
}

void animator_play(Animator *animator, int animation, bool forceReset)
{
	if(animator == NULL) return;
	if(animation < 0 || animation >= animator->size) return;

	if(animator->current != animation || forceReset)
		animator->progress = 0;

	animator->current = animation;
}

void animator_blend(Animator *animator, int animation, var ticks)
{
	if(animator == NULL) return;
	if(animation < 0 || animation >= animator->size) return;
	if(animation == animator->current) return;
	
	animator->previous = animator->current;
	animator->blend = 100;
	animator->blendSpeed = 100 / ticks;
	animator->current = animation;
}

void animator_stop(Animator *animator)
{
	if(animator == NULL) return;
	animator->current = -1; // Invalid id forces animator to stop
}





























