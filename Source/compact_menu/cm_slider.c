
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

void fncCMSliderRemove ()
{
	CMSLIDER *slider = cmmemberMe->child;
	sys_free ( slider );
}

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
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );
	CMSLIDER *slider = cmmemberMe->child;
	var slide_x = *slider->value;
	slide_x -= slider->min;
	slide_x /= slider->range;
	slide_x *= slider->size_x;
	slide_x = maxv ( slide_x, 1 );
	if ( cmmemberMe == cmenuMe->cmmemberActual )
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( slide_x, cmmemberMe->size_y-2, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMOver, 100, 0 );
		vecPos.x += slider->size_x;
		vecPos.y += cmmemberMe->size_y - 2;
		draw_line ( &vecPos, colCMOver, 0 );
		draw_line ( &vecPos, colCMOver, 100 );
		vecPos.x -= slider->size_x;
		draw_line ( &vecPos, colCMOver, 100 );
		draw_line ( &vecPos, colCMOver, 0 );
		
		vec_set ( &vecPos, vector ( cmmemberMe->tab+slide_x, cmmemberMe->pos_y+1, 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.y += cmmemberMe->size_y-2;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab+slider->size_x, cmmemberMe->pos_y+cmmemberMe->size_y-1, 0 ) );
		draw_line ( &vecPos, colCMOver, 0 );
		draw_line ( &vecPos, colCMOver, 100 );
		vecPos.x -= slider->size_x * CM_TAB_LINE;
		draw_line ( &vecPos, colCMBack, 100 );
		draw_line ( &vecPos, colCMBack, 0 );
	}
	
	cmmember_draw_var ( slider->value, slider->format );
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
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );
	vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+1, 0 ) );
	vec_set ( &vecSize, vector ( slide_x, cmmemberMe->size_y-2, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMOver, 100, 0 );
	vecPos.x += slider->size_x;
	vecPos.y += cmmemberMe->size_y - 2;
	draw_line ( &vecPos, colCMOver, 0 );
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.x -= slider->size_x;
	draw_line ( &vecPos, colCMOver, 100 );
	draw_line ( &vecPos, colCMOver, 0 );
	
	vec_set ( &vecPos, vector ( cmmemberMe->tab+slide_x, cmmemberMe->pos_y+1, 0 ) );
	draw_line ( &vecPos, colCMText, 0 );
	draw_line ( &vecPos, colCMText, 100 );
	vecPos.y += cmmemberMe->size_y-2;
	draw_line ( &vecPos, colCMText, 100 );
	draw_line ( &vecPos, colCMText, 0 );
	
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

CMCLASS cmclassSlider;

void fncCMSlider_startup ()
{
	cmclassSlider.event = evnCMSlider;
	cmclassSlider.draw = drwCMSlider;
	cmclassSlider.resize = fncCMSliderResize;
	cmclassSlider.remove = fncCMSliderRemove;
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
	int iSliderFormat = clamp ( str_to_int ( strData ), 0, 3 );
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
	slider->format = (txtCMFormats->pstring)[iSliderFormat];
	slider->event = fncCMPrototype;
	
	cmmemberMe->flags = CM_ACTIVE;
	cmmemberMe->class = &cmclassSlider;
	cmmemberMe->count = 0;
	cmmemberMe->child = slider;
	fncCMSliderResize ();
}
