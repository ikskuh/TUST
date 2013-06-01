
#define CM_TAB_STEP					3
#define CM_TAB_RIGHT					3
#define CM_TAB_SRIGHT				14
#define CM_TAB_DIGIT					5
#define CM_TAB_SUB					2
#define CM_TAB_TEXT					6
#define CM_TAB_LINE					0.85

#define CM_ACTIVE						(1<<0)
#define CM_SUBMENU					(1<<1)
#define CM_OPENED						(1<<2)
#define CM_INVISIBLE					(1<<3)
#define CM_FREE						(1<<4)

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
	TEXT *text;
	var flags;
	void *cmenu;
	struct CMMEMBER *parent;
	void *child;
	int count;
	void event ( struct CMMEMBER *member );
	void draw ( struct CMMEMBER *member );
	void resize ( struct CMMEMBER *member );
} CMMEMBER;

typedef struct COMPACT_MENU
{
	COLOR *colText;
	COLOR *colBack;
	COLOR *colOver;
	CMMEMBER member;
	CMMEMBER *memberActual;
	FONT *font;
	PANEL *panel;
	int digits;
//	var digitValue;
//	PANEL *digitPanel;
	struct COMPACT_MENU *next;
} COMPACT_MENU;

COMPACT_MENU *cmenuFirst = NULL;
COMPACT_MENU *cmenuMouseLast = NULL;
STRING *strCMTemp = "";
STRING *strCMData = "";
STRING *strCMType = "";
STRING *strCMEvent = "";


// PROTOTYPES
/* ---------------------------------------------------------------------------------------------------- */

void fncCMPrototype ( void *object );
CMMEMBER *memberCMTextParse ( COMPACT_MENU *cmenu, CMMEMBER *parent, TEXT *txtMembers );
void evnCMSubmenu ( CMMEMBER *member );


// GLOBAL FUNCTIONS
/* ---------------------------------------------------------------------------------------------------- */

void drwCMMembers ( CMMEMBER *member, int count )
{
	VECTOR vecPos;
	COMPACT_MENU *cmenu = member->cmenu;
	var nMouseY = mouse_pos.y - cmenu->panel->pos_y;
	CMMEMBER *memberTemp = member;
	CMMEMBER *memberTempLast = memberTemp + count;
	for ( ; memberTemp<memberTempLast; memberTemp ++ )
	{
		memberTemp->pos_y = cmenu->panel->size_y;
		if ( memberTemp->flags & CM_INVISIBLE )
			continue;
		
		if ( mouse_panel == cmenu->panel )
		{
			if ( cmenu->memberActual == NULL )
			{
				if ( nMouseY < cmenu->panel->size_y + memberTemp->size_y )
					cmenu->memberActual = memberTemp;
			}
		}
		
		memberTemp->draw ( memberTemp );
		cmenu->panel->size_y += memberTemp->size_y;
		
		if ( memberTemp->flags & CM_OPENED )
		{
			var old_position = cmenu->panel->size_y;
			drwCMMembers ( memberTemp->child, memberTemp->count );
			vec_set ( &vecPos, vector ( member->tab-1, old_position, 0 ) );
			draw_line ( &vecPos, cmenu->colText, 0 );
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.y = cmenu->panel->size_y;
			draw_line ( &vecPos, cmenu->colText, 100 );
			draw_line ( &vecPos, cmenu->colText, 0 );
		}
	}
}

void drwCMComplete ( COMPACT_MENU *cmenu )
{
	int i = 0;
	for ( ; i<cmenu->digits; i++ )
		pan_setdigits ( cmenu->panel, i+1, 0, -100000, "", cmenu->font, 1, &0 );
	cmenu->digits = 1;
	cmenu->memberActual = NULL;
	cmenu->panel->size_y = 0;
	bmap_rendertarget ( cmenu->panel->bmap, 0, 0 );
	draw_line ( nullvector, nullvector, 0 );
	drwCMMembers ( cmenu->member.child, cmenu->member.count );
	draw_line ( nullvector, cmenu->colOver, 0 );
	draw_line ( nullvector, cmenu->colOver, 100 );
	draw_line ( vector(0,cmenu->panel->size_y,0), cmenu->colOver, 100 );
	draw_line ( vector(0,cmenu->panel->size_y,0), cmenu->colOver, 0 );
	draw_line ( vector(cmenu->panel->size_x,cmenu->panel->size_y,0), cmenu->colOver, 0 );
	draw_line ( vector(cmenu->panel->size_x,cmenu->panel->size_y,0), cmenu->colOver, 100 );
	draw_line ( vector(cmenu->panel->size_x,0,0), cmenu->colOver, 100 );
	draw_line ( vector(cmenu->panel->size_x,0,0), cmenu->colOver, 0 );
	
	if ( cmenu != cmenuFirst )
		draw_quad ( NULL, nullvector, NULL, vector(cmenu->panel->size_x,cmenu->panel->size_x,0), NULL, cmenu->colOver, 0, 0 );
	
	bmap_rendertarget ( NULL, 0, 0 );
	cmenu->panel->size_y -= 1;
}

