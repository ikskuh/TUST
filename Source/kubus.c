#include <acknex.h>
#include "kubus.h"
#include "DynamicModels.h"

int kubusBlockSize = 16;
int kubusViewRange = 8;
int kubusForceTotalRefresh = 0;

MATERIAL *kubusChunkMtl = 
{
	effect = "
Texture mtlSkin1;
float4 vecSunDir;
float4x4 matWorld;
float4x4 matWorldViewProj;

sampler smp = sampler_state
{
	Texture = <mtlSkin1>;
};

struct vsData 
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

vsData vs(
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 tex : TEXCOORD0)
{
	vsData data;
	data.pos = mul(pos, matWorldViewProj);
	data.normal = mul(normal, (float3x3)matWorld);
	data.tex = tex;
	return data;
}

float4 ps(vsData data) : COLOR0
{
	float4 intensity = 0.4 + 1.0 * saturate(dot(data.normal, -vecSunDir.xyz));
	intensity.a = 1.0;
	return intensity * tex2D(smp, data.tex);
}

technique alpha_test
{
	pass p0
	{
		ZWriteEnable = true;
		AlphaTestEnable = true;
		AlphaBlendEnable = false;
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}";
	flags = PASS_SOLID;
}

typedef struct 
{
	BlockType *type;
	void *data;
} Block;

typedef struct
{
	int updated;
	int hasFaces;
	ENTITY *entity;
	Block blocks[16][16][16];
} Chunk;

Chunk *kubusChunk[CHUNK_COUNT_X][CHUNK_COUNT_Y][CHUNK_COUNT_Z];

Chunk *kubus_get_chunk(int x, int y, int z)
{
	int cx = x / 16;
	int cy = y / 16;
	int cz = z / 16;
	if(cx >= CHUNK_COUNT_X || cy >= CHUNK_COUNT_Y || cz >= CHUNK_COUNT_Z) return NULL;
	if(kubusChunk[cx][cy][cz] == NULL)
	{
		kubusChunk[cx][cy][cz] = sys_malloc(sizeof(Chunk));
	}
	return kubusChunk[cx][cy][cz];
}

BlockType *kubus_get_block(int x, int y, int z)
{
	if(x < 0 || y < 0 || z < 0) return NULL;
	if(x >= CHUNK_COUNT_X * 16 || y >= CHUNK_COUNT_Y * 16 || z >= CHUNK_COUNT_Z * 16) return NULL;
	Chunk *chunk = kubus_get_chunk(x, y, z);
	if(chunk == NULL) return NULL;
	return (chunk->blocks)[x % 16][y % 16][z % 16].type;
}

void kubus_set_block(int x, int y, int z, BlockType *blockType)
{
	if(x < 0 || y < 0 || z < 0) return;
	if(x >= CHUNK_COUNT_X * 16 || y >= CHUNK_COUNT_Y * 16 || z >= CHUNK_COUNT_Z * 16) return;
	Chunk *chunk = kubus_get_chunk(x, y, z);
	(chunk->blocks)[x % 16][y % 16][z % 16].type = blockType;
	chunk->updated = 0;
	
	if(x % 16 == 0)
	{
		chunk = kubus_get_chunk(x - 1, y, z);
		if(chunk != NULL) chunk->updated = 0;
	}
	if(y % 16 == 0)
	{
		chunk = kubus_get_chunk(x, y - 1, z);
		if(chunk != NULL) chunk->updated = 0;
	}
	if(z % 16 == 0)
	{
		chunk = kubus_get_chunk(x, y, z - 1);
		if(chunk != NULL) chunk->updated = 0;
	}
	if(x % 16 == 15)
	{
		chunk = kubus_get_chunk(x + 1, y, z);
		if(chunk != NULL) chunk->updated = 0;
	}
	if(y % 16 == 15)
	{
		chunk = kubus_get_chunk(x, y + 1, z);
		if(chunk != NULL) chunk->updated = 0;
	}
	if(z % 16 == 15)
	{
		chunk = kubus_get_chunk(x, y, z + 1);
		if(chunk != NULL) chunk->updated = 0;
	}
}

void *kubus_get_blockdata(int x, int y, int z)
{
	if(x < 0 || y < 0 || z < 0) return NULL;
	if(x >= CHUNK_COUNT_X * 16 || y >= CHUNK_COUNT_Y * 16 || z >= CHUNK_COUNT_Z * 16) return NULL;
	Chunk *chunk = kubus_get_chunk(x, y, z);
	if(chunk == NULL) return NULL;
	return (chunk->blocks)[x % 16][y % 16][z % 16].data;
}

void kubus_set_blockdata(int x, int y, int z, void *data)
{
	if(x < 0 || y < 0 || z < 0) return;
	if(x >= CHUNK_COUNT_X * 16 || y >= CHUNK_COUNT_Y * 16 || z >= CHUNK_COUNT_Z * 16) return;
	Chunk *chunk = kubus_get_chunk(x, y, z);
	if(chunk == NULL) return;
	(chunk->blocks)[x % 16][y % 16][z % 16].data = data;
}

void kubus_quad_top(DYNAMIC_QUAD *quad, int x, int y, int z)
{
	quad->v[0].x = kubusBlockSize * (x - 0.5);
	quad->v[0].z = kubusBlockSize * (y - 0.5);
	quad->v[0].y = kubusBlockSize * (z + 0.5);
	
	quad->v[1].x = kubusBlockSize * (x + 0.5);
	quad->v[1].z = kubusBlockSize * (y - 0.5);
	quad->v[1].y = kubusBlockSize * (z + 0.5);
	
	quad->v[2].x = kubusBlockSize * (x - 0.5);
	quad->v[2].z = kubusBlockSize * (y + 0.5);
	quad->v[2].y = kubusBlockSize * (z + 0.5);
	
	quad->v[3].x = kubusBlockSize * (x + 0.5);
	quad->v[3].z = kubusBlockSize * (y + 0.5);
	quad->v[3].y = kubusBlockSize * (z + 0.5);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		quad->v[i].nx = 0;
		quad->v[i].ny = 1;
		quad->v[i].nz = 0;
	}
}

