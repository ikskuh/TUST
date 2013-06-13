
void fncCMButtonResize ()
{
	cmmemberMe->size_y = cmenuMe->style->font->dy;
}

void drwCMButton ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );
	if ( cmmemberMe == cmenuMe->cmmemberActual )
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab-1, cmmemberMe->pos_y+1, 0 ) );
		vec_set ( &vecSize, vector ( cmenuMe->panel->size_x+1-cmmemberMe->tab, cmmemberMe->size_y-1, 0 ) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );		
		vec_set ( &vecPos, vector ( cmenuMe->panel->size_x-3, cmmemberMe->pos_y+(cmmemberMe->size_y/2), 0 ) );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		vecPos.x -= 8;
		vecPos.y -= 3;
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		vecPos.y += 6;
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		vecPos.x += 8;
		vecPos.y -= 3;
		draw_line ( &vecPos, cmenuMe->style->colText, 100 );
		draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	}
	cmmember_draw_name ();
//	VECTOR vecOffset;
//	vec_set ( &vecOffset, vector ( 0, cmmemberMe->index * cmenuMe->style->font->dy, 0 ) );
//	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
//	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x-CM_TAB_SRIGHT, cmenuMe->style->font->dy, 0 ) );
//	draw_quad ( cmmemberMe->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
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
	cmmemberMe->flags = CM_ACTIVE;
	cmmemberMe->event = fncCMPrototype;
	cmmemberMe->draw = drwCMButton;
	cmmemberMe->resize = fncCMButtonResize;
	cmmemberMe->remove = NULL;
	cmmemberMe->count = 0;
	cmmemberMe->child = NULL;
	
	fncCMButtonResize ();
}

