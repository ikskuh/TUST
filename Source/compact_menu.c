
#define CM_TAB_STEP            3
#define CM_TAB_RIGHT           3
#define CM_TAB_SRIGHT          14
#define CM_TAB_DIGIT           5
#define CM_TAB_SUB             2
#define CM_TAB_TEXT            6
#define CM_TAB_LINE            0.85
#define CM_HEADER_HEIGHT       8
#define CM_FOOTER_HEIGHT       4

#define CM_ACTIVE              (1<<0)
#define CM_OPENED              (1<<1)
#define CM_INVISIBLE           (1<<2)
#define CM_ARIGHT              (1<<3)
#define CM_RESIZE              (1<<4)
#define CM_POINTER             (1<<5)
#define CM_COMPUTE             (1<<6)

typedef struct CMCLASS
{
	void event ();
	void draw ();
	void resize ();
	void remove ();
	void select ();
} CMCLASS;

typedef struct CMMEMBER
{
	int index;
	var tab;
	var pos_y;
	var size_y;
	STRING *name;
	var flags;
	struct CMMEMBER *parent;
	void *child;
	int count;
	CMCLASS *class;
	struct CMMEMBER *next;
	struct CMMEMBER *prev;
} CMMEMBER;

typedef struct COMPACT_MENU
{
	STRING *name;
	CMSTYLE *style;
	CMMEMBER *cmmember;
	CMMEMBER *cmmemberActual;
	BMAP *select;
	PANEL *panel;
	int digits;
	int strings;
	TEXT *text;
	long lastDraw;
	struct COMPACT_MENU *next;
} COMPACT_MENU;

COMPACT_MENU *cmenuFirst = NULL;
COMPACT_MENU *cmenuMouseLast = NULL;
COMPACT_MENU *cmenuMe = NULL;
CMMEMBER *cmmemberMe = NULL;
CMMEMBER *cmmemberPrev = NULL;
STRING *strCMTemp = "";
STRING *strCMData = "";
STRING *strCMType = "";
STRING *strCMEvent = "";
COLOR colCMText;
COLOR colCMBack;
COLOR colCMOver;
TEXT *txtCMFormats =
{
	string = ( "%.0f", "%.1f", "%.2f", "%.3f" );
}

void fncCMPrototype ( void *object );
void fncCMConstructor ( STRING *strData );

void drwCMSelection ()
{
	if ( cmenuMe->cmmemberActual != NULL )
	{
		cmmemberMe = cmenuMe->cmmemberActual;
		if ( cmmemberMe->flags & CM_ACTIVE )
		{
			vec_set ( &colCMText, &cmenuMe->style->colText );
			vec_set ( &colCMBack, &cmenuMe->style->colBack );
			vec_set ( &colCMOver, &cmenuMe->style->colOver );
			bmap_rendertarget ( cmenuMe->select, 0, 0 );
			cmmemberMe->class->select ();
			bmap_rendertarget ( NULL, 0, 0 );
			pan_setwindow ( cmenuMe->panel, 1, cmmemberMe->tab, cmmemberMe->pos_y, cmenuMe->panel->size_x - cmmemberMe->tab - 1, cmmemberMe->size_y, cmenuMe->select, &cmmemberMe->tab, &0 );
		}
		else
		{
			pan_setwindow ( cmenuMe->panel, 1, 0, 0, 0, 0, cmenuMe->select, &0, &0 );
		}
	}
	else
	{
		pan_setwindow ( cmenuMe->panel, 1, 0, 0, 0, 0, cmenuMe->select, &0, &0 );
	}
}

void fncCMSelect ()
{
	cmmemberMe = cmenuMe->cmmember;
	var nMouseY = mouse_pos.y - cmenuMe->panel->pos_y;
	while ( cmmemberMe )
	{
		if ( nMouseY <= cmmemberMe->pos_y + cmmemberMe->size_y )
			break;
		cmmemberMe = cmmemberMe->next;
	}
	cmenuMe->cmmemberActual = cmmemberMe;
	
	drwCMSelection ();
}