void drwCMDelayed ( COMPACT_MENU *cmenu )
{
	wait(1);
	drwCMComplete ( cmenu );
}

void drwCMTextTargetMap ( COMPACT_MENU *cmenu, TEXT *text )
{
	text->flags |= SHOW;
	if ( text->target_map != NULL )
		bmap_remove ( text->target_map );
	text->target_map = bmap_createblack ( cmenu->panel->size_x-CM_TAB_RIGHT, cmenu->font->dy*text->strings, 32 );
	wait(1);
	text->flags &= SHOW;
}

void fncCMMembersResize ( CMMEMBER *member, int count )
{
	drwCMTextTargetMap ( member->cmenu, member->text );

	CMMEMBER *memberTemp = member;
	CMMEMBER *memberTempLast = memberTemp + count;
	for ( ; memberTemp<memberTempLast; memberTemp++ )
	{
		if ( memberTemp->resize != NULL )
			memberTemp->resize ( memberTemp );
		if ( memberTemp->flags & CM_SUBMENU )
			fncCMMembersResize ( memberTemp->child, memberTemp->count );
	}
}

void fncCMMembersRemove ( CMMEMBER *member, int count )
{
	bmap_remove ( member->text->target_map );
	txt_remove ( member->text );
	CMMEMBER *memberTemp = member;
	CMMEMBER *memberTempLast = memberTemp + count;
	for ( ; memberTemp<memberTempLast; memberTemp++ )
	{
		if ( memberTemp->flags & CM_SUBMENU )
			fncCMMembersRemove ( memberTemp->child, memberTemp->count );
		if ( memberTemp->flags & CM_FREE )
			sys_free ( memberTemp->child );
	}
}


// SUBMENU
/* ---------------------------------------------------------------------------------------------------- */

void fncCMSubmenuResize ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	member->size_y = cmenu->font->dy;
}

void evnCMSubmenu ( CMMEMBER *member )
{
	member->flags ^= CM_OPENED;
	if ( member->flags & CM_OPENED )
	{
		CMMEMBER *memberTemp = member->child;
		CMMEMBER *memberTempLast = memberTemp + member->count;
		for ( ; memberTemp<memberTempLast; memberTemp++ )
			memberTemp->flags &= ~CM_OPENED;
		memberTemp = member->parent->child;
		memberTempLast = memberTemp + member->parent->count;
		for ( ; memberTemp<memberTempLast; memberTemp++ )
		{
			if ( memberTemp == member )
				continue;
			memberTemp->flags &= ~CM_OPENED;
		}
	}
	drwCMComplete ( member->cmenu );
}

