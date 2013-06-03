
#define PRAGMA_PATH "compact_menu"

#define CM_TAB_STEP            3
#define CM_TAB_RIGHT           3
#define CM_TAB_SRIGHT          14
#define CM_TAB_DIGIT           5
#define CM_TAB_SUB             2
#define CM_TAB_TEXT            4
#define CM_TAB_LINE            0.85

#define CM_ACTIVE              (1<<0)
#define CM_SUBMENU             (1<<1)
#define CM_OPENED              (1<<2)
#define CM_INVISIBLE           (1<<3)
#define CM_FREE                (1<<4)

COLOR *colCMText 	= { blue=20; 	green=20; 	red=20; }
COLOR *colCMBack 	= { blue=250; 	green=250; 	red=250; }
COLOR *colCMOver 	= { blue=210; 	green=210; 	red=210; }

// GENERAL
/* ---------------------------------------------------------------------------------------------------- */
typedef struct CMMEMBER
{
	int index;
	var tab;
	var pos_y;
	var size_y;
	STRING *name;
	TEXT *text;
	var flags;
	void *cmenu;
	struct CMMEMBER *parent;
	void *child;
	int count;
	void event ();
	void draw ();
	void resize ();
} CMMEMBER;

typedef struct COMPACT_MENU
{
	STRING *name;
	COLOR *colText;
	COLOR *colBack;
	COLOR *colOver;
	CMMEMBER member;
	CMMEMBER *memberActual;
	FONT *font;
	PANEL *panel;
	int digits;
	int strings;
	TEXT *text;
	struct COMPACT_MENU *next;
} COMPACT_MENU;

COMPACT_MENU *cmenuFirst = NULL;
COMPACT_MENU *cmenuMouseLast = NULL;
COMPACT_MENU *cmenuMe = NULL;
CMMEMBER *memberMe = NULL;
STRING *strCMTemp = "";
STRING *strCMData = "";
STRING *strCMType = "";
STRING *strCMEvent = "";


// PROTOTYPES
/* ---------------------------------------------------------------------------------------------------- */

void fncCMPrototype ( void *object );
void fncCMConstructor ( STRING *strData );
//CMMEMBER *memberCMTextParse ( TEXT *txtMembers );


// GLOBAL FUNCTIONS
/* ---------------------------------------------------------------------------------------------------- */

void drwCMMembers ()
{
	VECTOR vecPos;
	var nMouseY = mouse_pos.y - cmenuMe->panel->pos_y;
	CMMEMBER **memberTemp = memberMe->child;
	CMMEMBER **memberTempLast = memberTemp + memberMe->count;
	for ( ; memberTemp<memberTempLast; memberTemp ++ )
	{
		(*memberTemp)->pos_y = cmenuMe->panel->size_y;
		if ( (*memberTemp)->flags & CM_INVISIBLE )
			continue;
		
		if ( mouse_panel == cmenuMe->panel )
		{
			if ( cmenuMe->memberActual == NULL )
			{
				if ( nMouseY <= cmenuMe->panel->size_y + (*memberTemp)->size_y )
					cmenuMe->memberActual = *memberTemp;
			}
		}
		
		memberMe = *memberTemp;
		(*memberTemp)->draw ();
		cmenuMe->panel->size_y += (*memberTemp)->size_y;
		
		if ( (*memberTemp)->flags & CM_OPENED )
		{
			var old_position = cmenuMe->panel->size_y;
			drwCMMembers ();
			vec_set ( &vecPos, vector ( (*memberTemp)->tab-1, old_position, 0 ) );
			draw_line ( &vecPos, cmenuMe->colText, 0 );
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.y = cmenuMe->panel->size_y;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			draw_line ( &vecPos, cmenuMe->colText, 0 );
		}
	}
}