void kubus_quad_bottom(DYNAMIC_QUAD *quad, int x, int y, int z)
{
	quad->v[0].x = kubusBlockSize * (x - 0.5);
	quad->v[0].z = kubusBlockSize * (y - 0.5);
	quad->v[0].y = kubusBlockSize * (z - 0.5);
	
	quad->v[1].x = kubusBlockSize * (x - 0.5);
	quad->v[1].z = kubusBlockSize * (y + 0.5);
	quad->v[1].y = kubusBlockSize * (z - 0.5);
	
	quad->v[2].x = kubusBlockSize * (x + 0.5);
	quad->v[2].z = kubusBlockSize * (y - 0.5);
	quad->v[2].y = kubusBlockSize * (z - 0.5);
	
	quad->v[3].x = kubusBlockSize * (x + 0.5);
	quad->v[3].z = kubusBlockSize * (y + 0.5);
	quad->v[3].y = kubusBlockSize * (z - 0.5);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		quad->v[i].nx = 0;
		quad->v[i].ny = -1;
		quad->v[i].nz = 0;
	}
}

void kubus_quad_north(DYNAMIC_QUAD *quad, int x, int y, int z)
{
	quad->v[0].x = kubusBlockSize * (x - 0.5);
	quad->v[0].z = kubusBlockSize * (y - 0.5);
	quad->v[0].y = kubusBlockSize * (z - 0.5);
	
	quad->v[1].x = kubusBlockSize * (x - 0.5);
	quad->v[1].z = kubusBlockSize * (y - 0.5);
	quad->v[1].y = kubusBlockSize * (z + 0.5);
	
	quad->v[2].x = kubusBlockSize * (x - 0.5);
	quad->v[2].z = kubusBlockSize * (y + 0.5);
	quad->v[2].y = kubusBlockSize * (z - 0.5);
	
	quad->v[3].x = kubusBlockSize * (x - 0.5);
	quad->v[3].z = kubusBlockSize * (y + 0.5);
	quad->v[3].y = kubusBlockSize * (z + 0.5);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		quad->v[i].nx = -1;
		quad->v[i].ny = 0;
		quad->v[i].nz = 0;
	}
}

