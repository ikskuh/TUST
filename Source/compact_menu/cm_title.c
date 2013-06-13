
void fncCMTitleResize ()
{
	cmmemberMe->size_y = cmmemberMe->count + 2;
	if ( str_len ( (cmmemberMe->text->pstring)[cmmemberMe->index] ) > 0 )
		cmmemberMe->size_y += cmenuMe->style->font->dy;
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
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colOver, 100, 0 );
	draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	vecPos.x = cmenuMe->panel->size_x;
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	vecPos.y += 1;
	draw_line ( &vecPos, cmenuMe->style->colBack, 100 );
	vecPos.x = -1;
	draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
	draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y+cmmemberMe->size_y-1, 0 ) );
	draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	vecPos.x = cmenuMe->panel->size_x;
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	vecPos.y -= 1;
	draw_line ( &vecPos, cmenuMe->style->colBack, 100 );
	vecPos.x = -1;
	draw_line ( &vecPos, cmenuMe->style->colText, 100 );
	draw_line ( &vecPos, cmenuMe->style->colText, 0 );
	
	cmmember_draw_name ();
//	VECTOR vecOffset;
//	vec_set ( &vecOffset, vector ( 0, cmmemberMe->index * cmenuMe->style->font->dy, 0 ) );
//	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y + cmmemberMe->count, 0 ) );
//	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
//	draw_quad ( cmmemberMe->text->target_map, &vecPos, &vecOffset, &vecSize, NULL, NULL, 100, 0 );
}

void titleCMTypeCreate( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	cmmemberMe->flags = CM_ACTIVE;
	cmmemberMe->event = evnCMTitle;
	cmmemberMe->draw = drwCMTitle;
	cmmemberMe->resize = fncCMTitleResize;
	cmmemberMe->remove = NULL;
	cmmemberMe->count = size_y;
	cmmemberMe->child = NULL;
	
	fncCMTitleResize ();
}

