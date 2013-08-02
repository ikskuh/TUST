#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "list.h"

/**
 
 * Simple hitbox system for bones and vertex attachments
 */
 
 /**
 * \defgroup  HitboxtModule Hitbox System
 * \ingroup   GenericScriptLibrary
 * \brief     Provides functions to create a simplified shape for a character hitbox.
 *
 * \{
 *
 * \file hitbox.h
 * \brief Header of the hitbox module.
 *
 * \file hitbox.c
 * \brief Code of the hitbox module.
 */

/**
 * \brief The shape of a character.
 *
 * Represents a character shape. This shape is made out of different hitboxes that will
 * be position by \ref charshape_update.
 */
typedef struct CharacterShape
{
	ENTITY *character;
	List *hitboxList;
} CharacterShape;

/**
 * \brief Attaches an character shape to an entity.
 *
 * A character shape is a container for hitboxes.
 * \param  ent The entity where the character shape should be attached.
 * \return     The created character shape.
 */
CharacterShape *charshape_attach(ENTITY *ent);

/**
 * \brief Updates an character shape and all hitbox positions.
 *
 * Needs to be called periodically.
 * \param shape The character shape to be updated.
 */
void charshape_update(CharacterShape *shape);

/**
 * \brief Adds a new hitbox to the character shape.
 * \param  shape       The character shape where the hitbox should be added.
 * \param  model       The hitbox model.
 * \param  vertexStart The vertex where the hitbox begins.
 * \param  vertexEnd   The vertex where the hitbox ends.
 * \return             The entity of the hitbox.
 */
ENTITY* charshape_add(CharacterShape *shape, STRING *model, int vertexStart, int vertexEnd);

/**
 * \brief Adds a new hitbox to the character shape.
 * \param  shape        The character shape where the hitbox should be added.
 * \param  model        The hitbox model.
 * \param  vertexStartA The first vertex where the hitbox begins. The resulting position is exactly between the two start vertices.
 * \param  vertexStartB The second vertex where the hitbox begins. The resulting position is exactly between the two start vertices.
 * \param  vertexEndA   The first vertex where the hitbox ends. The resulting position is exactly between the two end vertices.
 * \param  vertexEndB   The second vertex where the hitbox ends. The resulting position is exactly between the two end vertices.
 * \return              The entity of the hitbox.
 */
ENTITY* charshape_add(CharacterShape *shape, STRING *model, int vertexStartA, int vertexStartB, int vertexEndA, int vertexEndB);

#include "hitbox.c"
#endif