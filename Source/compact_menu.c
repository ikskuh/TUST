

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
	CMSTYLE *style;
	COLOR *colText;
	COLOR *colBack;
	COLOR *colOver;
	CMMEMBER cmmember;
	CMMEMBER *cmmemberActual;
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
CMMEMBER *cmmemberMe = NULL;
STRING *strCMTemp = "";
STRING *strCMData = "";
STRING *strCMType = "";
STRING *strCMEvent = "";


// PROTOTYPES
/* ---------------------------------------------------------------------------------------------------- */

void fncCMPrototype ( void *object );
void fncCMConstructor ( STRING *strData );
//CMMEMBER *cmmemberCMTextParse ( TEXT *txtMembers );


// GLOBAL FUNCTIONS
/* ---------------------------------------------------------------------------------------------------- */

void drwCMMembers ()
{
	VECTOR vecPos;
	var nMouseY = mouse_pos.y - cmenuMe->panel->pos_y;
	CMMEMBER **cmmemberTemp = cmmemberMe->child;
	CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
	for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp ++ )
	{
		(*cmmemberTemp)->pos_y = cmenuMe->panel->size_y;
		if ( (*cmmemberTemp)->flags & CM_INVISIBLE )
			continue;
		
		if ( mouse_panel == cmenuMe->panel )
		{
			if ( cmenuMe->cmmemberActual == NULL )
			{
				if ( nMouseY <= cmenuMe->panel->size_y + (*cmmemberTemp)->size_y )
					cmenuMe->cmmemberActual = *cmmemberTemp;
			}
		}
		
		cmmemberMe = *cmmemberTemp;
		(*cmmemberTemp)->draw ();
		cmenuMe->panel->size_y += (*cmmemberTemp)->size_y;
		
		if ( (*cmmemberTemp)->flags & CM_OPENED )
		{
			var old_position = cmenuMe->panel->size_y;
			drwCMMembers ();
			vec_set ( &vecPos, vector ( (*cmmemberTemp)->tab-1, old_position, 0 ) );
			draw_line ( &vecPos, cmenuMe->style->colText, 0 );
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.y = cmenuMe->panel->size_y;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			draw_line ( &vecPos, cmenuMe->style->colText, 0 );
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
	cmenu->cmmemberActual = NULL;
	cmenu->panel->size_y = 0;
	
	bmap_rendertarget ( cmenu->panel->bmap, 0, 0 );
	draw_line ( nullvector, nullvector, 0 );
	
	cmenuMe = cmenu;
	cmmemberMe = &cmenu->cmmember;
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
	CMMEMBER **cmmemberTemp = cmmemberMe->child;
	CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
	for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
	{
		cmmemberMe = *cmmemberTemp;
		if ( (*cmmemberTemp)->resize != NULL )
			cmmemberMe->resize ();
		if ( (*cmmemberTemp)->flags & CM_SUBMENU )
			fncCMMembersResize ();
	}
}

void fncCMMembersRemove ()
{
	bmap_remove ( cmmemberMe->text->target_map );
	txt_remove ( cmmemberMe->text );
	CMMEMBER **cmmemberTemp = cmmemberMe->child;
	CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
	for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
	{
		cmmemberMe = *cmmemberTemp;
		if ( (*cmmemberTemp)->flags & CM_SUBMENU )
			fncCMMembersRemove ();
		if ( (*cmmemberTemp)->flags & CM_FREE )
			sys_free ( (*cmmemberTemp)->child );
	}
}

void cmcmmember_draw_name ()
{
	if ( cmenuMe->strings == cmenuMe->text->strings )
		txt_addstring ( cmenuMe->text, NULL );
	STRING *strTemp = (cmenuMe->text->pstring)[cmenuMe->strings];
	str_cpy ( strTemp, cmmemberMe->name );
	var posX = cmmemberMe->tab + str_width(strTemp,cmenuMe->style->font) + CM_TAB_TEXT;
	if ( !pan_setstring ( cmenuMe->panel, cmenuMe->digits, posX, cmmemberMe->pos_y, cmenuMe->style->font, strTemp ) )
		pan_setstring ( cmenuMe->panel, 0, posX, cmmemberMe->pos_y, cmenuMe->style->font, strTemp );
	cmenuMe->strings ++;
	cmenuMe->digits ++;
}

void fncCMSubmenuResize ()
{
	cmmemberMe->size_y = cmenuMe->style->font->dy;
}

void evnCMSubmenu ()
{
	cmmemberMe->flags ^= CM_OPENED;
	if ( cmmemberMe->flags & CM_OPENED )
	{
		CMMEMBER **cmmemberTemp = cmmemberMe->child;
		CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
		for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
			(*cmmemberTemp)->flags &= ~CM_OPENED;
		cmmemberTemp = cmmemberMe->parent->child;
		cmmemberTempLast = cmmemberTemp + cmmemberMe->parent->count;
		for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
		{
			if ( *cmmemberTemp == cmmemberMe )
				continue;
			(*cmmemberTemp)->flags &= ~CM_OPENED;
		}
	}
	drwCMComplete ( cmenuMe );
}

void drwCMSubmenu ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );
	VECTOR vecOffset;
	if ( cmmemberMe->flags & CM_OPENED )
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab-1, cmmemberMe->pos_y+cmmemberMe->size_y-3, 0 ) );
		vec_set ( &vecSize, vector(cmenuMe->panel->size_x-vecPos.x,3,0) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );
		if ( cmmemberMe == cmenuMe->cmmemberActual )
		{
			vec_set ( &vecPos, vector ( cmenuMe->panel->size_x+1-CM_TAB_SRIGHT, cmmemberMe->pos_y+cmmemberMe->size_y-4, 0 ) );
			draw_line ( &vecPos, cmenuMe->style->colText, 0 );
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.x += 3;
			vecPos.y -= 7;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.x += 3;
			vecPos.y += 7;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.x -= 6;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		}
	}
	else
	{
		if ( cmmemberMe == cmenuMe->cmmemberActual )
		{
			vec_set ( &vecPos, vector ( cmmemberMe->tab-1, cmmemberMe->pos_y+cmmemberMe->size_y-3, 0 ) );
			vec_set ( &vecSize, vector(cmenuMe->panel->size_x-vecPos.x,3,0) );
			draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );
			vec_set ( &vecPos, vector ( cmenuMe->panel->size_x+1-CM_TAB_SRIGHT, cmmemberMe->pos_y+cmmemberMe->size_y-11, 0 ) );
			draw_line ( &vecPos, cmenuMe->style->colText, 0 );
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.x += 3;
			vecPos.y += 7;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.x += 3;
			vecPos.y -= 7;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			vecPos.x -= 6;
			draw_line ( &vecPos, cmenuMe->style->colText, 100 );
			draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		}
		vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+(cmmemberMe->size_y/2)-2, 0 ) );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		vecPos.y += 5;
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		vec_set ( &vecPos, vector ( cmmemberMe->tab-1, cmmemberMe->pos_y+(cmmemberMe->size_y/2), 0 ) );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		vecPos.x += 3;
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	}
	
	cmcmmember_draw_name ();
}