void drwCMSubmenu ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );
	VECTOR vecOffset;
	if ( member->flags & CM_OPENED )
	{
		vec_set ( &vecPos, vector ( member->tab-1, member->pos_y+member->size_y-3, 0 ) );
		vec_set ( &vecSize, vector(cmenu->panel->size_x-vecPos.x,3,0) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colOver, 100, 0 );
		if ( member == cmenu->memberActual )
		{
			vec_set ( &vecPos, vector ( cmenu->panel->size_x+1-CM_TAB_SRIGHT, member->pos_y+member->size_y-4, 0 ) );
			draw_line ( &vecPos, cmenu->colText, 0 );
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.x += 3;
			vecPos.y -= 7;
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.x += 3;
			vecPos.y += 7;
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.x -= 6;
			draw_line ( &vecPos, cmenu->colText, 100 );
			draw_line ( &vecPos, cmenu->colText, 0 );
		}
	}
	else
	{
		if ( member == cmenu->memberActual )
		{
			vec_set ( &vecPos, vector ( member->tab-1, member->pos_y+member->size_y-3, 0 ) );
			vec_set ( &vecSize, vector(cmenu->panel->size_x-vecPos.x,3,0) );
			draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colOver, 100, 0 );
			vec_set ( &vecPos, vector ( cmenu->panel->size_x+1-CM_TAB_SRIGHT, member->pos_y+member->size_y-11, 0 ) );
			draw_line ( &vecPos, cmenu->colText, 0 );
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.x += 3;
			vecPos.y += 7;
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.x += 3;
			vecPos.y -= 7;
			draw_line ( &vecPos, cmenu->colText, 100 );
			vecPos.x -= 6;
			draw_line ( &vecPos, cmenu->colText, 100 );
			draw_line ( &vecPos, cmenu->colText, 0 );
		}
		vec_set ( &vecPos, vector ( member->tab, member->pos_y+(member->size_y/2)-2, 0 ) );
		draw_line ( &vecPos, cmenu->colText, 0 );
		draw_line ( &vecPos, cmenu->colText, 100 );
		vecPos.y += 5;
		draw_line ( &vecPos, cmenu->colText, 100 );
		draw_line ( &vecPos, cmenu->colText, 0 );
		vec_set ( &vecPos, vector ( member->tab-1, member->pos_y+(member->size_y/2), 0 ) );
		draw_line ( &vecPos, cmenu->colText, 0 );
		draw_line ( &vecPos, cmenu->colText, 100 );
		vecPos.x += 3;
		draw_line ( &vecPos, cmenu->colText, 100 );
		draw_line ( &vecPos, cmenu->colText, 0 );
	}
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	vec_set ( &vecOffset, vector ( 0, member->index * cmenu->font->dy, 0 ) );
	vec_set ( &vecSize, vector ( cmenu->panel->size_x-CM_TAB_SRIGHT, cmenu->font->dy, 0 ) );
	draw_quad ( member->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void fncCMSubmenuCreate ( CMMEMBER *member, STRING *strData )
{
	COMPACT_MENU *cmenu = member->cmenu;
	TEXT *txtSub = var_for_name ( strData );
	#ifdef CM_SAFE_MODE
		if ( txtSub == NULL )
		{
			str_cat ( strData, "\nTEXT struct not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	member->flags = CM_ACTIVE | CM_SUBMENU | CM_FREE;
	member->event = evnCMSubmenu;
	member->draw = drwCMSubmenu;
	member->resize = fncCMSubmenuResize;
	member->count = txtSub->strings;
	member->child = memberCMTextParse ( cmenu, member, txtSub );
	
	fncCMSubmenuResize ( member );
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

void fncCMSliderResize ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	CMSLIDER *slider = member->child;
	slider->size_x = ( cmenu->panel->size_x - member->tab - CM_TAB_RIGHT ) - 1;
	slider->slide_x = *slider->value;
	slider->slide_x -= slider->min;
	slider->slide_x /= slider->range;
	slider->slide_x *= slider->size_x;
	member->size_y = cmenu->font->dy;
}

void drwCMSlider ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );
	CMSLIDER *slider = member->child;
	if ( member == cmenu->memberActual )
	{
		vec_set ( &vecPos, vector ( member->tab, member->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( slider->slide_x, member->size_y-2, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colOver, 100, 0 );
		vecPos.x += slider->size_x;
		vecPos.y += member->size_y - 2;
		draw_line ( &vecPos, cmenu->colOver, 0 );
		draw_line ( &vecPos, cmenu->colOver, 100 );
		vecPos.x -= slider->size_x;
		draw_line ( &vecPos, cmenu->colOver, 100 );
		draw_line ( &vecPos, cmenu->colOver, 0 );
		
		vec_set ( &vecPos, vector ( member->tab+slider->slide_x, member->pos_y+1, 0 ) );
		draw_line ( &vecPos, cmenu->colText, 0 );
		draw_line ( &vecPos, cmenu->colText, 100 );
		vecPos.y += member->size_y-2;
		draw_line ( &vecPos, cmenu->colText, 100 );
		draw_line ( &vecPos, cmenu->colText, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( member->tab+slider->size_x, member->pos_y+member->size_y-1, 0 ) );
		draw_line ( &vecPos, cmenu->colOver, 0 );
		draw_line ( &vecPos, cmenu->colOver, 100 );
		vecPos.x -= slider->size_x * CM_TAB_LINE;
		draw_line ( &vecPos, cmenu->colBack, 100 );
		draw_line ( &vecPos, cmenu->colBack, 0 );
	}
	
	if ( !pan_setdigits ( cmenu->panel, cmenu->digits, cmenu->panel->size_x - CM_TAB_DIGIT, member->pos_y, slider->format, cmenu->font, 1, slider->value ) )
	{
		pan_setdigits ( cmenu->panel, 0, cmenu->panel->size_x - CM_TAB_DIGIT, member->pos_y, slider->format, cmenu->font, 1, slider->value );
	}
	cmenu->digits += 1;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecOffset;
	vec_set ( &vecOffset, vector ( 0, member->index * cmenu->font->dy, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector(cmenu->panel->size_x-CM_TAB_RIGHT,cmenu->font->dy,0) );
	draw_quad ( member->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void drwCMSliderUpdate ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	CMSLIDER *slider = member->child;
	VECTOR vecPos;
	vec_set ( &vecPos, vector( member->tab, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x-member->tab, member->size_y, 0 ) );
	bmap_rendertarget ( cmenu->panel->bmap, 0, 1 );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );
	vec_set ( &vecPos, vector ( member->tab, member->pos_y+1, 0 ) );
	vec_set ( &vecSize, vector ( maxv(slider->slide_x,1), member->size_y-2, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colOver, 100, 0 );
	vecPos.x += slider->size_x;
	vecPos.y += member->size_y - 2;
	draw_line ( &vecPos, cmenu->colOver, 0 );
	draw_line ( &vecPos, cmenu->colOver, 100 );
	vecPos.x -= slider->size_x;
	draw_line ( &vecPos, cmenu->colOver, 100 );
	draw_line ( &vecPos, cmenu->colOver, 0 );
	
	vec_set ( &vecPos, vector ( member->tab+slider->slide_x, member->pos_y+1, 0 ) );
	draw_line ( &vecPos, cmenu->colText, 0 );
	draw_line ( &vecPos, cmenu->colText, 100 );
	vecPos.y += member->size_y-2;
	draw_line ( &vecPos, cmenu->colText, 100 );
	draw_line ( &vecPos, cmenu->colText, 0 );
	
	VECTOR vecOffset;
	vec_set ( &vecOffset, vector ( 0, member->index * cmenu->font->dy, 0 ) );
	vec_set ( &vecSize, vector ( cmenu->panel->size_x-CM_TAB_RIGHT, cmenu->font->dy, 0 ) );
	vec_set ( &vecPos, vector( 0, member->pos_y, 0 ) );
	draw_quad ( member->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
	bmap_rendertarget ( NULL, 0, 0 );
}

void evnCMSlider ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	CMSLIDER *slider = member->child;
	var old_value = 0;
	while ( mouse_left )
	{
		old_value = *slider->value;
		slider->slide_x = clamp ( mouse_pos.x - ( cmenu->panel->pos_x + member->tab ), 0, slider->size_x );
		*slider->value = slider->slide_x;
		*slider->value /= slider->size_x;
		*slider->value *= slider->range;
		if ( slider->step != 0 )
			*slider->value = integer ( *slider->value / slider->step ) * slider->step;
		*slider->value += slider->min;
		if ( old_value != *slider->value )
		{
			drwCMSliderUpdate ( member );
		}
		if ( slider->event != NULL )
			slider->event ();
		wait(1);
	}
}

void fncCMSliderCreate ( CMMEMBER *member, STRING *strData )
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
	
	COMPACT_MENU *cmenu = member->cmenu;
	*pointer = clamp ( *pointer, nMin, nMax );
	CMSLIDER *slider = sys_malloc ( sizeof(CMSLIDER) );
	slider->min = nMin;
	slider->range = nMax - nMin;
	slider->step = nStep;
	slider->value = pointer;
	slider->format = str_create ( strCMType );
	slider->event = fncCMPrototype;
//	slider->digit = pan_setdigits ( cmenu->panel, 0, 0, 0, "", cmenu->font, 1, pointer );
//	cmenu->digits ++;
	
	member->flags = CM_ACTIVE | CM_FREE;
	member->event = evnCMSlider;
	member->resize = fncCMSliderResize;
	member->draw = drwCMSlider;
	member->count = 0;
	member->child = slider;
	fncCMSliderResize ( member );
}


// BITMAP
/* ---------------------------------------------------------------------------------------------------- */

void fncCMBitmapResize ( CMMEMBER *member )
{
	BMAP **bmpTemp = member->child;
	COMPACT_MENU *cmenu = member->cmenu;
	if ( *bmpTemp != NULL )
		member->size_y = ( bmap_height ( *bmpTemp ) * ( cmenu->panel->size_x - member->tab - CM_TAB_RIGHT ) ) / bmap_width ( *bmpTemp );
	else
		member->size_y = cmenu->font->dy;
}

void drwCMBitmap ( CMMEMBER *member )
{
	fncCMBitmapResize ( member );
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );
	BMAP **bmpTemp = member->child;
	if ( *bmpTemp != NULL )
	{
		vecPos.x = member->tab;
		var nProportion = ( cmenu->panel->size_x - vecPos.x - CM_TAB_RIGHT ) / bmap_width ( *bmpTemp );
		VECTOR vecScale;
		vec_set ( &vecScale, vector(nProportion,nProportion,0) );
		draw_quad ( *bmpTemp, &vecPos, NULL, vector(bmap_width(*bmpTemp),bmap_height(*bmpTemp),0), &vecScale, NULL, 100, 0 );
	}
	else
	{
		VECTOR vecOffset;
		vec_set ( &vecOffset, vector ( 0, member->index * cmenu->font->dy, 0 ) );
		draw_quad ( member->text->target_map, &vecPos, &vecOffset, vector(vecSize.x-CM_TAB_RIGHT,cmenu->font->dy,0), NULL, NULL, 100, 0 );
	}
}


void fncCMBitmapCreate ( CMMEMBER *member, STRING *strData )
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
	member->flags = 0;
	member->event = NULL;
	member->draw = drwCMBitmap;
	member->resize = fncCMBitmapResize;
	member->count = 0;
	member->child = bmapPtr;
	str_cpy ( (member->text->pstring)[member->index], "NULL" );
	
	fncCMBitmapResize ( member );
}


// FUNCTION
/* ---------------------------------------------------------------------------------------------------- */

void fncCMButtonResize ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	member->size_y = cmenu->font->dy;
}

void drwCMButton ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );
	if ( member == cmenu->memberActual )
	{
		vec_set ( &vecPos, vector ( member->tab-1, member->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( cmenu->panel->size_x+1-member->tab, member->size_y-1, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colOver, 100, 0 );		
		vec_set ( &vecPos, vector ( cmenu->panel->size_x-3, member->pos_y+(member->size_y/2), 0 ) );
		draw_line ( &vecPos, cmenu->colText, 0 );
		draw_line ( &vecPos, cmenu->colText, 100 );
		vecPos.x -= 8;
		vecPos.y -= 3;
		draw_line ( &vecPos, cmenu->colText, 100 );
		vecPos.y += 6;
		draw_line ( &vecPos, cmenu->colText, 100 );
		vecPos.x += 8;
		vecPos.y -= 3;
		draw_line ( &vecPos, cmenu->colText, 100 );
		draw_line ( &vecPos, cmenu->colText, 0 );
	}
	VECTOR vecOffset;
	vec_set ( &vecOffset, vector ( 0, member->index * cmenu->font->dy, 0 ) );
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	vec_set ( &vecSize, vector ( cmenu->panel->size_x-CM_TAB_SRIGHT, cmenu->font->dy, 0 ) );
	draw_quad ( member->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void fncCMButtonCreate ( CMMEMBER *member, STRING *strData )
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
	member->flags = CM_ACTIVE;
	member->event = fncCMPrototype;
	member->draw = drwCMButton;
	member->resize = fncCMButtonResize;
	member->count = 0;
	member->child = NULL;
	
	fncCMButtonResize ( member );
}


// TITLE
/* ---------------------------------------------------------------------------------------------------- */

void fncCMTitleResize ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	member->size_y = member->count + 2;
	if ( str_len ( (member->text->pstring)[member->index] ) > 0 )
		member->size_y += cmenu->font->dy;
}

void evnCMTitle ( CMMEMBER *member )
{
	VECTOR oldMousePos, oldPanelPos, vecTemp;
	COMPACT_MENU *cmenu = member->cmenu;
	PANEL *panel = cmenu->panel;
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

void drwCMTitle ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colOver, 100, 0 );
	draw_line ( &vecPos, cmenu->colText, 0 );
	draw_line ( &vecPos, cmenu->colText, 100 );
	vecPos.x = cmenu->panel->size_x;
	draw_line ( &vecPos, cmenu->colText, 100 );
	vecPos.y += 1;
	draw_line ( &vecPos, cmenu->colBack, 100 );
	vecPos.x = -1;
	draw_line ( &vecPos, cmenu->colOver, 100 );
	draw_line ( &vecPos, cmenu->colText, 0 );
	
	vec_set ( &vecPos, vector ( 0, member->pos_y+member->size_y-1, 0 ) );
	draw_line ( &vecPos, cmenu->colText, 0 );
	draw_line ( &vecPos, cmenu->colText, 100 );
	vecPos.x = cmenu->panel->size_x;
	draw_line ( &vecPos, cmenu->colText, 100 );
	vecPos.y -= 1;
	draw_line ( &vecPos, cmenu->colBack, 100 );
	vecPos.x = -1;
	draw_line ( &vecPos, cmenu->colText, 100 );
	draw_line ( &vecPos, cmenu->colText, 0 );
	
	VECTOR vecOffset;
	vec_set ( &vecOffset, vector ( 0, member->index * cmenu->font->dy, 0 ) );
	vec_set ( &vecPos, vector ( 0, member->pos_y + member->count, 0 ) );
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( member->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void fncCMTitleCreate ( CMMEMBER *member, STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	member->flags = CM_ACTIVE;
	member->event = evnCMTitle;
	member->draw = drwCMTitle;
	member->resize = fncCMTitleResize;
	member->count = size_y;
	member->child = NULL;
	
	fncCMTitleResize ( member );
}


// LINE
/* ---------------------------------------------------------------------------------------------------- */

void drwCMLine ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );
	vecPos.x += member->tab - CM_TAB_STEP;
	vecPos.y += member->count;
	draw_line ( &vecPos, cmenu->colText, 0 );
	draw_line ( &vecPos, cmenu->colText, 100 );
	vecPos.x = cmenu->panel->size_x * CM_TAB_LINE;
	draw_line ( &vecPos, cmenu->colBack, 100 );
	draw_line ( &vecPos, cmenu->colBack, 0 );
}

