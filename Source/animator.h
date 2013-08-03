#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

/**
 * \defgroup  AnimatorSampleModule Animator Example
 * \ingroup   ExampleModule
 * A small example for the animator system.
 *
 * \{
 * \include animation.c
 * \}
 */
 
 /**
 * \defgroup  AnimatorModule Animation System
 * \ingroup   GenericScriptLibrary
 * \brief     Provides functions to create an easy-to-control animation system.
 *
 * Simple animation system for vertex animations.
 * \{
 *
 * \file animator.h
 * \brief Header of the animation module.
 *
 * \file animator.c
 * \brief Code of the animation module.
 */


typedef struct Animation
{
	char scene[100];
	var speed;
	int isLooped;
} Animation;

/**
 * \struct Animator
 * \brief Entity animation controller
 */
typedef struct Animator
{
	ENTITY *target;
	int current;
	var progress;
	
	int previous;
	var blend;
	var blendSpeed;
	
	Animation *animations;
	int size;
} Animator;

/**
 * \brief Attaches an animation controller to an entity.
 * \param  ent        The entity for the controller.
 * \param  animations Maximum number of animations for the controller.
 * \return            Animation controller for the entity.
 */
Animator *animator_attach(ENTITY *ent, int animations);

/**
 * \brief Updates the animation controller and sets the entity frames.
 * \param animator The animation controller to update.
 */
void animator_update(Animator *animator);

/**
 * \brief Sets up a single animation.
 * \param  animator  The animation controller containing the animation.
 * \param  animation ID of the animation.
 * \param  scene     Name of the scene.
 * \param  speed     Speed of the animation.
 * \param  isLooped  If set, the animation will be played looped.
 */
void animator_set(Animator *animator, int animation, STRING *scene, var speed, bool isLooped);

/**
 * \brief Starts to play an animation.
 * \param animator   The animation controller that should play the animation.
 * \param animation  The animation to be played.
 * \param forceReset Forces the animation be played from the start.
 */
void animator_play(Animator *animator, int animation, bool forceReset);

/**
 * \brief Starts to blend from one animation to another.
 *
 * This function starts the blending of two animations.
 * The first animation is the current animation of the controller,
 * the second animation is the animation defined in the parameters.
 * \param animator   The animation controller that should blend the animation.
 * \param animation  The animation that should be blended to.
 * \param ticks      Duration of the blending in ticks (1/16 sec).
 */
void animator_blend(Animator *animator, int animation, var ticks);

/**
 * Stops the animation controller from playing any animation.
 * \param animator The animation controller that should be stopped.
 */
void animator_stop(Animator *animator);

#include "animator.c"
#endif















