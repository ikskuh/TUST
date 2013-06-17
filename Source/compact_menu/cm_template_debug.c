
#ifdef CM_DEBUG_PANEL
//PANEL *panCMDebug;

TEXT *txtCMDebug =
{
	string = (
		".line=0",
		"window.submenu=txtCMDebugWindow",
		"camera.submenu=txtCMDebugCamera",
		"ms/frame.submenu=txtCMDebugTimes",
		"count of.submenu=txtCMDebugCountOf",
		"memory MB.submenu=txtCMDebugMemory",
		"network.submenu=txtCMDebugNetwork",
		".line=2"
	);
}

var nCMfps = 0;
var fncCMfps ()
{
	return ( cmdigitValue * 0.9 ) + ( 1.6 / time_frame );
}

var nCMpps = 0;
var fncCMpps ()
{
	return ( cmdigitValue * 0.9 ) + ( (0.16*num_vistriangles)/(100*time_frame) );
}

TEXT *txtCMDebugWindow =
{
	string = (
		".line=0",
		"width.digit=0,screen_size.x",
		"height.digit=0,screen_size.y",
		".space=1",
		"fps.digit=0,nCMfps,fncCMfps",
		"Kpps.digit=0,nCMpps,fncCMpps",
		".line=2"
	);
}

TEXT *txtCMDebugCamera =
{
	string = (
		".line=0",
		"x.digit=3,camera->x",
		"y.digit=3,camera->y",
		"z.digit=3,camera->z",
		".space=1",
		"pan.digit=3,camera->pan",
		"tilt.digit=3,camera->tilt",
		"roll.digit=3,camera->roll",
		".line=2"
	);
}

TEXT *txtCMDebugTimes =
{
	string = (
		".line=0",
		"idle.digit=2,time_idle",
		"refresh.digit=2,time_update",
		"actions.digit=2,time_actions",
		"functions.digit=2,time_pfuncs",
		"level.digit=2,time_level",
		"sky.digit=2,time_sky",
		"entities.digit=2,time_entities",
		"particles.digit=2,time_effects",
		"draws.digit=2,time_draw",
		"panels.digit=2,time_panels",
		"server.digit=2,time_server",
		"client.digit=2,time_client",
		"physX.digit=2,time_physics",
		"scope.digit=2,time_scope",
		".line=2"
	);
}

TEXT *txtCMDebugCountOf =
{
	string = (
		".line=0",
		"entities.digit=0,num_entities",
		"visible.digit=0,num_visents",
		"entpolys.digit=0,num_visentpolys",
		"mappolys.digit=0,num_vismappolys",
		"triangles.digit=0,num_vistriangles",
		"particles.digit=0,num_particles",
		"lights.digit=0,num_lights",
		"media.digit=0,num_mediastreams",
		"sounds.digit=0,num_sounds",
		"functions.digit=0,num_actions",
		"bodies.digit=0,num_bodies",
		"joysticks.digit=0,num_joysticks",
		".line=2"
	);
}

TEXT *txtCMDebugMemory =
{
	string = (
		".line=0",
		"nexus.digit=1,nexus",
		"memory.digit=1,sys_memory",
		"geometry.digit=1,d3d_texsurfs",
		"shadowmaps.digit=1,d3d_texsmaps",
		"models.digit=1,d3d_texskins",
		"free.digit=1,d3d_texfree",
		".line=2"
	);
}

TEXT *txtCMDebugNetwork =
{
	string = (
		".line=0",
		"status.digit=0,dplay_status",
		"latency.digit=0,dplay_latency",
		"bps.digit=0,dplay_bps",
		"reliable.digit=0,dplay_bpsreliable",
		"unreliable.digit=0,dplay_bpsunreliable",
		"compression.digit=0,dplay_compression",
		"clients.digit=0,num_clients",
		".line=2"
	);
}

#endif

