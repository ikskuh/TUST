
typedef struct CMSLIDER
{
	var size_x;
	var min;
	var range;
	var step;
	STRING *format;
	var *value;
	void event ();
} CMSLIDER;

void fncCMSliderResize ()
{
	CMSLIDER *slider = cmmemberMe->child;
	slider->size_x = ( cmenuMe->panel->size_x - cmmemberMe->tab - CM_TAB_RIGHT ) - 1;
	cmmemberMe->size_y = cmenuMe->style->font->dy;
}

void drwCMSlider ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );
	CMSLIDER *slider = cmmemberMe->child;
	var slide_x = *slider->value;
	slide_x -= slider->min;
	slide_x /= slider->range;
	slide_x *= slider->size_x;
	if ( cmmemberMe == cmenuMe->cmmemberActual )
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( slide_x, cmmemberMe->size_y-2, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );
		vecPos.x += slider->size_x;
		vecPos.y += cmmemberMe->size_y - 2;
		draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
		draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
		vecPos.x -= slider->size_x;
		draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
		draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
		
		vec_set ( &vecPos, vector ( cmmemberMe->tab+slide_x, cmmemberMe->pos_y+1, 0 ) );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		vecPos.y += cmmemberMe->size_y-2;
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab+slider->size_x, cmmemberMe->pos_y+cmmemberMe->size_y-1, 0 ) );
		draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
		draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
		vecPos.x -= slider->size_x * CM_TAB_LINE;
		draw_line ( &vecPos, cmenuMe->style->colBack, 100 );
		draw_line ( &vecPos, cmenuMe->style->colBack, 0 );
	}
	
	if ( !pan_setdigits ( cmenuMe->panel, cmenuMe->digits, cmenuMe->panel->size_x - CM_TAB_DIGIT, cmmemberMe->pos_y, slider->format, cmenuMe->style->font, 1, slider->value ) )
	{
		pan_setdigits ( cmenuMe->panel, 0, cmenuMe->panel->size_x - CM_TAB_DIGIT, cmmemberMe->pos_y, slider->format, cmenuMe->style->font, 1, slider->value );
	}
	cmenuMe->digits += 1;
	cmmember_draw_name ();
}

void drwCMSliderUpdate ()
{
	CMSLIDER *slider = cmmemberMe->child;
	var slide_x = *slider->value;
	slide_x -= slider->min;
	slide_x /= slider->range;
	slide_x *= slider->size_x;
	slide_x = maxv ( slide_x, 1 );
	VECTOR vecPos;
	vec_set ( &vecPos, vector( cmmemberMe->tab, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x-cmmemberMe->tab-1, cmmemberMe->size_y, 0 ) );
	bmap_rendertarget ( cmenuMe->panel->bmap, 0, 1 );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );
	vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+1, 0 ) );
	vec_set ( &vecSize, vector ( slide_x, cmmemberMe->size_y-2, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );
	vecPos.x += slider->size_x;
	vecPos.y += cmmemberMe->size_y - 2;
	draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
	draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
	vecPos.x -= slider->size_x;
	draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
	draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
	
	vec_set ( &vecPos, vector ( cmmemberMe->tab+slide_x, cmmemberMe->pos_y+1, 0 ) );
	draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	vecPos.y += cmmemberMe->size_y-2;
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	
	bmap_rendertarget ( NULL, 0, 0 );
}

void evnCMSlider ()
{
	CMMEMBER *cmmember = cmmemberMe;
	CMSLIDER *slider = cmmemberMe->child;
	var old_value = 0;
	while ( mouse_left )
	{
		old_value = *slider->value;
		*slider->value = clamp ( mouse_pos.x - ( cmenuMe->panel->pos_x + cmmember->tab ), 0, slider->size_x );
		*slider->value /= slider->size_x;
		*slider->value *= slider->range;
		if ( slider->step != 0 )
			*slider->value = integer ( *slider->value / slider->step ) * slider->step;
		*slider->value += slider->min;
		if ( old_value != *slider->value )
		{
			cmmemberMe = cmmember;
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
	
	cmmemberMe->flags = CM_ACTIVE | CM_FREE;
	cmmemberMe->event = evnCMSlider;
	cmmemberMe->resize = fncCMSliderResize;
	cmmemberMe->draw = drwCMSlider;
	cmmemberMe->count = 0;
	cmmemberMe->child = slider;
	fncCMSliderResize ();
}