void kubus_quad_south(DYNAMIC_QUAD *quad, int x, int y, int z)
{
	quad->v[0].x = kubusBlockSize * (x + 0.5);
	quad->v[0].z = kubusBlockSize * (y - 0.5);
	quad->v[0].y = kubusBlockSize * (z - 0.5);
	
	quad->v[1].x = kubusBlockSize * (x + 0.5);
	quad->v[1].z = kubusBlockSize * (y + 0.5);
	quad->v[1].y = kubusBlockSize * (z - 0.5);
	
	quad->v[2].x = kubusBlockSize * (x + 0.5);
	quad->v[2].z = kubusBlockSize * (y - 0.5);
	quad->v[2].y = kubusBlockSize * (z + 0.5);
	
	quad->v[3].x = kubusBlockSize * (x + 0.5);
	quad->v[3].z = kubusBlockSize * (y + 0.5);
	quad->v[3].y = kubusBlockSize * (z + 0.5);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		quad->v[i].nx = 1;
		quad->v[i].ny = 0;
		quad->v[i].nz = 0;
	}
}

void kubus_quad_east(DYNAMIC_QUAD *quad, int x, int y, int z)
{
	quad->v[0].x = kubusBlockSize * (x - 0.5);
	quad->v[0].z = kubusBlockSize * (y + 0.5);
	quad->v[0].y = kubusBlockSize * (z - 0.5);
	
	quad->v[1].x = kubusBlockSize * (x - 0.5);
	quad->v[1].z = kubusBlockSize * (y + 0.5);
	quad->v[1].y = kubusBlockSize * (z + 0.5);
	
	quad->v[2].x = kubusBlockSize * (x + 0.5);
	quad->v[2].z = kubusBlockSize * (y + 0.5);
	quad->v[2].y = kubusBlockSize * (z - 0.5);
	
	quad->v[3].x = kubusBlockSize * (x + 0.5);
	quad->v[3].z = kubusBlockSize * (y + 0.5);
	quad->v[3].y = kubusBlockSize * (z + 0.5);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		quad->v[i].nx = 0;
		quad->v[i].ny = 0;
		quad->v[i].nz = 1;
	}
}

void kubus_quad_west(DYNAMIC_QUAD *quad, int x, int y, int z)
{
	quad->v[0].x = kubusBlockSize * (x - 0.5);
	quad->v[0].z = kubusBlockSize * (y - 0.5);
	quad->v[0].y = kubusBlockSize * (z - 0.5);
	
	quad->v[1].x = kubusBlockSize * (x + 0.5);
	quad->v[1].z = kubusBlockSize * (y - 0.5);
	quad->v[1].y = kubusBlockSize * (z - 0.5);
	
	quad->v[2].x = kubusBlockSize * (x - 0.5);
	quad->v[2].z = kubusBlockSize * (y - 0.5);
	quad->v[2].y = kubusBlockSize * (z + 0.5);
	
	quad->v[3].x = kubusBlockSize * (x + 0.5);
	quad->v[3].z = kubusBlockSize * (y - 0.5);
	quad->v[3].y = kubusBlockSize * (z + 0.5);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		quad->v[i].nx = 0;
		quad->v[i].ny = 0;
		quad->v[i].nz = -1;
	}
}

int kubusTextureConfig[6][4];

void kubus_quad_tex(DYNAMIC_QUAD *quad, int texID, int configID)
{
	float texSize = 1.0 / 16.0;
	quad->v[kubusTextureConfig[configID][0]].u1 = texSize * (texID % 16);
	quad->v[kubusTextureConfig[configID][0]].v1 = texSize * (texID / 16);
	
	quad->v[kubusTextureConfig[configID][1]].u1 = texSize * (texID % 16);
	quad->v[kubusTextureConfig[configID][1]].v1 = texSize * (texID / 16 + 1);
	
	quad->v[kubusTextureConfig[configID][2]].u1 = texSize * (texID % 16 + 1);
	quad->v[kubusTextureConfig[configID][2]].v1 = texSize * (texID / 16);
	
	quad->v[kubusTextureConfig[configID][3]].u1 = texSize * (texID % 16 + 1);
	quad->v[kubusTextureConfig[configID][3]].v1 = texSize * (texID / 16 + 1);
}