void fncCMLineCreate ( CMMEMBER *member, STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	member->flags = 0;
	member->event = NULL;
	member->draw = drwCMLine;
	member->count = size_y;
	member->resize = NULL;
	member->size_y = size_y+1;
	member->child = NULL;
}



// SEPARATOR
/* ---------------------------------------------------------------------------------------------------- */

void drwCMSeparator ( CMMEMBER *member )
{
	COMPACT_MENU *cmenu = member->cmenu;
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, member->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenu->panel->size_x, member->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenu->colBack, 100, 0 );	
	vecPos.x += member->tab - CM_TAB_STEP;
	vecPos.y += member->count;
	draw_line ( &vecPos, cmenu->colOver, 0 );
	draw_line ( &vecPos, cmenu->colOver, 100 );
	vecPos.x = cmenu->panel->size_x * CM_TAB_LINE;
	draw_line ( &vecPos, cmenu->colBack, 100 );
	draw_line ( &vecPos, cmenu->colBack, 0 );
}

void fncCMSeparatorCreate ( CMMEMBER *member, STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	member->flags = 0;
	member->event = NULL;
	member->draw = drwCMSeparator;
	member->count = 1;
	member->resize = NULL;
	member->size_y = size_y+1;
	member->child = NULL;
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
				memberTemp->event ( memberTemp );
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

CMMEMBER *memberCMTextParse ( COMPACT_MENU *cmenu, CMMEMBER *parent, TEXT *txtMembers )
{
	int iMembersCount = txtMembers->strings;
	TEXT *txtTemp = txt_create ( iMembersCount, 1 );
	int i = 0;
	for ( ; i<iMembersCount; i++ )
	{
		str_cpy ( (txtTemp->pstring)[i], (txtMembers->pstring)[i] ); 
	}
	
	txtTemp->pos_x = parent->tab + CM_TAB_TEXT;
	txtTemp->pos_y = 0;
	txtTemp->font = cmenu->font;
	vec_set ( &txtTemp->blue, colCMText );
	drwCMTextTargetMap ( cmenu, txtTemp );
	
	CMMEMBER *members = sys_malloc ( sizeof(CMMEMBER) * iMembersCount );
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
		
		CMMEMBER *memberTemp = members + i;
		memberTemp->index = i;
		memberTemp->tab = parent->tab + CM_TAB_STEP;
		memberTemp->text = txtTemp;
		memberTemp->parent = parent;
		memberTemp->cmenu = cmenu;
		if ( str_cmpi ( strCMType, "submenu" ) )
		{
			fncCMSubmenuCreate ( memberTemp, strCMData );
		}
		else if ( str_cmpi ( strCMType, "button" ) )
		{
			fncCMButtonCreate ( memberTemp, strCMData );
		}
		else if ( str_cmpi ( strCMType, "slider" ) )
		{
			fncCMSliderCreate ( memberTemp, strCMData );
		}
		else if ( str_cmpi ( strCMType, "bmap" ) )
		{
			fncCMBitmapCreate ( memberTemp, strCMData );
		}
		else if ( str_cmpi ( strCMType, "line" ) )
		{
			fncCMLineCreate ( memberTemp, strCMData );
		}
		else if ( str_cmpi ( strCMType, "space" ) )
		{
			fncCMSeparatorCreate ( memberTemp, strCMData );
		}
		else if ( str_cmpi ( strCMType, "title" ) )
		{
			fncCMTitleCreate ( memberTemp, strCMData );
		}
		else
		{
			memberTemp->flags = CM_INVISIBLE;
		}
	}
	
	return members;
}