void drwCMComplete ( COMPACT_MENU *cmenu )
{
	int i = 0;
	for ( ; i<cmenu->digits; i++ )
		pan_setdigits ( cmenu->panel, i+1, 0, -100000, "", cmenu->font, 1, &0 );
	for ( i=0; i<cmenu->text->strings; i++ )
		str_cpy ( (cmenu->text->pstring)[i], "" );
	cmenu->digits = 1;
	cmenu->strings = 1;
	cmenu->memberActual = NULL;
	cmenu->panel->size_y = 0;
	
	bmap_rendertarget ( cmenu->panel->bmap, 0, 0 );
	draw_line ( nullvector, nullvector, 0 );
	
	cmenuMe = cmenu;
	memberMe = &cmenu->member;
	drwCMMembers ();
	
	draw_line ( nullvector, cmenu->colOver, 0 );
	draw_line ( nullvector, cmenu->colOver, 100 );
	draw_line ( vector(0,cmenu->panel->size_y,0), cmenu->colOver, 100 );
	draw_line ( vector(0,cmenu->panel->size_y,0), cmenu->colOver, 0 );
	draw_line ( vector(cmenu->panel->size_x-1,cmenu->panel->size_y,0), cmenu->colOver, 0 );
	draw_line ( vector(cmenu->panel->size_x-1,cmenu->panel->size_y,0), cmenu->colOver, 100 );
	draw_line ( vector(cmenu->panel->size_x-1,0,0), cmenu->colOver, 100 );
	draw_line ( vector(cmenu->panel->size_x-1,0,0), cmenu->colOver, 0 );
	
//	if ( cmenu != cmenuFirst )
//		draw_quad ( NULL, nullvector, NULL, vector(cmenu->panel->size_x,cmenu->panel->size_x,0), NULL, cmenu->colOver, 0, 0 );
	
	bmap_rendertarget ( NULL, 0, 0 );
}

void fncCMMembersResize ()
{
	CMMEMBER **memberTemp = memberMe->child;
	CMMEMBER **memberTempLast = memberTemp + memberMe->count;
	for ( ; memberTemp<memberTempLast; memberTemp++ )
	{
		memberMe = *memberTemp;
		if ( (*memberTemp)->resize != NULL )
			memberMe->resize ();
		if ( (*memberTemp)->flags & CM_SUBMENU )
			fncCMMembersResize ();
	}
}

void fncCMMembersRemove ()
{
	bmap_remove ( memberMe->text->target_map );
	txt_remove ( memberMe->text );
	CMMEMBER **memberTemp = memberMe->child;
	CMMEMBER **memberTempLast = memberTemp + memberMe->count;
	for ( ; memberTemp<memberTempLast; memberTemp++ )
	{
		memberMe = *memberTemp;
		if ( (*memberTemp)->flags & CM_SUBMENU )
			fncCMMembersRemove ();
		if ( (*memberTemp)->flags & CM_FREE )
			sys_free ( (*memberTemp)->child );
	}
}

void cmmember_draw_name ()
{
	if ( cmenuMe->strings == cmenuMe->text->strings )
		txt_addstring ( cmenuMe->text, NULL );
	STRING *strTemp = (cmenuMe->text->pstring)[cmenuMe->strings];
	str_cpy ( strTemp, memberMe->name );
	var posX = memberMe->tab + str_width(strTemp,cmenuMe->font) + CM_TAB_TEXT;
	if ( !pan_setstring ( cmenuMe->panel, cmenuMe->digits, posX, memberMe->pos_y, cmenuMe->font, strTemp ) )
		pan_setstring ( cmenuMe->panel, 0, posX, memberMe->pos_y, cmenuMe->font, strTemp );
	cmenuMe->strings ++;
	cmenuMe->digits ++;
}

// SUBMENU
/* ---------------------------------------------------------------------------------------------------- */

void fncCMSubmenuResize ()
{
	memberMe->size_y = cmenuMe->font->dy;
}

void evnCMSubmenu ()
{
	memberMe->flags ^= CM_OPENED;
	if ( memberMe->flags & CM_OPENED )
	{
		CMMEMBER **memberTemp = memberMe->child;
		CMMEMBER **memberTempLast = memberTemp + memberMe->count;
		for ( ; memberTemp<memberTempLast; memberTemp++ )
			(*memberTemp)->flags &= ~CM_OPENED;
		memberTemp = memberMe->parent->child;
		memberTempLast = memberTemp + memberMe->parent->count;
		for ( ; memberTemp<memberTempLast; memberTemp++ )
		{
			if ( *memberTemp == memberMe )
				continue;
			(*memberTemp)->flags &= ~CM_OPENED;
		}
	}
	drwCMComplete ( cmenuMe );
}

