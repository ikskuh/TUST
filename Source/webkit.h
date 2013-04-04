#ifndef _WEBKIT_H_
#define _WEBKIT_H_

/**
 * \file webkit.h
 * Provides functions to interface WebKit, a HTML rendering and javascript engine.
 */

typedef void* wkeWebView;

typedef struct
{
    int x;
    int y;
    int w;
    int h;
} wkeRect;

#define WKE_LBUTTON		0x01
#define WKE_RBUTTON		0x02
#define WKE_SHIFT		0x04
#define WKE_CONTROL		0x08
#define WKE_MBUTTON		0x10

#define WKE_EXTENDED = 0x0100
#define WKE_REPEAT = 0x4000

#define WKE_MSG_MOUSEMOVE       0x0200
#define WKE_MSG_LBUTTONDOWN     0x0201
#define WKE_MSG_LBUTTONUP       0x0202
#define WKE_MSG_LBUTTONDBLCLK   0x0203
#define WKE_MSG_RBUTTONDOWN     0x0204
#define WKE_MSG_RBUTTONUP       0x0205
#define WKE_MSG_RBUTTONDBLCLK   0x0206
#define WKE_MSG_MBUTTONDOWN     0x0207
#define WKE_MSG_MBUTTONUP       0x0208
#define WKE_MSG_MBUTTONDBLCLK   0x0209
#define WKE_MSG_MOUSEWHEEL      0x020A

typedef void* jsExecState;
typedef long jsValue;

void wke_init();
void wke_shutdown();
void wke_update();
unsigned int wke_version();
const char* wke_version_string();

//typedef void* (*FILE_OPEN) (const char* path);
//typedef void (*FILE_CLOSE) (void* handle);
//typedef size_t (*FILE_SIZE) (void* handle);
//typedef int (*FILE_READ) (void* handle, void* buffer, size_t size);
//typedef int (*FILE_SEEK) (void* handle, int offset, int origin);
//
//void wkeSetFileSystem(FILE_OPEN pfn_open, FILE_CLOSE pfn_close, FILE_SIZE pfn_size, FILE_READ pfn_read, FILE_SEEK pfn_seek);


wkeWebView wke_create_webView();
wkeWebView wke_get_webView(const char* name);
void wke_destroy_webView(wkeWebView webView);

const char* wke_webView_name(wkeWebView webView);
void wke_set_webView_name(wkeWebView webView, const char* name);

bool wke_is_transparent(wkeWebView webView);
void wke_set_transparent(wkeWebView webView, bool transparent);

void wke_load_url(wkeWebView webView, const char* url);
void wke_load_html(wkeWebView webView, const char* html);
void wke_load_file(wkeWebView webView, const char* filename);

bool wke_is_loaded(wkeWebView webView);
bool wke_is_load_failed(wkeWebView webView);
bool wke_is_load_complete(wkeWebView webView);
bool wke_is_document_ready(wkeWebView webView);
bool wke_is_loading(wkeWebView webView);
void wke_stop_loading(wkeWebView webView);
void wke_reload(wkeWebView webView);

const char* wke_title(wkeWebView webView);

void wke_resize(wkeWebView webView, int w, int h);
int wke_width(wkeWebView webView);
int wke_height(wkeWebView webView);

int wke_contents_width(wkeWebView webView);
int wke_contents_height(wkeWebView webView);

void wke_set_dirty(wkeWebView webView, bool dirty);
bool wke_is_dirty(wkeWebView webView);
void wke_add_dirty_area(wkeWebView webView, int x, int y, int w, int h);
void wke_layout_if_needed(wkeWebView webView);
void wke_paint(wkeWebView webView, void* bits, int pitch);

bool wke_can_go_back(wkeWebView webView);
bool wke_go_back(wkeWebView webView);
bool wke_can_go_forward(wkeWebView webView);
bool wke_go_forward(wkeWebView webView);

void wke_select_all(wkeWebView webView);
void wke_copy(wkeWebView webView);
void wke_cut(wkeWebView webView);
void wke_paste(wkeWebView webView);
void wke_delete(wkeWebView webView);

void wke_set_cookie_enabled(wkeWebView webView, bool enable);
bool wke_cookie_enabled(wkeWebView webView);

void wke_set_media_volume(wkeWebView webView, float volume);
float wke_media_volume(wkeWebView webView);