void fncCMUpdate ()
{
	int i = 0;
	PANEL *panel = cmenuMe->panel;
	for ( ; i<cmenuMe->digits; i++ )
		pan_setdigits ( panel, i+1, 0, -100000, "", cmenuMe->style->font, 1, &0 );
	for ( i=0; i<cmenuMe->text->strings; i++ )
		str_cpy ( (cmenuMe->text->pstring)[i], "" );
	cmenuMe->digits = 1;
	cmenuMe->strings = 1;
	cmenuMe->cmmemberActual = NULL;
	cmenuMe->lastDraw = total_ticks;
	
	cmmemberMe = cmenuMe->cmmember;
	vec_set ( &colCMText, cmenuMe->style->colText );
	vec_set ( &colCMBack, cmenuMe->style->colBack );
	vec_set ( &colCMOver, cmenuMe->style->colOver );
	
	bmap_rendertarget ( panel->bmap, 0, 0 );
	draw_line ( nullvector, nullvector, 0 );
	
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, 0, 0 ) );
	draw_line ( &vecPos, colCMText, 0 );
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.x = panel->size_x;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.y += 1;
	draw_line ( &vecPos, colCMBack, 100 );
	vecPos.x = 0;
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.y += 1;
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( panel->size_x, CM_HEADER_HEIGHT-4, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMOver, 100, 0 );
	vecPos.y = CM_HEADER_HEIGHT-1;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.x = panel->size_x;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.y -= 1;
	draw_line ( &vecPos, colCMBack, 100 );
	vecPos.x = 0;
	draw_line ( &vecPos, colCMText, 100 );
	draw_line ( &vecPos, colCMText, 0 );
	
	panel->size_y = cmmemberMe->size_y + CM_HEADER_HEIGHT;
	
	cmmemberMe->prev = NULL;
	cmmemberMe->next = NULL;
	cmmemberPrev = cmmemberMe;
	cmmemberMe->class->draw ();
	
	vec_set ( &vecPos, vector ( 0, panel->size_y, 0 ) );
	draw_line ( &vecPos, colCMText, 0 );
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.x = cmenuMe->panel->size_x;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.y += 1;
	draw_line ( &vecPos, colCMBack, 100 );
	vecPos.x = 0;
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.y += 1;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.x = panel->size_x;
	draw_line ( &vecPos, colCMOver, 100 );
	draw_line ( &vecPos, colCMOver, 0 );
	
	panel->size_y += CM_FOOTER_HEIGHT;
	
	vec_set ( &vecPos, nullvector );
	draw_line ( &vecPos, colCMText, 0 );
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.y = panel->size_y - 1;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.x = panel->size_x - 1;
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.y = 0;
	draw_line ( &vecPos, colCMText, 100 );
	draw_line ( &vecPos, colCMText, 0 );
	
	bmap_rendertarget ( NULL, 0, 0 );
	
	fncCMSelect ();
	
	var nCMPosDiff = screen_size.y - ( panel->pos_y + panel->size_y );
	if ( nCMPosDiff < 0 )
		panel->pos_y += nCMPosDiff;
}

