#include <acknex.h>
#include <windows.h>
#include "webkit.h"

#define _PC_53	0x00010000
#define _MCW_PC	0x00030000

void wke_init_private();
unsigned int _controlfp(int newvalue, int mask);

void wke_init()
{
	_controlfp(_PC_53, _MCW_PC);
	wke_init_private();
}

// No direct implementation, just setup the correct API
#define PRAGMA_API _controlfp;msvcrt!_controlfp


#define PRAGMA_API wke_init_private;wke!wkeInit	// We need to call _controlfp in wke_init, so use a private function instead
#define PRAGMA_API wke_shutdown;wke!wkeShutdown
#define PRAGMA_API wke_update;wke!wkeUpdate
#define PRAGMA_API wke_version;wke!wkeVersion
#define PRAGMA_API wke_version_string;wke!wkeVersionString

#define PRAGMA_API wke_create_webView;wke!wkeCreateWebView
#define PRAGMA_API wke_get_webView;wke!wkeGetWebView
#define PRAGMA_API wke_destroy_webView;wke!wkeDestroyWebView

#define PRAGMA_API wke_webView_name;wke!wkeWebViewName
#define PRAGMA_API wke_set_webView_name;wke!wkeSetWebViewName

#define PRAGMA_API wke_is_transparent;wke!wkeIsTransparent
#define PRAGMA_API wke_set_transparent;wke!wkeSetTransparent

#define PRAGMA_API wke_load_url;wke!wkeLoadURL
#define PRAGMA_API wke_load_html;wke!wkeLoadHTML
#define PRAGMA_API wke_load_file;wke!wkeLoadFile

#define PRAGMA_API wke_is_loaded;wke!wkeIsLoaded
#define PRAGMA_API wke_is_load_failed;wke!wkeIsLoadFailed
#define PRAGMA_API wke_is_load_complete;wke!wkeIsLoadComplete
#define PRAGMA_API wke_is_document_ready;wke!wkeIsDocumentReady
#define PRAGMA_API wke_is_loading;wke!wkeIsLoading
#define PRAGMA_API wke_stop_loading;wke!wkeStopLoading
#define PRAGMA_API wke_reload;wke!wkeReload

#define PRAGMA_API wke_title;wke!wkeTitle

#define PRAGMA_API wke_resize;wke!wkeResize
#define PRAGMA_API wke_width;wke!wkeWidth
#define PRAGMA_API wke_height;wke!wkeHeight

#define PRAGMA_API wke_contents_width;wke!wkeContentsWidth
#define PRAGMA_API wke_contents_height;wke!wkeContentsHeight

#define PRAGMA_API wke_set_dirty;wke!wkeSetDirty
#define PRAGMA_API wke_is_dirty;wke!wkeIsDirty
#define PRAGMA_API wke_add_dirty_area;wke!wkeAddDirtyArea
#define PRAGMA_API wke_layout_if_needed;wke!wkeLayoutIfNeeded
#define PRAGMA_API wke_paint;wke!wkePaint

#define PRAGMA_API wke_can_go_back;wke!wkeCanGoBack
#define PRAGMA_API wke_go_back;wke!wkeGoBack
#define PRAGMA_API wke_can_go_forward;wke!wkeCanGoForward
#define PRAGMA_API wke_go_forward;wke!wkeGoForward

#define PRAGMA_API wke_select_all;wke!wkeSelectAll
#define PRAGMA_API wke_copy;wke!wkeCopy
#define PRAGMA_API wke_cut;wke!wkeCut
#define PRAGMA_API wke_paste;wke!wkePaste
#define PRAGMA_API wke_delete;wke!wkeDelete

#define PRAGMA_API wke_set_cookie_enabled;wke!wkeSetCookieEnabled
#define PRAGMA_API wke_cookie_enabled;wke!wkeCookieEnabled

#define PRAGMA_API wke_set_media_volume;wke!wkeSetMediaVolume
#define PRAGMA_API wke_media_volume;wke!wkeMediaVolume

#define PRAGMA_API wke_mouse_event;wke!wkeMouseEvent
#define PRAGMA_API wke_context_menu_event;wke!wkeContextMenuEvent
#define PRAGMA_API wke_mouse_wheel;wke!wkeMouseWheel
#define PRAGMA_API wke_key_up;wke!wkeKeyUp
#define PRAGMA_API wke_key_down;wke!wkeKeyDown
#define PRAGMA_API wke_key_press;wke!wkeKeyPress

#define PRAGMA_API wke_focus;wke!wkeFocus
#define PRAGMA_API wke_unfocus;wke!wkeUnfocus

#define PRAGMA_API wke_get_caret;wke!wkeGetCaret

#define PRAGMA_API wke_run_js;wke!wkeRunJS

#define PRAGMA_API wke_global_exec;wke!wkeGlobalExec

#define PRAGMA_API wke_sleep;wke!wkeSleep
#define PRAGMA_API wke_awaken;wke!wkeAwaken
#define PRAGMA_API wke_is_awake;wke!wkeIsAwake

#define PRAGMA_API wke_set_zoom_factor;wke!wkeSetZoomFactor
#define PRAGMA_API wke_zoom_factor;wke!wkeZoomFactor

#define PRAGMA_API wke_set_editable;wke!wkeSetEditable
















