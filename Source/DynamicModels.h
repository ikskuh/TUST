#ifndef _DYNAMICMODELS_H_
#define _DYNAMICMODELS_H_

#include <acknex.h>
#include <d3d9.h>

/**
 * \file DynamicModels.h
 * \brief Library for dynamic model creation
 *
 * DynamicModels.h allows you to create new models at runtime. 
 */

/**
 * A single face definition of a dynamic model.
 */
typedef struct
{
	/**
	 * Array of three vertices. The vertices are ordered CCW.
	 */
	D3DVERTEX v[3];
} DYNAMIC_FACE;


/**
 * A two-faced quad definition of a dynamic model.
 */
typedef struct
{
	/**
	 * Array of three vertices. The vertices are ordered CCW.
	 * \image html dynamic_quad_order.png
	 */
	D3DVERTEX v[4];
} DYNAMIC_QUAD;

/**
 * Maximum vertex count of a dynamic model.
 */
#define DMDL_MAX_VERTEXCOUNT 65536

/**
 * Maximum index count of a dynamic model.
 */
#define DMDL_MAX_INDEXCOUNT 128000

/** 
 * A dynamic model. It contains all data to build a mesh from it.
 */
typedef struct
{
	/**
	 * Number of vertices stored in the model.
	 */
	int vertexCount;
	
	/**
	 * Number of faces stored in the model.
	 */
	int faceCount;
	
	/**
	 * The vertex buffer of the dynamic model. Contains actual vertex data.
	 */
	D3DVERTEX vertexBuffer[DMDL_MAX_VERTEXCOUNT];
	
	DWORD attributeBuffer[DMDL_MAX_VERTEXCOUNT];
	
	/**
	 * The index buffer of the dynamic model. Contains all faces of the model.
	 */
	short indexBuffer[DMDL_MAX_INDEXCOUNT];
	
	/**
	 * Array with the four first skins of a newly created dynamic entity.
	 */
	BMAP* skin[4];
} DynamicModel;

/** 
 * DynamicModels System Flag.
 * Activates normal fixing when creating the mesh. Normals will be smoothed out and calculated correctly for each vertex.
 */
#define DMDL_FIXNORMALS	(1<<0)

/** 
 * DynamicModels System Flag.
 * Activates mesh optimizations when creating the mesh. Vertices will be welded if they are close together and enables a smoother lighting and faster rendering.
 */
#define DMDL_OPTIMIZE	(1<<1)

/** 
 * DynamicModels System Flag.
 * Clones the textures of a newly created entity from the original ones.
 */
#define DMDL_CLONE_TEX	(1<<2)

/** 
 * Save dynamic models in a binary x format.
 */
#define D3DXF_FILEFORMAT_BINARY		0

/** 
 * Save dynamic models in a text x format.
 */
#define D3DXF_FILEFORMAT_TEXT		1

/** 
 * Save dynamic models in a compressed x format.
 */
#define D3DXF_FILEFORMAT_COMPRESSED	2

typedef struct
{
	/**
	 * The x format DynamicModels should save its meshes with.
	 */
	int xFormat;
	
	/**
	 * DynamicModels System Flags. Enable or disable the system flags to change the behaviour of DynamicModels.
	 */
	long flags;
} DynamicModelSettings;

///
/// All settings of DynamicModels can be setup in this variable
///
DynamicModelSettings DMDLSettings;

///
/// Returns 0 if the both vertices aren't equal
///
int dmdl_vertex_equals(D3DVERTEX *v1,D3DVERTEX *v2);

///
/// Creates a new dynamic model
///
DynamicModel* dmdl_create();

///
/// Creates a new instance(ENTITY) of the dynamic model
///
ENTITY* dmdl_create_instance(DynamicModel *model, VECTOR* pos, EVENT act);

///
/// Creates a new DirectX mesh for the dynamic model
///
LPD3DXMESH dmdl_create_mesh(DynamicModel *model);

///
/// Frees the memory allocated by the whole dynamic model
///
void dmdl_delete(DynamicModel *model);

///
/// Adds a new polygon to the dynamic model
///
void dmdl_add_face(DynamicModel *model, DYNAMIC_FACE *face);

///
/// Adds an existing entity to the dynamic model
/// 
void dmdl_add_entity(DynamicModel *model, ENTITY *ent);

///
/// Adds an existing mesh to the dynamic model
///
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh);
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset);
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset, ANGLE *rotation);
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset, ANGLE *rotation, VECTOR *scale);

///
/// Adds a new quad to the dynamic model
///
///
void dmdl_add_quad(DynamicModel *model, DYNAMIC_QUAD *quad);

///
/// Saves a dynamic model into a DirectX x file
///
void dmdl_save(DynamicModel *model, char *filename);

#include "DynamicModels.c"

#endif