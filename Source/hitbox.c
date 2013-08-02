#include <acknex.h>
#include "hitbox.h"

typedef struct HitboxAttachment
{
	void getPosition(struct HitboxAttachment *attachment, ENTITY *ent, VECTOR *pos);
} HitboxAttachment;

typedef struct HitboxVertexAttachment
{
	void getPosition(struct HitboxVertexAttachment *attachment, ENTITY *ent, VECTOR *pos);
	int v1, v2;
} HitboxVertexAttachment;

typedef struct Hitbox
{
	ENTITY *box;
	HitboxAttachment *start;
	HitboxAttachment *end;
} Hitbox;

CharacterShape *charshape_attach(ENTITY *ent)
{
	CharacterShape *shape = sys_nxalloc(sizeof(CharacterShape));
	shape->character = ent;
	shape->hitboxList = list_create();
	return shape;
}

void charshape_update(CharacterShape *shape)
{
	if(shape == NULL) return;
	ListIterator *it = list_begin_iterate(shape->hitboxList);
	Hitbox *box;
	for(box = list_iterate(it); box != NULL; box = list_iterate(it))
	{
		void getPosition(HitboxAttachment *attachment, ENTITY *ent, VECTOR *pos);
		VECTOR start, end;
		
		getPosition = box->start->getPosition; getPosition(box->start, shape->character, &start);
		getPosition = box->end->getPosition; getPosition(box->end, shape->character, &end);
		
		vec_set(&box->box->x, &start);
		box->box->scale_x = vec_dist(&start, &end) / box->box->max_x;
		vec_diff(&box->box->pan, &end, &start);
		vec_to_angle(&box->box->pan, &box->box->pan);
	}
	list_end_iterate(it);
}

void charshape_vertex_attachment(HitboxVertexAttachment *attachment, ENTITY *ent, VECTOR *pos)
{
	VECTOR p2;	
	vec_for_vertex(pos, ent, attachment->v1);
	if(attachment->v2 != 0)
	{
		vec_for_vertex(p2, ent, attachment->v2);
		vec_lerp(pos, pos, p2, 0.5);
	}
}

ENTITY* charshape_add(CharacterShape *shape, STRING *model, int vertexStart, int vertexEnd)
{
	charshape_add(shape, model, vertexStart, vertexStart, vertexEnd, vertexEnd);
}

ENTITY* charshape_add(CharacterShape *shape, STRING *model, int vertexStartA, int vertexStartB, int vertexEndA, int vertexEndB)
{
	if(shape == NULL) return;
	HitboxVertexAttachment *start = sys_nxalloc(sizeof(HitboxVertexAttachment));
	HitboxVertexAttachment *end = sys_nxalloc(sizeof(HitboxVertexAttachment));
	
	start->v1 = vertexStartA;
	start->v2 = vertexStartB;
	end->v1 = vertexEndA;
	end->v2 = vertexEndB;
	
	start->getPosition = charshape_vertex_attachment;
	end->getPosition = charshape_vertex_attachment;
	
	Hitbox *box = sys_nxalloc(sizeof(Hitbox));
	box->box = ent_create(model, vector(0, 0, 0), NULL);
	box->box->flags = INVISIBLE;
	box->start = start;
	box->end = end;
	
	list_add(shape->hitboxList, box);
	
	return box->box;
}