void cmmemberCMStringParse ()
{
	STRING *strData = cmmemberMe->name;
	str_cpy ( strCMData, strData );
	var pointPos = str_stri ( strCMData, "." );
	#ifdef CM_SAFE_MODE
		if ( !pointPos )
		{
			str_cat ( strCMData, "\npoint expected" );
			error ( strCMData );
			sys_exit ( NULL );
		}
	#endif
	str_trunc ( strData, str_len(strData)-pointPos+1 );
	str_clip ( strCMData, pointPos );
	var equalPos = str_stri ( strCMData, "=" );
	#ifdef CM_SAFE_MODE
		if ( !equalPos )
		{
			str_cat ( strCMData, "\nequal sign expected" );
			error ( strCMData );
			sys_exit ( NULL );
		}
	#endif
	str_cpy ( strCMType, strCMData );
	str_trunc ( strCMType, str_len(strCMType)-equalPos+1 );
	str_clip ( strCMData, equalPos );
	
	str_cat ( strCMType, "CMTypeCreate" );
	fncCMConstructor = engine_getscript ( strCMType->chars );
	if ( fncCMConstructor != NULL )
	{
		fncCMConstructor ( strCMData );
	}
	else
	{
		#ifdef CM_SAFE_MODE
			str_trunc ( strCMType, str_len( "CMTypeCreate" ) );
			str_cat ( strCMType, "\unknown compact menu cmmember" );
			error ( strCMType );
			sys_exit ( NULL );
		#endif
		cmmemberMe->flags |= CM_INVISIBLE;
	}
}

void evnCMMove ()
{
	VECTOR oldMousePos, oldPanelPos, vecTemp;
	PANEL *panel = cmenuMe->panel;
	vec_set ( &oldMousePos, &mouse_pos );
	vec_set ( &oldPanelPos, &panel->pos_x );
	while ( mouse_left )
	{
		vec_diff ( &vecTemp, &mouse_pos, &oldMousePos );
		vec_add ( &vecTemp, &oldPanelPos );
		panel->pos_x = clamp ( vecTemp.x, 0, screen_size.x - panel->size_x );
		panel->pos_y = clamp ( vecTemp.y, 0, screen_size.y - panel->size_y );
		wait(1);
	}
}

void fncCMRun ()
{
	var old_mouse_left = 0;
	PANEL *panMouseLast = NULL;
	
	while (1)
	{
		proc_mode = PROC_EARLY;
		wait (1);
		if ( mouse_panel == panMouseLast )
		{
			if ( cmenuMouseLast == NULL )
				continue;
			var nMouseY = mouse_pos.y - cmenuMouseLast->panel->pos_y;
			cmmemberMe = cmenuMouseLast->cmmemberActual;
			if ( cmmemberMe == NULL )
			{
				if ( mouse_left && !old_mouse_left )
				{
					evnCMMove ();
					while ( mouse_left )
						wait(1);
				}
				else if ( ( nMouseY > CM_HEADER_HEIGHT ) && ( cmenuMouseLast->panel->size_y - nMouseY > CM_FOOTER_HEIGHT ) )
				{
					cmenuMe = cmenuMouseLast;
					fncCMSelect ();
				}
			}
			else
			{
				if ( nMouseY <= cmmemberMe->pos_y )
				{
					CMMEMBER *cmmemberTemp = cmmemberMe->prev;
					while ( cmmemberTemp )
					{
						if ( nMouseY >= cmmemberTemp->pos_y )
							break;
						cmmemberTemp = cmmemberTemp->prev;
					}
					cmenuMouseLast->cmmemberActual = cmmemberTemp;
					cmenuMe = cmenuMouseLast;
					drwCMSelection ();
				}
				else if ( nMouseY > cmmemberMe->pos_y + cmmemberMe->size_y )
				{
					CMMEMBER *cmmemberTemp = cmmemberMe->next;
					while ( cmmemberTemp )
					{
						if ( nMouseY <= cmmemberMe->pos_y + cmmemberMe->size_y )
							break;
						cmmemberTemp = cmmemberTemp->next;
					}
					cmenuMouseLast->cmmemberActual = cmmemberTemp;
					cmenuMe = cmenuMouseLast;
					drwCMSelection ();
				}
				else if ( mouse_left && !old_mouse_left && ( cmmemberMe->flags & CM_ACTIVE ) )
				{
					cmmemberMe->class->event ();
					if ( cmenuMouseLast != NULL )
					{
						if ( cmmemberMe->flags & CM_RESIZE )
						{
							cmmemberMe->flags &= ~CM_RESIZE;
							cmenuMe = cmenuMouseLast;
							fncCMUpdate ();
						}
					}
					while ( mouse_left )
						wait(1);
				}
			}
			old_mouse_left = mouse_left;
			continue;
		}
		
		old_mouse_left = mouse_left;
		panMouseLast = mouse_panel;
		if ( panMouseLast == NULL )
		{
			if ( cmenuMouseLast != NULL )
			{
				pan_setwindow ( cmenuMouseLast->panel, 1, 0, 0, 0, 0, cmenuMouseLast->select, &0, &0 );
				cmenuMouseLast = NULL;
			}
			continue;
		}
		
		COMPACT_MENU *cmenuTemp = cmenuFirst;
		while ( cmenuTemp != NULL )
		{
			if ( cmenuTemp->panel == panMouseLast )
				break;
			cmenuTemp = cmenuTemp->next;
		}
		
		if ( cmenuTemp != cmenuMouseLast )
		{
			if ( cmenuMouseLast != NULL )
			{
				pan_setwindow ( cmenuMouseLast->panel, 1, 0, 0, 0, 0, cmenuMouseLast->select, &0, &0 );
			}
			if ( cmenuTemp == NULL )
				continue;
			
			cmenuMouseLast = cmenuTemp;
			cmenuMe = cmenuTemp;
			fncCMSelect ();
//			fncCMUpdate ();
		}
	}
}