void drwCMSubmenu ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );
	VECTOR vecOffset;
	if ( memberMe->flags & CM_OPENED )
	{
		vec_set ( &vecPos, vector ( memberMe->tab-1, memberMe->pos_y+memberMe->size_y-3, 0 ) );
		vec_set ( &vecSize, vector(cmenuMe->panel->size_x-vecPos.x,3,0) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colOver, 100, 0 );
		if ( memberMe == cmenuMe->memberActual )
		{
			vec_set ( &vecPos, vector ( cmenuMe->panel->size_x+1-CM_TAB_SRIGHT, memberMe->pos_y+memberMe->size_y-4, 0 ) );
			draw_line ( &vecPos, cmenuMe->colText, 0 );
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.x += 3;
			vecPos.y -= 7;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.x += 3;
			vecPos.y += 7;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.x -= 6;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			draw_line ( &vecPos, cmenuMe->colText, 0 );
		}
	}
	else
	{
		if ( memberMe == cmenuMe->memberActual )
		{
			vec_set ( &vecPos, vector ( memberMe->tab-1, memberMe->pos_y+memberMe->size_y-3, 0 ) );
			vec_set ( &vecSize, vector(cmenuMe->panel->size_x-vecPos.x,3,0) );
			draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colOver, 100, 0 );
			vec_set ( &vecPos, vector ( cmenuMe->panel->size_x+1-CM_TAB_SRIGHT, memberMe->pos_y+memberMe->size_y-11, 0 ) );
			draw_line ( &vecPos, cmenuMe->colText, 0 );
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.x += 3;
			vecPos.y += 7;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.x += 3;
			vecPos.y -= 7;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			vecPos.x -= 6;
			draw_line ( &vecPos, cmenuMe->colText, 100 );
			draw_line ( &vecPos, cmenuMe->colText, 0 );
		}
		vec_set ( &vecPos, vector ( memberMe->tab, memberMe->pos_y+(memberMe->size_y/2)-2, 0 ) );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		vecPos.y += 5;
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
		vec_set ( &vecPos, vector ( memberMe->tab-1, memberMe->pos_y+(memberMe->size_y/2), 0 ) );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		vecPos.x += 3;
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
	}
	
	cmmember_draw_name ();
}

CMMEMBER *memberCMTextParse ( TEXT *txtMembers )
{
	int iMembersCount = txtMembers->strings;
	TEXT *txtTemp = txt_create ( iMembersCount, 1 );
	int i = 0;
	for ( ; i<iMembersCount; i++ )
	{
		str_cpy ( (txtTemp->pstring)[i], (txtMembers->pstring)[i] ); 
	}
	
	CMMEMBER *parent = memberMe;
	CMMEMBER **members = sys_malloc ( sizeof(CMMEMBER*) * iMembersCount );
	int i = 0;
	for ( ; i<iMembersCount; i++ )
	{
		str_cpy ( strCMData, (txtTemp->pstring)[i] );
		var pointPos = str_stri ( strCMData, "." );
		#ifdef CM_SAFE_MODE
			if ( !pointPos )
			{
				str_cat ( strCMData, "\npoint expected" );
				error ( strCMData );
				sys_exit ( NULL );
			}
		#endif
		str_trunc ( (txtTemp->pstring)[i], str_len((txtTemp->pstring)[i])-pointPos+1 );
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
		
		CMMEMBER *memberTemp = sys_malloc ( sizeof(CMMEMBER) );
		*(members+i) = memberTemp;
		memberTemp->name = (txtTemp->pstring)[i];
		memberTemp->index = i;
		memberTemp->tab = parent->tab + CM_TAB_STEP;
		memberTemp->text = txtTemp;
		memberTemp->parent = parent;
		
		str_cat ( strCMType, "CMTypeCreate" );
		fncCMConstructor = engine_getscript ( strCMType->chars );
		if ( fncCMConstructor != NULL )
		{
			memberMe = memberTemp;
			fncCMConstructor ( strCMData );
		}
		else
		{
			#ifdef CM_SAFE_MODE
				str_trunc ( strCMType, str_len( "CMTypeCreate" ) );
				str_cat ( strCMType, "\unknown compact menu member" );
				error ( strCMType );
				sys_exit ( NULL );
			#endif
			memberTemp->flags |= CM_INVISIBLE;
		}
	}
	
	return members;
}