bool wke_mouse_event(wkeWebView webView, unsigned int message, int x, int y, unsigned int flags);
bool wke_context_menu_event(wkeWebView webView, int x, int y, unsigned int flags);
bool wke_mouse_wheel(wkeWebView webView, int x, int y, int delta, unsigned int flags);
bool wke_key_up(wkeWebView webView, unsigned int virtualKeyCode, unsigned int flags, bool systemKey);
bool wke_key_down(wkeWebView webView, unsigned int virtualKeyCode, unsigned int flags, bool systemKey);
bool wke_key_press(wkeWebView webView, unsigned int charCode, unsigned int flags, bool systemKey);

void wke_focus(wkeWebView webView);
void wke_unfocus(wkeWebView webView);

wkeRect wke_get_caret(wkeWebView webView);

jsValue wke_run_js(wkeWebView webView, const char* script);

jsExecState wke_global_exec(wkeWebView webView);

void wke_sleep(wkeWebView webView);
void wke_awaken(wkeWebView webView);
bool wke_is_awake(wkeWebView webView);

void wke_set_zoom_factor(wkeWebView webView, float factor);
float wke_zoom_factor(wkeWebView webView);

void wke_set_editable(wkeWebView webView, bool editable);


/***JavaScript Bind***/
//#define JS_CALL __fastcall
//typedef jsValue (JS_CALL *jsNativeFunction) (jsExecState es);
typedef void *jsNativeFunction;

typedef int jsType;
#define JSTYPE_NUMBER		1
#define JSTYPE_STRING		2
#define JSTYPE_BOOLEAN		3
#define JSTYPE_OBJECT		4
#define JSTYPE_FUNCTION		5
#define JSTYPE_UNDEFINED	6

void jsBindFunction(const char* name, jsNativeFunction fn, unsigned int argCount);
void jsBindGetter(const char* name, jsNativeFunction fn); /*get property*/
void jsBindSetter(const char* name, jsNativeFunction fn); /*set property*/

int jsArgCount(jsExecState es);
jsType jsArgType(jsExecState es, int argIdx);
jsValue jsArg(jsExecState es, int argIdx);

jsType jsTypeOf(jsValue v);
bool jsIsNumber(jsValue v);
bool jsIsString(jsValue v);
bool jsIsBoolean(jsValue v);
bool jsIsObject(jsValue v);
bool jsIsFunction(jsValue v);
bool jsIsUndefined(jsValue v);
bool jsIsNull(jsValue v);
bool jsIsArray(jsValue v);
bool jsIsTrue(jsValue v);
bool jsIsFalse(jsValue v);

int jsToInt(jsExecState es, jsValue v);
float jsToFloat(jsExecState es, jsValue v);
double jsToDouble(jsExecState es, jsValue v);
bool jsToBoolean(jsExecState es, jsValue v);
const char* jsToString(jsExecState es, jsValue v);

jsValue jsInt(int n);
jsValue jsFloat(float f);
jsValue jsDouble(double d);
jsValue jsBoolean(bool b);

jsValue jsUndefined();
jsValue jsNull();
jsValue jsTrue();
jsValue jsFalse();

jsValue jsString(jsExecState es, const char* str);
jsValue jsObject(jsExecState es);
jsValue jsArray(jsExecState es);

jsValue jsFunction(jsExecState es, jsNativeFunction fn, unsigned int argCount);

//return the window object
jsValue jsGlobalObject(jsExecState es);

jsValue jsEval(jsExecState es, const char* str);

jsValue jsCall(jsExecState es, jsValue func, jsValue thisObject, jsValue* args, int argCount);
jsValue jsCallGlobal(jsExecState es, jsValue func, jsValue* args, int argCount);

jsValue jsGet(jsExecState es, jsValue object, const char* prop);
void jsSet(jsExecState es, jsValue object, const char* prop, jsValue v);

jsValue jsGetGlobal(jsExecState es, const char* prop);
void jsSetGlobal(jsExecState es, const char* prop, jsValue v);

jsValue jsGetAt(jsExecState es, jsValue object, int index);
void jsSetAt(jsExecState es, jsValue object, int index, jsValue v);

int jsGetLength(jsExecState es, jsValue object);
void jsSetLength(jsExecState es, jsValue object, int length);

wkeWebView jsGetWebView(jsExecState es);

void jsGC(); //garbage collect


#include "webkit.c"
#endif