
typedef struct CMDIGIT
{
	var *value;
	STRING *format;
	void event ();
} CMDIGIT;

void fncCMDigitRemove ()
{
	CMDIGIT *digit = cmmemberMe->child;
	sys_free ( digit );
}

void fncCMDigitResize ()
{
	cmmemberMe->size_y = cmenuMe->style->font->dy;
}

void drwCMDigit ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );
	if ( cmmemberMe == cmenuMe->cmmemberActual )
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+cmmemberMe->size_y-3, 0 ) );
		vec_set ( &vecSize, vector ( cmenuMe->panel->size_x-cmmemberMe->tab-CM_TAB_RIGHT, 3, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( cmenuMe->panel->size_x-cmmemberMe->tab-CM_TAB_RIGHT, cmmemberMe->pos_y+cmmemberMe->size_y-1, 0 ) );
		draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
		draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
		vecPos.x -= ( cmenuMe->panel->size_x - cmmemberMe->tab - CM_TAB_RIGHT ) * CM_TAB_LINE;
		draw_line ( &vecPos, cmenuMe->style->colBack, 100 );
		draw_line ( &vecPos, cmenuMe->style->colBack, 0 );
	}
	
	CMDIGIT *digit = cmmemberMe->child;
	cmmember_draw_var ( digit->value, digit->format );
	cmmember_draw_name ();
}

void evnCMDigit ()
{
	CMDIGIT *digit = cmmemberMe->child;
	str_cpy ( strCMTemp, "" );
	var *pointer = digit->value;
	str_cat_num ( strCMTemp, digit->format, *pointer );
	int digitLength = str_len ( strCMTemp );
	int digitDecimals = str_stri ( strCMTemp, "." );
	if ( digitDecimals > 0 )
		digitDecimals = minv ( digitLength - digitDecimals, 3 );
	FONT *fntTemp = cmenuMe->style->font;
	PANEL *panTemp = cmenuMe->panel;
	var posX = panTemp->pos_x + panTemp->size_x - CM_TAB_DIGIT;
	posX -= mouse_pos.x;
	int digitPos = 0;
	char *chrFirst = strCMTemp->chars;
	char *chrLast = chrFirst + digitLength - 1;
	for ( ; chrLast>chrFirst; chrLast-- )
	{
		if ( str_width ( chrLast, fntTemp ) > posX )
			break;
		if ( str_stri ( chrLast, "." ) == 1 )
			continue;
		digitPos += 1;
	}
	if ( str_stri ( chrLast, "-" ) == 1 )
		digitPos -= 1;
	var digitStep;
	if ( digitPos > digitDecimals )
		digitStep = pow ( 10, digitPos - digitDecimals );
	else
		digitStep = 1 / pow ( 10, minv ( digitDecimals - digitPos, 3 ) );
	var mouseLapse = 0;
	while ( mouse_left )
	{
		mouseLapse += mouse_force.y;
		if ( abs(mouseLapse) > 5 )
		{
			*pointer += sign(mouseLapse) * digitStep;
			mouseLapse = 0;
			if ( digit->event != NULL )
				digit->event ();
		}
		wait(1);
	}
}

void digitCMTypeCreate ( STRING *strData )
{
	int iDigitFormat = clamp ( str_to_int ( strData ), 0, 3 );
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
	
	CMDIGIT *digit = sys_malloc ( sizeof(CMDIGIT) );
	digit->value = pointer;
	digit->format = (txtCMFormats->pstring)[iDigitFormat];
	digit->event = fncCMPrototype;
	
	cmmemberMe->flags = CM_ACTIVE;
	cmmemberMe->event = evnCMDigit;
	cmmemberMe->resize = fncCMDigitResize;
	cmmemberMe->remove = fncCMDigitRemove;
	cmmemberMe->draw = drwCMDigit;
	cmmemberMe->count = 0;
	cmmemberMe->child = digit;
	
	fncCMDigitResize ();
}
