#ifndef _ATTACHMENT_H_
#define _ATTACHMENT_H_

/**
 * \file attachment.h
 * Provides functions to attach entities to other entities
 */

/**
 * Attaches an entity to a parent entity. This function is a single action,
 * to get continous attachments you need to call this function every frame.
 * \param ent           The entity to be attached.
 * \param target        The entity where ent should be attached.
 * \param offset        The attachments position offset.
 * \param angularOffset The attachments rotation offset.
 */
void attach_entity(ENTITY *ent, ENTITY *target, VECTOR *offset, ANGLE *angularOffset);

/**
 * Attaches an entity to a parent entity by a vertex position. This function is a
 * single action, to get continous attachments you need to call this function every frame.
 * \param ent           The entity to be attached.
 * \param target        The entity where ent should be attached.
 * \param vertexID      The vertex in the parent entity where ent should be attached.
 * \param offset        The attachments position offset.
 * \param angularOffset The attachments rotation offset.
 */
void attach_entity_to_vertex(ENTITY *ent, ENTITY *_target, int vertexID, VECTOR *offset, ANGLE *angularOffset);

/**
 * Attaches an entity to a parent entity by a vertex position and a second vertex for rotation.
 * This function is a single action, to get continous attachments
 * you need to call this function every frame.
 * \param ent           The entity to be attached.
 * \param target        The entity where ent should be attached.
 * \param vertexPosID   The vertex in the parent entity where ent should be attached.
 * \param vertexAngID   The vertex in the parent entity to which ent should be rotated.
 * \param offset        The attachments position offset.
 * \param angularOffset The attachments rotation offset.
 */
void attach_entity_to_vertices(ENTITY *ent, ENTITY *_target, int vertexPosID, int vertexAngID, VECTOR *offset, ANGLE *angularOffset);

#include "attachment.c"
#endif