#include <acknex.h>
#include <default.c>


#include "..\\Source\\tust.h"

wkeWebView view = NULL;

BMAP *webTarget = "#1024x768x8888";

PANEL *webView = 
{
	bmap = webTarget;
	flags = SHOW;
}

function on_mouse_left_event()
{
	wke_mouse_event(view, WKE_MSG_LBUTTONDOWN, mouse_pos.x, mouse_pos.y, WKE_LBUTTON);
	while(mouse_left) wait(1);
	wke_mouse_event(view, WKE_MSG_LBUTTONUP, mouse_pos.x, mouse_pos.y, WKE_LBUTTON);
}

function on_mouse_right_event()
{
	wke_mouse_event(view, WKE_MSG_RBUTTONDOWN, mouse_pos.x, mouse_pos.y, WKE_RBUTTON);
	while(mouse_right) wait(1);
	wke_mouse_event(view, WKE_MSG_RBUTTONUP, mouse_pos.x, mouse_pos.y, WKE_RBUTTON);
}

function main()
{
	video_set(1024, 768, 32, 2);
	mouse_mode = 4;
	level_load("..\\Ressources\\Terrains\\small.hmp");
	
	wke_init();
	
	error(str_for_int(NULL, wke_version()));
	error(wke_version_string());
	
	view = wke_create_webView();
	wke_resize(view, screen_size.x, screen_size.y);
	wke_focus(view);
	wke_load_url(view, "http://www.google.de");
	wke_set_transparent(view, 1);

	float zoom = 1;
	while(!key_esc)
	{
		wke_mouse_event(view, WKE_MSG_MOUSEMOVE, mouse_pos.x, mouse_pos.y, 0);
		wke_mouse_wheel(view, mouse_pos.x, mouse_pos.y, mickey.z, 0);
		
		if(key_plus)
		{
			zoom += 0.1 * time_step;
			wke_set_zoom_factor(view, zoom);
		}
		if(key_minusc)
		{
			zoom = maxv(1, zoom - 0.1 * time_step);
			wke_set_zoom_factor(view, zoom);
		}
		
		wke_update();
		if(wke_is_dirty(view))
		{
			var format = bmap_lock(webTarget, 0);
			wke_paint(view, webTarget->finalbits, webTarget->finalpitch);
			bmap_unlock(webTarget);
		}
		wait(1);
	}
	
	wke_shutdown();
	sys_exit(NULL);
}