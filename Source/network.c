#include <acknex.h>
#include "network.h"
#include "math.h"

void net_entsync_event()
{
	switch(event_type)
	{
		case EVENT_RECEIVE:
			if(connection & 1)
			{
				// On server, broadcast skill updates
				send_skill(&my.NET_SKILL_POS_X, SEND_VEC | SEND_ALL);
				send_skill(&my.NET_SKILL_ANG_PAN, SEND_VEC | SEND_ALL);
			}
			break;
	}
}

void net_entsync(ENTITY *ent)
{
	ent.event = net_entsync_event;
	ent.emask |= ENABLE_RECEIVE;
	ent.smask = NOSEND_ALPHA | NOSEND_AMBIENT | NOSEND_ANGLES | NOSEND_ATTACH | NOSEND_COLOR | NOSEND_FLAGS | NOSEND_FRAME | NOSEND_LIGHT | NOSEND_ORIGIN | NOSEND_SCALE | NOSEND_SKIN | NOSEND_SOUND;
	// Wait for network handle
	while(ent.client_id < 0) wait(1);
	
	ent.NET_SKILL_LERPSPEED = net_ent_default_lerpspeed;
	
	proc_mode = PROC_GLOBAL | PROC_LATE;
	
	if(ent.client_id == dplay_id)
	{
		// Controller
		var update = net_ent_sendrate;
		while(handle(ent) != NULL)
		{
			// Send entity updates
			update -= time_step;
			if(update <= 0)
			{
				// Send position if distance since last update greater the minimum send distance
				if(vec_dist(ent.x, ent.NET_SKILL_POS_X) > net_ent_mindist)
				{
					vec_set(ent.NET_SKILL_POS_X, ent.x);
					if(net_ent_flags & NET_ENT_UNRELIABLE)
						send_skill(ent.NET_SKILL_POS_X, SEND_VEC | SEND_ALL | SEND_UNRELIABLE);
					else
						send_skill(ent.NET_SKILL_POS_X, SEND_VEC | SEND_ALL);
				}
				if(vec_dist(ent.pan, ent.NET_SKILL_ANG_PAN) > net_ent_mindiff)
				{
					vec_set(ent.NET_SKILL_ANG_PAN, ent.pan);
					if(net_ent_flags & NET_ENT_UNRELIABLE)
						send_skill(ent.NET_SKILL_ANG_PAN, SEND_VEC | SEND_ALL | SEND_UNRELIABLE);
					else
						send_skill(ent.NET_SKILL_ANG_PAN, SEND_VEC | SEND_ALL);
				}
				update = net_ent_sendrate;
			}
			wait(1);
		}
	}
	else
	{
		// Receiver
		while(handle(ent) != NULL)
		{
			if(net_ent_lerpfactor > 0)
			{
				// Lerping
				vec_lerp(ent.x, ent.x, ent.NET_SKILL_POS_X, net_ent_lerpfactor);
				ang_lerp(ent.pan, ent.pan, ent.NET_SKILL_ANG_PAN, net_ent_lerpfactor);
			}
			else
			{
				// Linear movement
				VECTOR dir;
				vec_diff(dir, ent.NET_SKILL_POS_X, ent.x);
				var dist = vec_length(dir);
				if(dist > 0.5 * net_ent_mindist)
				{
					vec_normalize(dir, minv(ent.NET_SKILL_LERPSPEED * time_step, dist));
					vec_add(ent.x, dir);
				}
				vec_set(ent.pan, ent.NET_SKILL_ANG_PAN);
			}
			wait(1);
		}
	}
}