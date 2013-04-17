#ifndef _NETWORK_H_
#define _NETWORK_H_

/**
 * \file network.h
 * \brief Network synchronisation and helpers.
 *
 * Provides functions for synchronizing entities in multiplayer games.
 */

/**
 * Readonly: This skill is used for synchronizing the x position.
 */
#define NET_SKILL_POS_X		skill60

/**
 * Readonly: This skill is used for synchronizing the y position.
 */
#define NET_SKILL_POS_Y		skill61

/**
 * Readonly: This skill is used for synchronizing the z position.
 */
#define NET_SKILL_POS_Z		skill62

/**
 * Readonly: This skill is used for synchronizing the pan rotation.
 */
#define NET_SKILL_ANG_PAN	skill63

/**
 * Readonly: This skill is used for synchronizing the tilt rotation.
 */
#define NET_SKILL_ANG_TILT	skill64

/**
 * Readonly: This skill is used for synchronizing the roll rotation.
 */
#define NET_SKILL_ANG_ROLL	skill65

/**
 * This skill sets the speed which is used for this entity in linear interpolation.
 */
#define NET_SKILL_LERPSPEED	skill66

/**
 * Set this flag in net_ent_flags to enable unreliable sending of position and angle updates.
 */
#define NET_ENT_UNRELIABLE	(1 << 0)



/**
 * The maximum update rate for sending position and angle updates in ticks.
 * Use a smaller value to send changes more frequent, 0 sends every change.
 */
var net_ent_sendrate = 1;

/**
 * The factor used in interpolating entity positions and angles with vec_lerp.
 * If the factor is 0, linear position interpolation will be used instead of the usage if vec_lerp.
 */
var net_ent_lerpfactor = 0.5;

/**
 * The minimal movement distance for sending a position update
 */
var net_ent_mindist = 1;

/**
 * The minimal angular difference for sending a angle update
 */
var net_ent_mindiff = 1;

/**
 * The default lerp speed stored in NET_SKILL_LERPSPEED
 */
var net_ent_default_lerpspeed = 15;

/**
 * Flags for setting different behaviours for synchronizing entities.
 */
var net_ent_flags = 0;


/**
 * Initialized the network synchronisation for the entity.
 * This function will modify the position of the entity on all participants of the network session but the creator.
 * Also the function changes the entities event, smask and emask.
 * \param	ent		The entity to be synchronized.
 */
void net_entsync(ENTITY *ent);

/**
 * This is the default event for a network synchronized entity. If you want to use a custom event,
 * call this function in your entities event to provide correct network synchronisation.
 */
void net_entsync_event();

#include "network.c"
#endif