/* ---------------------------------------------------------------------------------------------------- */

void cmmember_name ()
{
	if ( cmenuMe->strings == cmenuMe->text->strings )
		txt_addstring ( cmenuMe->text, NULL );
	STRING *strTemp = (cmenuMe->text->pstring)[cmenuMe->strings];
	str_cpy ( strTemp, cmmemberMe->name );
	var posX;
	if ( cmmemberMe->flags & CM_ARIGHT )
		posX = cmenuMe->panel->size_x - CM_TAB_SRIGHT;
	else
		posX = cmmemberMe->tab + str_width(strTemp,cmenuMe->style->font) + CM_TAB_TEXT;
	if ( !pan_setstring ( cmenuMe->panel, cmenuMe->digits, posX, cmmemberMe->pos_y, cmenuMe->style->font, strTemp ) )
		pan_setstring ( cmenuMe->panel, 0, posX, cmmemberMe->pos_y, cmenuMe->style->font, strTemp );
	cmenuMe->strings ++;
	cmenuMe->digits ++;
}

void cmmember_digit ( var *pointer, STRING *format )
{
	var posX = cmenuMe->panel->size_x - CM_TAB_DIGIT;
	if ( !pan_setdigits ( cmenuMe->panel, cmenuMe->digits, posX, cmmemberMe->pos_y, format, cmenuMe->style->font, 1, pointer ) )
	{
		pan_setdigits ( cmenuMe->panel, 0, posX, cmmemberMe->pos_y, format, cmenuMe->style->font, 1, pointer );
	}
	cmenuMe->digits += 1;
}

CMSTYLE *cmstyle_create ( FONT *font, COLOR *colText, COLOR *colBackground, COLOR *colOver )
{
	CMSTYLE *style = sys_malloc ( sizeof(CMSTYLE) );
	style->font = font;
	vec_set ( style->colText, colText );
	vec_set ( style->colBack, colBackground );
	vec_set ( style->colOver, colOver );
	return style;
} 