int kubus_compare_blocks(BlockType *blockA, BlockType *blockB)
{
	if(blockA == blockB) return 0;
	
	if(blockA == NULL) return 1;
	if(blockB == NULL) return 1;
	
	if(blockA->flags & TRANSLUCENT) return 1;
	if(blockB->flags & TRANSLUCENT) return 1;
	
	if(blockA->construct) return 1;
	if(blockB->construct) return 1;
	
	return 0;
}

int kubus_update_chunk(int cx, int cy, int cz)
{
	Chunk *chunk = kubusChunk[cx][cy][cz];
	if(chunk == NULL) return 0;
	
	if(chunk->entity == NULL)
	{
		chunk->entity = ent_create(CUBE_MDL, vector(16 * kubusBlockSize * cx, 16 * kubusBlockSize * cy, 16 * kubusBlockSize * cz), NULL);
		//wait(1);
		ent_clone(chunk->entity);
		ent_setmesh(chunk->entity, NULL, 0, 0);
		chunk->entity->flags = NOFILTER | INVISIBLE;
		chunk->entity->material = kubusChunkMtl;
		return;
	}
	
	var dist = vec_dist(vector(kubusBlockSize * 16 * cx, kubusBlockSize * 16 * cy, 0), vector(camera->x, camera->y, 0));
	if(dist > 16 * kubusBlockSize * kubusViewRange)
	{
		chunk->entity->flags |= INVISIBLE;
		return 0;
	}
	
	if(chunk->updated)
	{
		if(chunk->hasFaces)
			chunk->entity->flags &= ~INVISIBLE;
		else
			chunk->entity->flags |= INVISIBLE;
		return 0;
	}
	
	DynamicModel *model = dmdl_create();
	cx *= 16;
	cy *= 16;
	cz *= 16;
	
	int x, y, z;
	DYNAMIC_QUAD quad;
	for(x = 0; x < 16; x++) for(y = 0; y < 16; y++) for(z = 0; z < 16; z++)
	{
		BlockType *type = kubus_get_block(cx + x, cy + y, cz + z);
		if(type == NULL) continue;
		
		if(type->construct == NULL)
		{
			if(kubus_compare_blocks(kubus_get_block(cx + x, cy + y, cz + z - 1), type))
			{
				kubus_quad_tex(&quad, type->textureBottom, 0);
				kubus_quad_bottom(&quad, x, y, z);
				dmdl_add_quad(model, &quad);
			}
			if(kubus_compare_blocks(kubus_get_block(cx + x, cy + y, cz + z + 1), type))
			{
				kubus_quad_tex(&quad, type->textureTop, 1);
				kubus_quad_top(&quad, x, y, z);
				dmdl_add_quad(model, &quad);
			}
			if(kubus_compare_blocks(kubus_get_block(cx + x - 1, cy + y, cz + z), type))
			{
				kubus_quad_tex(&quad, type->textureSide, 2);
				kubus_quad_north(&quad, x, y, z);
				dmdl_add_quad(model, &quad);
			}
			if(kubus_compare_blocks(kubus_get_block(cx + x + 1, cy + y, cz + z), type))
			{
				kubus_quad_tex(&quad, type->textureSide, 3);
				kubus_quad_south(&quad, x, y, z);
				dmdl_add_quad(model, &quad);
			}
			if(kubus_compare_blocks(kubus_get_block(cx + x, cy + y - 1, cz + z), type))
			{
				kubus_quad_tex(&quad, type->textureSide, 4);
				kubus_quad_west(&quad, x, y, z);
				dmdl_add_quad(model, &quad);
			}
			if(kubus_compare_blocks(kubus_get_block(cx + x, cy + y + 1, cz + z), type))
			{
				kubus_quad_tex(&quad, type->textureSide, 5);
				kubus_quad_east(&quad, x, y, z);
				dmdl_add_quad(model, &quad);
			}
		}
		else
		{
			void construct(DynamicModel *model, BlockType *type, int x, int y, int z);
			construct = type->construct;
			construct(model, type, x, y, z);
		}
	}
	if(model.faceCount > 0)
	{
		ent_setmesh(chunk->entity, dmdl_create_mesh(model), 0, 0);
		chunk->entity->flags &= ~INVISIBLE;
		chunk->hasFaces = 1;
	}
	else
	{
		ent_setmesh(chunk->entity, NULL, 0, 0);
		chunk->entity->flags |= INVISIBLE;
		chunk->hasFaces = 0;
	}	
	dmdl_delete(model);
	chunk->updated = 1;
	
	return 1;
}