void submenuCMTypeCreate ( STRING *strData )
{
	TEXT *txtSub = var_for_name ( strData );
	#ifdef CM_SAFE_MODE
		if ( txtSub == NULL )
		{
			str_cat ( strData, "\nTEXT struct not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	memberMe->flags = CM_ACTIVE | CM_SUBMENU | CM_FREE;
	memberMe->event = evnCMSubmenu;
	memberMe->draw = drwCMSubmenu;
	memberMe->resize = fncCMSubmenuResize;
	memberMe->count = txtSub->strings;
	fncCMSubmenuResize ();
	CMMEMBER *memberTemp = memberMe;
	memberTemp->child = memberCMTextParse ( txtSub );
}

// SLIDER
/* ---------------------------------------------------------------------------------------------------- */

typedef struct CMSLIDER
{
	var size_x;
	var slide_x;
	var min;
	var range;
	var step;
	STRING *format;
	var *value;
	void event ();
} CMSLIDER;

void fncCMSliderResize ()
{
	CMSLIDER *slider = memberMe->child;
	slider->size_x = ( cmenuMe->panel->size_x - memberMe->tab - CM_TAB_RIGHT ) - 1;
	slider->slide_x = *slider->value;
	slider->slide_x -= slider->min;
	slider->slide_x /= slider->range;
	slider->slide_x *= slider->size_x;
	memberMe->size_y = cmenuMe->font->dy;
}

void drwCMSlider ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );
	CMSLIDER *slider = memberMe->child;
	if ( memberMe == cmenuMe->memberActual )
	{
		vec_set ( &vecPos, vector ( memberMe->tab, memberMe->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( slider->slide_x, memberMe->size_y-2, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colOver, 100, 0 );
		vecPos.x += slider->size_x;
		vecPos.y += memberMe->size_y - 2;
		draw_line ( &vecPos, cmenuMe->colOver, 0 );
		draw_line ( &vecPos, cmenuMe->colOver, 100 );
		vecPos.x -= slider->size_x;
		draw_line ( &vecPos, cmenuMe->colOver, 100 );
		draw_line ( &vecPos, cmenuMe->colOver, 0 );
		
		vec_set ( &vecPos, vector ( memberMe->tab+slider->slide_x, memberMe->pos_y+1, 0 ) );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		vecPos.y += memberMe->size_y-2;
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( memberMe->tab+slider->size_x, memberMe->pos_y+memberMe->size_y-1, 0 ) );
		draw_line ( &vecPos, cmenuMe->colOver, 0 );
		draw_line ( &vecPos, cmenuMe->colOver, 100 );
		vecPos.x -= slider->size_x * CM_TAB_LINE;
		draw_line ( &vecPos, cmenuMe->colBack, 100 );
		draw_line ( &vecPos, cmenuMe->colBack, 0 );
	}
	
	if ( !pan_setdigits ( cmenuMe->panel, cmenuMe->digits, cmenuMe->panel->size_x - CM_TAB_DIGIT, memberMe->pos_y, slider->format, cmenuMe->font, 1, slider->value ) )
	{
		pan_setdigits ( cmenuMe->panel, 0, cmenuMe->panel->size_x - CM_TAB_DIGIT, memberMe->pos_y, slider->format, cmenuMe->font, 1, slider->value );
	}
	cmenuMe->digits += 1;
	cmmember_draw_name ();
}

void drwCMSliderUpdate ()
{
	CMSLIDER *slider = memberMe->child;
	VECTOR vecPos;
	vec_set ( &vecPos, vector( memberMe->tab, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x-memberMe->tab-1, memberMe->size_y, 0 ) );
	bmap_rendertarget ( cmenuMe->panel->bmap, 0, 1 );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );
	vec_set ( &vecPos, vector ( memberMe->tab, memberMe->pos_y+1, 0 ) );
	vec_set ( &vecSize, vector ( maxv(slider->slide_x,1), memberMe->size_y-2, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colOver, 100, 0 );
	vecPos.x += slider->size_x;
	vecPos.y += memberMe->size_y - 2;
	draw_line ( &vecPos, cmenuMe->colOver, 0 );
	draw_line ( &vecPos, cmenuMe->colOver, 100 );
	vecPos.x -= slider->size_x;
	draw_line ( &vecPos, cmenuMe->colOver, 100 );
	draw_line ( &vecPos, cmenuMe->colOver, 0 );
	
	vec_set ( &vecPos, vector ( memberMe->tab+slider->slide_x, memberMe->pos_y+1, 0 ) );
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	vecPos.y += memberMe->size_y-2;
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	
	bmap_rendertarget ( NULL, 0, 0 );
}

void evnCMSlider ()
{
	CMMEMBER *member = memberMe;
	CMSLIDER *slider = memberMe->child;
	var old_value = 0;
	while ( mouse_left )
	{
		old_value = *slider->value;
		slider->slide_x = clamp ( mouse_pos.x - ( cmenuMe->panel->pos_x + member->tab ), 0, slider->size_x );
		*slider->value = slider->slide_x;
		*slider->value /= slider->size_x;
		*slider->value *= slider->range;
		if ( slider->step != 0 )
			*slider->value = integer ( *slider->value / slider->step ) * slider->step;
		*slider->value += slider->min;
		if ( old_value != *slider->value )
		{
			memberMe = member;
			drwCMSliderUpdate ();
		}
		if ( slider->event != NULL )
			slider->event ();
		wait(1);
	}
}

void sliderCMTypeCreate ( STRING *strData )
{
	var nMin = str_to_num ( strData );
	var commaPos = str_stri ( strData, "," );
	#ifdef CM_SAFE_MODE
		if ( !commaPos )
		{
			str_cat ( strData, "\ncomma expected" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	str_clip ( strData, commaPos );
	var nMax = str_to_num ( strData );
	commaPos = str_stri ( strData, "," );
	#ifdef CM_SAFE_MODE
		if ( !commaPos )
		{
			str_cat ( strData, "\ncomma expected" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	str_clip ( strData, commaPos );
	var nStep = str_to_num ( strData );
	commaPos = str_stri ( strData, "," );
	#ifdef CM_SAFE_MODE
		if ( !commaPos )
		{
			str_cat ( strData, "\ncomma expected" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	str_clip ( strData, commaPos );
	str_cpy ( strCMType, strData );
	commaPos = str_stri ( strData, "," );
	#ifdef CM_SAFE_MODE
		if ( !commaPos )
		{
			str_cat ( strData, "\ncomma expected" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	str_clip ( strData, commaPos );
	str_trunc ( strCMType, str_len(strCMType)-commaPos+1 );
	
	str_cpy ( strCMEvent, strData );
	commaPos = str_stri ( strData, "," );
	fncCMPrototype = NULL;
	if ( commaPos )
	{
		str_clip ( strCMEvent, commaPos );
		str_trunc ( strData, str_len(strData)-commaPos+1 );
		
		fncCMPrototype = engine_getscript ( strCMEvent->chars );
		#ifdef CM_SAFE_MODE
			if ( fncCMPrototype == NULL )
			{
				str_cat ( strCMEvent, "\nfunction not found" );
				error ( strCMEvent );
				sys_exit ( NULL );
			}
		#endif
	}
	
	var *pointer = engine_getvar ( strData->chars, NULL );
	#ifdef CM_SAFE_MODE
		if ( pointer == NULL )
		{
			str_cat ( strData, "\nvariable not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	
	*pointer = clamp ( *pointer, nMin, nMax );
	CMSLIDER *slider = sys_malloc ( sizeof(CMSLIDER) );
	slider->min = nMin;
	slider->range = nMax - nMin;
	slider->step = nStep;
	slider->value = pointer;
	slider->format = str_create ( strCMType );
	slider->event = fncCMPrototype;
//	slider->digit = pan_setdigits ( cmenuMe->panel, 0, 0, 0, "", cmenuMe->font, 1, pointer );
//	cmenuMe->digits ++;
	
	memberMe->flags = CM_ACTIVE | CM_FREE;
	memberMe->event = evnCMSlider;
	memberMe->resize = fncCMSliderResize;
	memberMe->draw = drwCMSlider;
	memberMe->count = 0;
	memberMe->child = slider;
	fncCMSliderResize ();
}


// BITMAP
/* ---------------------------------------------------------------------------------------------------- */

void fncCMBitmapResize ()
{
	BMAP **bmpTemp = memberMe->child;
	if ( *bmpTemp != NULL )
		memberMe->size_y = ( bmap_height ( *bmpTemp ) * ( cmenuMe->panel->size_x - memberMe->tab - CM_TAB_RIGHT ) ) / bmap_width ( *bmpTemp );
	else
		memberMe->size_y = cmenuMe->font->dy;
}

void drwCMBitmap ()
{
	fncCMBitmapResize ();
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );
	BMAP **bmpTemp = memberMe->child;
	if ( *bmpTemp != NULL )
	{
		vecPos.x = memberMe->tab;
		var nProportion = ( cmenuMe->panel->size_x - vecPos.x - CM_TAB_RIGHT ) / bmap_width ( *bmpTemp );
		VECTOR vecScale;
		vec_set ( &vecScale, vector(nProportion,nProportion,0) );
		draw_quad ( *bmpTemp, &vecPos, NULL, vector(bmap_width(*bmpTemp),bmap_height(*bmpTemp),0), &vecScale, NULL, 100, 0 );
	}
	else
	{
		cmmember_draw_name ();
//		VECTOR vecOffset;
//		vec_set ( &vecOffset, vector ( 0, memberMe->index * cmenuMe->font->dy, 0 ) );
//		draw_quad ( memberMe->text->target_map, &vecPos, &vecOffset, vector(vecSize.x-CM_TAB_RIGHT,cmenuMe->font->dy,0), NULL, NULL, 100, 0 );
	}
}


void bmapCMTypeCreate ( STRING *strData )
{
	BMAP **bmapPtr = engine_getvar ( strData->chars, NULL );
	#ifdef CM_SAFE_MODE
		if ( bmapPtr == NULL )
		{
			str_cat ( strData, "\npointer to bitmap pointer not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	memberMe->flags = 0;
	memberMe->event = NULL;
	memberMe->draw = drwCMBitmap;
	memberMe->resize = fncCMBitmapResize;
	memberMe->count = 0;
	memberMe->child = bmapPtr;
	str_cpy ( (memberMe->text->pstring)[memberMe->index], "NULL" );
	
	fncCMBitmapResize ();
}


// FUNCTION
/* ---------------------------------------------------------------------------------------------------- */

void fncCMButtonResize ()
{
	memberMe->size_y = cmenuMe->font->dy;
}

void drwCMButton ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );
	if ( memberMe == cmenuMe->memberActual )
	{
		vec_set ( &vecPos, vector ( memberMe->tab-1, memberMe->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( cmenuMe->panel->size_x+1-memberMe->tab, memberMe->size_y-1, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colOver, 100, 0 );		
		vec_set ( &vecPos, vector ( cmenuMe->panel->size_x-3, memberMe->pos_y+(memberMe->size_y/2), 0 ) );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		vecPos.x -= 8;
		vecPos.y -= 3;
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		vecPos.y += 6;
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		vecPos.x += 8;
		vecPos.y -= 3;
		draw_line ( &vecPos, cmenuMe->colText, 100 );
		draw_line ( &vecPos, cmenuMe->colText, 0 );
	}
	cmmember_draw_name ();
//	VECTOR vecOffset;
//	vec_set ( &vecOffset, vector ( 0, memberMe->index * cmenuMe->font->dy, 0 ) );
//	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
//	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x-CM_TAB_SRIGHT, cmenuMe->font->dy, 0 ) );
//	draw_quad ( memberMe->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void buttonCMTypeCreate ( STRING *strData )
{
	fncCMPrototype = engine_getscript ( strData->chars );
	
	#ifdef CM_SAFE_MODE
		if ( fncCMPrototype == NULL )
		{
			str_cat ( strData, "\nfunction not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	memberMe->flags = CM_ACTIVE;
	memberMe->event = fncCMPrototype;
	memberMe->draw = drwCMButton;
	memberMe->resize = fncCMButtonResize;
	memberMe->count = 0;
	memberMe->child = NULL;
	
	fncCMButtonResize ();
}


// TITLE
/* ---------------------------------------------------------------------------------------------------- */

void fncCMTitleResize ()
{
	memberMe->size_y = memberMe->count + 2;
	if ( str_len ( (memberMe->text->pstring)[memberMe->index] ) > 0 )
		memberMe->size_y += cmenuMe->font->dy;
}

void evnCMTitle ()
{
	VECTOR oldMousePos, oldPanelPos, vecTemp;
	PANEL *panel = cmenuMe->panel;
	vec_set ( &oldMousePos, &mouse_pos );
	vec_set ( &oldPanelPos, &panel->pos_x );
	while ( mouse_left )
	{
		vec_diff ( &vecTemp, &mouse_pos, &oldMousePos );
		vec_add ( &vecTemp, &oldPanelPos );
		panel->pos_x = vecTemp.x;
		panel->pos_y = vecTemp.y;
		wait(1);
	}
}

void drwCMTitle ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colOver, 100, 0 );
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	vecPos.x = cmenuMe->panel->size_x;
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	vecPos.y += 1;
	draw_line ( &vecPos, cmenuMe->colBack, 100 );
	vecPos.x = -1;
	draw_line ( &vecPos, cmenuMe->colOver, 100 );
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y+memberMe->size_y-1, 0 ) );
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	vecPos.x = cmenuMe->panel->size_x;
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	vecPos.y -= 1;
	draw_line ( &vecPos, cmenuMe->colBack, 100 );
	vecPos.x = -1;
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	
	cmmember_draw_name ();
//	VECTOR vecOffset;
//	vec_set ( &vecOffset, vector ( 0, memberMe->index * cmenuMe->font->dy, 0 ) );
//	vec_set ( &vecPos, vector ( 0, memberMe->pos_y + memberMe->count, 0 ) );
//	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
//	draw_quad ( memberMe->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void titleCMTypeCreate( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	memberMe->flags = CM_ACTIVE;
	memberMe->event = evnCMTitle;
	memberMe->draw = drwCMTitle;
	memberMe->resize = fncCMTitleResize;
	memberMe->count = size_y;
	memberMe->child = NULL;
	
	fncCMTitleResize ();
}


// LINE
/* ---------------------------------------------------------------------------------------------------- */

void drwCMLine ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );
	vecPos.x += memberMe->tab - CM_TAB_STEP;
	vecPos.y += memberMe->count;
	draw_line ( &vecPos, cmenuMe->colText, 0 );
	draw_line ( &vecPos, cmenuMe->colText, 100 );
	vecPos.x = cmenuMe->panel->size_x * CM_TAB_LINE;
	draw_line ( &vecPos, cmenuMe->colBack, 100 );
	draw_line ( &vecPos, cmenuMe->colBack, 0 );
}

void lineCMTypeCreate ( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	memberMe->flags = 0;
	memberMe->event = NULL;
	memberMe->draw = drwCMLine;
	memberMe->count = size_y;
	memberMe->resize = NULL;
	memberMe->size_y = size_y+1;
	memberMe->child = NULL;
}



// SEPARATOR
/* ---------------------------------------------------------------------------------------------------- */

void drwCMSeparator ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, memberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, memberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->colBack, 100, 0 );	
	vecPos.x += memberMe->tab - CM_TAB_STEP;
	vecPos.y += memberMe->count;
	draw_line ( &vecPos, cmenuMe->colOver, 0 );
	draw_line ( &vecPos, cmenuMe->colOver, 100 );
	vecPos.x = cmenuMe->panel->size_x * CM_TAB_LINE;
	draw_line ( &vecPos, cmenuMe->colBack, 100 );
	draw_line ( &vecPos, cmenuMe->colBack, 0 );
}

void spaceCMTypeCreate ( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	memberMe->flags = 0;
	memberMe->event = NULL;
	memberMe->draw = drwCMSeparator;
	memberMe->count = 1;
	memberMe->resize = NULL;
	memberMe->size_y = size_y+1;
	memberMe->child = NULL;
}


/* ---------------------------------------------------------------------------------------------------- */

void fncCMRun ()
{
	var old_mouse_left = 0;
	PANEL *panMouseLast = NULL;
	
	while (1)
	{
		wait (1);
		if ( mouse_panel == panMouseLast )
		{
			if ( cmenuMouseLast == NULL )
				continue;
			var nMouseY = mouse_pos.y - cmenuMouseLast->panel->pos_y;
			CMMEMBER *memberTemp = cmenuMouseLast->memberActual;
			if ( ( nMouseY < memberTemp->pos_y - 2 ) || ( nMouseY > memberTemp->pos_y + memberTemp->size_y + 2 ) )
			{
				drwCMComplete ( cmenuMouseLast );
			}
			else if ( mouse_left && !old_mouse_left && ( memberTemp->flags & CM_ACTIVE ) )
			{
				memberMe = memberTemp;
				memberTemp->event ();
				if ( cmenuMouseLast != NULL )
					drwCMComplete ( cmenuMouseLast );
				while ( mouse_left )
					wait(1);
			}
			old_mouse_left = mouse_left;
			continue;
		}
		
		old_mouse_left = mouse_left;
		panMouseLast = mouse_panel;
		if ( cmenuMouseLast != NULL )
			drwCMComplete ( cmenuMouseLast );
		cmenuMouseLast = NULL;
		if ( panMouseLast == NULL )
			continue;
		
		COMPACT_MENU *cmenuTemp = cmenuFirst;
		while ( cmenuTemp != NULL )
		{
			if ( cmenuTemp->panel == panMouseLast )
				break;
			cmenuTemp = cmenuTemp->next;
		}
		
		if ( cmenuTemp == NULL )
			continue;
		
		drwCMComplete ( cmenuTemp );
		cmenuMouseLast = cmenuTemp;
	}
}


PANEL *cmenu_create ( char *chrName, var pos_x, var pos_y, var size_x, var layer, var flags, char *chrMembers, FONT *font )
{
	PANEL *panBG = pan_create ( "", layer );
	panBG->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
	panBG->pos_x = pos_x;
	panBG->pos_y = pos_y;
	panBG->size_x = size_x;
	panBG->size_y = 0;
	panBG->flags = flags | ARIGHT;
	vec_set ( &panBG->blue, colCMText );
	
	cmenuMe = sys_malloc ( sizeof(COMPACT_MENU) );
	cmenuMe->name = str_create ( chrName );
	cmenuMe->font = font;
	cmenuMe->panel = panBG;
	cmenuMe->digits = 0;
	cmenuMe->strings = 0;
	cmenuMe->text = txt_create ( 1, 1 );
	txt_addstring ( cmenuMe->text, chrMembers );
	cmenuMe->colBack = colCMBack;
	cmenuMe->colOver = colCMOver;
	cmenuMe->colText = colCMText;
	
	cmenuMe->member.index = 0;
	cmenuMe->member.text = cmenuMe->text;
	cmenuMe->member.flags = CM_OPENED;
	cmenuMe->member.parent = NULL;
	cmenuMe->member.tab = 0;
	cmenuMe->member.event = NULL;
	cmenuMe->member.draw = NULL;
	cmenuMe->member.resize = NULL;
	cmenuMe->member.name = cmenuMe->name;
	memberMe = &cmenuMe->member;
	submenuCMTypeCreate ( (cmenuMe->text->pstring)[0] );
	
	cmenuMe->next = cmenuFirst;
	cmenuFirst = cmenuMe;
	
	drwCMComplete ( cmenuMe );
	
	if ( !proc_status ( fncCMRun ) )
		fncCMRun ();
	
	return panBG;
}

void cmenu_resize ( PANEL *panel, var size_x )
{
	COMPACT_MENU *cmenuTemp = cmenuFirst;
	while ( cmenuTemp != NULL )
	{
		if ( cmenuTemp->panel == panel )
		{
			bmap_remove ( panel->bmap );
			panel->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
			panel->size_x = size_x;
			cmenuMe = cmenuTemp;
			memberMe = &cmenuTemp->member;
			fncCMMembersResize ();
			drwCMComplete ( cmenuTemp );
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
			pan_remove ( panel );
			
			str_remove ( cmenuTemp->name );
			
			cmenuMe = cmenuTemp;
			memberMe = &cmenuTemp->member;
			fncCMMembersRemove ();
			
			if ( cmenuPrev == NULL )
				cmenuFirst = cmenuTemp->next;
			else
				cmenuPrev->next = cmenuTemp->next;
			
			sys_free ( (CMMEMBER*)cmenuTemp->member.child );
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
		pan_remove ( cmenuTemp->panel );
		
		cmenuMe = cmenuTemp;
		memberMe = &cmenuTemp->member;
		fncCMMembersRemove ();
		
		COMPACT_MENU *cmenu = cmenuTemp;
		cmenuTemp = cmenuTemp->next;
		
		sys_free ( (CMMEMBER*)cmenu->member.child );
		sys_free ( cmenu );
	}
}