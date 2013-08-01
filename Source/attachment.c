#include <acknex.h>
#include "attachment.h"

void attach_entity(ENTITY *ent, ENTITY *_target, VECTOR *offset, ANGLE *angularOffset)
{
	if(ent == NULL || _target == NULL || offset == NULL || angularOffset == NULL) return;
	vec_set(&ent->x, offset);
	vec_rotate(&ent->x, &_target->pan);
	vec_add(&ent->x, &_target->x);
	
	vec_set(&ent->pan, &_target->pan);
	vec_add(&ent->pan, angularOffset);
}

void attach_entity_to_vertex(ENTITY *ent, ENTITY *_target, int vertexID, VECTOR *offset, ANGLE *angularOffset)
{
	if(ent == NULL || _target == NULL || offset == NULL || angularOffset == NULL) return;
	VECTOR temp;
	vec_set(&ent->x, offset);
	vec_rotate(&ent->x, &_target->pan);
	vec_for_vertex(&temp, _target, vertexID);
	vec_add(&ent->x, &temp);
	
	vec_set(&ent->pan, &_target->pan);
	vec_add(&ent->pan, angularOffset);
}

void attach_entity_to_vertices(ENTITY *ent, ENTITY *_target, int vertexPosID, int vertexAngID, VECTOR *offset, ANGLE *angularOffset)
{
	if(ent == NULL || _target == NULL || offset == NULL || angularOffset == NULL) return;
	VECTOR temp, temp1;
	ANGLE angle;
	vec_diff(&angle, vec_for_vertex(&temp1, _target, vertexAngID), vec_for_vertex(&temp, _target, vertexPosID));
	vec_to_angle(&angle, &angle);
	
	vec_set(&ent->x, offset);
	vec_rotate(&ent->x, &_target->pan);
	vec_add(&ent->x, &temp); // We have stored the position in vec_diff
	
	vec_set(&ent->pan, &angle);
	vec_add(&ent->pan, angularOffset);
}



