CMMEMBER *cmmemberCMTextParse ( TEXT *txtMembers )
{
	int iMembersCount = txtMembers->strings;
	TEXT *txtTemp = txt_create ( iMembersCount, 1 );
	int i = 0;
	for ( ; i<iMembersCount; i++ )
	{
		str_cpy ( (txtTemp->pstring)[i], (txtMembers->pstring)[i] ); 
	}
	
	CMMEMBER *parent = cmmemberMe;
	CMMEMBER **cmmembers = sys_malloc ( sizeof(CMMEMBER*) * iMembersCount );
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
		
		CMMEMBER *cmmemberTemp = sys_malloc ( sizeof(CMMEMBER) );
		*(cmmembers+i) = cmmemberTemp;
		cmmemberTemp->name = (txtTemp->pstring)[i];
		cmmemberTemp->index = i;
		cmmemberTemp->tab = parent->tab + CM_TAB_STEP;
		cmmemberTemp->text = txtTemp;
		cmmemberTemp->parent = parent;
		
		str_cat ( strCMType, "CMTypeCreate" );
		fncCMConstructor = engine_getscript ( strCMType->chars );
		if ( fncCMConstructor != NULL )
		{
			cmmemberMe = cmmemberTemp;
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
			cmmemberTemp->flags |= CM_INVISIBLE;
		}
	}
	
	return cmmembers;
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
	cmmemberMe->flags = CM_ACTIVE | CM_SUBMENU | CM_FREE;
	cmmemberMe->event = evnCMSubmenu;
	cmmemberMe->draw = drwCMSubmenu;
	cmmemberMe->resize = fncCMSubmenuResize;
	cmmemberMe->count = txtSub->strings;
	fncCMSubmenuResize ();
	CMMEMBER *cmmemberTemp = cmmemberMe;
	cmmemberTemp->child = cmmemberCMTextParse ( txtSub );
}

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
			CMMEMBER *cmmemberTemp = cmenuMouseLast->cmmemberActual;
			if ( ( nMouseY <= cmmemberTemp->pos_y ) || ( nMouseY > cmmemberTemp->pos_y + cmmemberTemp->size_y ) )
			{
				drwCMComplete ( cmenuMouseLast );
			}
			else if ( mouse_left && !old_mouse_left && ( cmmemberTemp->flags & CM_ACTIVE ) )
			{
				cmmemberMe = cmmemberTemp;
				cmmemberTemp->event ();
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

/* ---------------------------------------------------------------------------------------------------- */

CMSTYLE *cmstyle_create ( FONT *font, COLOR *colText, COLOR *colBackground, COLOR *colOver )
{
	CMSTYLE *style = sys_malloc ( sizeof(CMSTYLE) );
	style->font = font;
	vec_set ( style->colText, colText );
	vec_set ( style->colBack, colBackground );
	vec_set ( style->colOver, colOver );
	return style;
} 

PANEL *cmenu_create ( char *chrName, var pos_x, var pos_y, var size_x, var layer, var flags, char *chrMembers, CMSTYLE *style )
{
	PANEL *panBG = pan_create ( "", layer );
	panBG->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
	panBG->pos_x = pos_x;
	panBG->pos_y = pos_y;
	panBG->size_x = size_x;
	panBG->size_y = 0;
	panBG->flags = flags | ARIGHT;
	vec_set ( &panBG->blue, style->colText );
	
	cmenuMe = sys_malloc ( sizeof(COMPACT_MENU) );
	cmenuMe->name = str_create ( chrName );
	cmenuMe->style = style;
	cmenuMe->panel = panBG;
	cmenuMe->digits = 0;
	cmenuMe->strings = 0;
	cmenuMe->text = txt_create ( 1, 1 );
	txt_addstring ( cmenuMe->text, chrMembers );
	
	cmenuMe->cmmember.index = 0;
	cmenuMe->cmmember.text = cmenuMe->text;
	cmenuMe->cmmember.flags = CM_OPENED;
	cmenuMe->cmmember.parent = NULL;
	cmenuMe->cmmember.tab = 0;
	cmenuMe->cmmember.event = NULL;
	cmenuMe->cmmember.draw = NULL;
	cmenuMe->cmmember.resize = NULL;
	cmenuMe->cmmember.name = cmenuMe->name;
	cmmemberMe = &cmenuMe->cmmember;
	submenuCMTypeCreate ( (cmenuMe->text->pstring)[0] );
	
	cmenuMe->next = cmenuFirst;
	cmenuFirst = cmenuMe;
	
	drwCMComplete ( cmenuMe );
	
	if ( !proc_status ( fncCMRun ) )
		fncCMRun ();
	
	return panBG;
}

void cmenu_modify ( PANEL *panel, var size_x, CMSTYLE *style )
{
	COMPACT_MENU *cmenuTemp = cmenuFirst;
	while ( cmenuTemp != NULL )
	{
		if ( cmenuTemp->panel == panel )
		{
			cmenuTemp->style = style;
			vec_set ( cmenuTemp->panel->blue, style->colText );
			if ( ( size_x != 0 ) && ( size_x != cmenuTemp->panel->size_x ) )
			{
				bmap_remove ( panel->bmap );
				panel->bmap = bmap_createblack ( size_x, screen_size.y, 32 );
				panel->size_x = size_x;
			}
			cmenuMe = cmenuTemp;
			cmmemberMe = &cmenuTemp->cmmember;
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
			cmmemberMe = &cmenuTemp->cmmember;
			fncCMMembersRemove ();
			
			if ( cmenuPrev == NULL )
				cmenuFirst = cmenuTemp->next;
			else
				cmenuPrev->next = cmenuTemp->next;
			
			sys_free ( (CMMEMBER*)cmenuTemp->cmmember.child );
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
		cmmemberMe = &cmenuTemp->cmmember;
		fncCMMembersRemove ();
		
		COMPACT_MENU *cmenu = cmenuTemp;
		cmenuTemp = cmenuTemp->next;
		
		sys_free ( (CMMEMBER*)cmenu->cmmember.child );
		sys_free ( cmenu );
	}
}