PANEL *cmenu_create ( char *chrMember, var pos_x, var pos_y, var size_x, var layer, var flags, CMSTYLE *style )
{
	PANEL *panTemp = pan_create ( "", layer );
	panTemp->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
	panTemp->pos_x = pos_x;
	panTemp->pos_y = pos_y;
	panTemp->size_x = size_x;
	panTemp->size_y = 0;
	panTemp->flags = flags | ARIGHT;
	vec_set ( &panTemp->blue, style->colText );
	
	cmenuMe = sys_malloc ( sizeof(COMPACT_MENU) );
	cmenuMe->name = str_create ( chrMember );
	cmenuMe->style = style;
	cmenuMe->panel = panTemp;
	cmenuMe->digits = 0;
	cmenuMe->strings = 0;
	cmenuMe->text = txt_create ( 1, 1 );
	cmenuMe->select = bmap_createblack ( size_x, style->font->dy, 32 );
	cmenuMe->cmmember = sys_malloc ( sizeof(CMMEMBER) );
	cmenuMe->next = cmenuFirst;
	cmenuFirst = cmenuMe;
	
	pan_setwindow ( panTemp, 0, 0, 0, 0, 0, cmenuMe->select, &0, &0 );
	
	cmmemberMe = cmenuMe->cmmember;
	cmmemberMe->index = 0;
	cmmemberMe->parent = NULL;
	cmmemberMe->tab = 0;
	cmmemberMe->pos_y = CM_HEADER_HEIGHT;
	cmmemberMe->name = cmenuMe->name;
	cmmemberCMStringParse ();
	
	fncCMUpdate ();
	
	if ( !proc_status ( fncCMRun ) )
		fncCMRun ();
	
	return panTemp;
}

void cmenu_modify ( PANEL *panel, var size_x, CMSTYLE *style )
{
	COMPACT_MENU *cmenuTemp = cmenuFirst;
	while ( cmenuTemp != NULL )
	{
		if ( cmenuTemp->panel == panel )
		{
			if ( style != NULL )
			{
				cmenuTemp->style = style;
				vec_set ( panel->blue, style->colText );
			}
			if ( ( size_x != 0 ) && ( size_x != panel->size_x ) )
			{
				bmap_remove ( panel->bmap );
				panel->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
				panel->size_x = size_x;
			}
			cmenuMe = cmenuTemp;
			cmmemberMe = cmenuTemp->cmmember;
			if ( cmmemberMe->class->resize != NULL )
				cmmemberMe->class->resize ();
			fncCMUpdate ();
			break;
		}
		cmenuTemp = cmenuTemp->next;
	}
}

void cmenu_remove ( PANEL *panel )
{
	COMPACT_MENU *cmenuTemp = cmenuFirst;
	COMPACT_MENU *cmenuPrev = NULL;
	while ( cmenuTemp != NULL )
	{
		if ( cmenuTemp->panel == panel )
		{
			if ( cmenuTemp == cmenuMouseLast )
				cmenuMouseLast = NULL;
			
			bmap_remove ( panel->bmap );
			bmap_remove ( cmenuTemp->select );
			pan_remove ( panel );
			str_remove ( cmenuTemp->name );
			
			cmenuMe = cmenuTemp;
			cmmemberMe = cmenuTemp->cmmember;
			if ( cmmemberMe->class->remove != NULL )
				cmmemberMe->class->remove ();
			sys_free ( cmenuTemp->cmmember );
			
			if ( cmenuPrev == NULL )
				cmenuFirst = cmenuTemp->next;
			else
				cmenuPrev->next = cmenuTemp->next;
			
			sys_free ( cmenuTemp );
			break;
		}
		cmenuPrev = cmenuTemp;
		cmenuTemp = cmenuTemp->next;
	}
}

void cmenu_remove_all ()
{
	COMPACT_MENU *cmenuTemp = cmenuFirst;
	cmenuFirst = NULL;
	cmenuMouseLast = NULL;
	while ( cmenuTemp != NULL )
	{
		bmap_remove ( cmenuTemp->panel->bmap );
		bmap_remove ( cmenuTemp->select );
		pan_remove ( cmenuTemp->panel );
		str_remove ( cmenuTemp->name );
		
		cmenuMe = cmenuTemp;
		cmmemberMe = cmenuTemp->cmmember;
		if ( cmmemberMe->class->remove != NULL )
			cmmemberMe->class->remove ();
		sys_free ( cmenuTemp->cmmember );
		
		COMPACT_MENU *cmenu = cmenuTemp;
		cmenuTemp = cmenuTemp->next;
		
		sys_free ( cmenu );
	}
}