void kubus_update()
{
	int x, y, z, updates = 0;
	
	int minX = maxv(0, (camera->x / (16 * kubusBlockSize)) - kubusViewRange - 1);
	int minY = maxv(0, (camera->y / (16 * kubusBlockSize)) - kubusViewRange - 1);
	
	int maxX = minv(CHUNK_COUNT_X - 1, (camera->x / (16 * kubusBlockSize)) + kubusViewRange + 1);
	int maxY = minv(CHUNK_COUNT_Y - 1, (camera->y / (16 * kubusBlockSize)) + kubusViewRange + 1);
	
	if(kubusForceTotalRefresh != 0)
	{
		minX = 0; minY = 0;
		maxX = CHUNK_COUNT_X - 1; maxY = CHUNK_COUNT_Y - 1;
		kubusForceTotalRefresh = 0;
	}
	
	for(x = minX; x <= maxX; x++) for(y = minY; y <= maxY; y++) for(z = 0; z < CHUNK_COUNT_Z; z++)
	{
		if(updates >= 5) break;
		updates += kubus_update_chunk(x, y, z);
	}
}

void kubus_init(BMAP *texpack)
{
	DMDLSettings.flags = 0;
	int x, y, z;
	for(x = 0; x < CHUNK_COUNT_X; x++) for(y = 0; y < CHUNK_COUNT_Y; y++) for(z = 0; z < CHUNK_COUNT_Z; z++)
	{
		kubusChunk[x][y][z] = NULL;
	}
	
	kubusChunkMtl->skin1 = texpack;
	
	// Bottom
	kubusTextureConfig[0][0] = 0;
	kubusTextureConfig[0][1] = 1;
	kubusTextureConfig[0][2] = 2;
	kubusTextureConfig[0][3] = 3;
	
	// Top
	kubusTextureConfig[1][0] = 0;
	kubusTextureConfig[1][1] = 1;
	kubusTextureConfig[1][2] = 2;
	kubusTextureConfig[1][3] = 3;
	
	// North
	kubusTextureConfig[2][0] = 1;
	kubusTextureConfig[2][1] = 0;
	kubusTextureConfig[2][2] = 3;
	kubusTextureConfig[2][3] = 2;
	
	// South
	kubusTextureConfig[3][0] = 2;
	kubusTextureConfig[3][1] = 0;
	kubusTextureConfig[3][2] = 3;
	kubusTextureConfig[3][3] = 1;
	
	// West
	kubusTextureConfig[4][0] = 2;
	kubusTextureConfig[4][1] = 0;
	kubusTextureConfig[4][2] = 3;
	kubusTextureConfig[4][3] = 1;
	
	// East
	kubusTextureConfig[5][0] = 1;
	kubusTextureConfig[5][1] = 0;
	kubusTextureConfig[5][2] = 3;
	kubusTextureConfig[5][3] = 2;
}

VECTOR *kubus_pos_from_hit(void)
{
	VECTOR pos;
	pos.x = (hit.x / 16) + 0.5 - 0.5 * hit.nx;
	pos.y = (hit.y / 16) + 0.5 - 0.5 * hit.ny;
	pos.z = (hit.z / 16) + 0.5 - 0.5 * hit.nz;
	return vector(pos.x, pos.y, pos.z);
}

void kubus_force_refresh()
{
	kubusForceTotalRefresh = 1;
}

void kubus_set_viewrange(int range)
{
	if(range < kubusViewRange)
		kubus_force_refresh();
	kubusViewRange = range;
}

int kubus_get_viewrange()
{
	return kubusViewRange;
}



















