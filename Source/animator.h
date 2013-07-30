#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

/**
 * \file animator.h
 * Simple animation system.
 */

typedef struct Animation
{
	char scene[100];
	var speed;
	int isLooped;
} Animation;

typedef struct Animator
{
	ENTITY *target;
	int current;
	var progress;
	Animation *animations;
	int size;
} Animator;

Animator *animator_attach(ENTITY *ent, int animations);

void animator_update(Animator *animator);

void animator_set(Animator *animator, int animation, STRING *scene, var speed, bool isLooped);

void animator_play(Animator *animator, int animation, bool forceReset);

void animator_stop(Animator *animator);

#include "animator.c"
#endif