PANEL *cmenu_create ( var pos_x, var pos_y, var size_x, var layer, var flags, TEXT *txtMembers )
{
	#ifdef CM_SAFE_MODE
		if ( txtMembers == NULL )
		{
			str_cpy ( strCMTemp, "null categories TEXT on cmenu_create" );
			error ( strCMTemp );
			sys_exit ( NULL );
		}
		if ( txtMembers->strings < 1 )
		{
			str_cpy ( strCMTemp, "empty categories TEXT on cmenu_create" );
			error ( strCMTemp );
			sys_exit ( NULL );
		}
	#endif
	
	if ( !proc_status ( fncCMRun ) )
		fncCMRun ();
	
	PANEL *panBG = pan_create ( "", layer );
	panBG->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
	panBG->pos_x = pos_x;
	panBG->pos_y = pos_y;
	panBG->size_x = size_x;
	panBG->size_y = 0;
	panBG->flags = flags | ARIGHT;
	vec_set ( &panBG->blue, colCMText );
	
	COMPACT_MENU *cmenuTemp = sys_malloc ( sizeof(COMPACT_MENU) );
	cmenuTemp->font = txtMembers->font;
	cmenuTemp->panel = panBG;
	cmenuTemp->digits = 0;
	cmenuTemp->member.index = 0;
	cmenuTemp->member.text = NULL;
	cmenuTemp->member.flags = CM_OPENED;
	cmenuTemp->member.parent = NULL;
	cmenuTemp->member.cmenu = cmenuTemp;
	cmenuTemp->member.tab = 0;
	cmenuTemp->colBack = colCMBack;
	cmenuTemp->colOver = colCMOver;
	cmenuTemp->colText = colCMText;
	cmenuTemp->member.event = NULL;
	cmenuTemp->member.draw = NULL;
	cmenuTemp->member.resize = NULL;
	cmenuTemp->member.count = txtMembers->strings;
	cmenuTemp->member.child = memberCMTextParse ( cmenuTemp, &cmenuTemp->member, txtMembers );
	cmenuTemp->next = cmenuFirst;
	cmenuFirst = cmenuTemp;
	
	drwCMDelayed ( cmenuTemp );
	
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
			fncCMMembersResize ( cmenuTemp->member.child, cmenuTemp->member.count );
			drwCMDelayed ( cmenuTemp );
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
			
			fncCMMembersRemove ( cmenuTemp->member.child, cmenuTemp->member.count );
			
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
	cmenuMouseLast = NULL;
	COMPACT_MENU *cmenuTemp = cmenuFirst;
	while ( cmenuTemp != NULL )
	{
		bmap_remove ( cmenuTemp->panel->bmap );
		pan_remove ( cmenuTemp->panel );
		
		fncCMMembersRemove ( cmenuTemp->member.child, cmenuTemp->member.count );
		
		COMPACT_MENU *cmenu = cmenuTemp;
		cmenuTemp = cmenuTemp->next;
		
		sys_free ( (CMMEMBER*)cmenu->member.child );
		sys_free ( cmenu );
	}
}