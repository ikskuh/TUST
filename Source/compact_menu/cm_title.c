
void fncCMTitleResize ()
{
	cmmemberMe->size_y = cmmemberMe->count + 2;
	if ( str_len ( cmmemberMe->name ) > 0 )
		cmmemberMe->size_y += cmenuMe->style->font->dy;
}

//	void evnCMTitle ()
//	{
//		VECTOR oldMousePos, oldPanelPos, vecTemp;
//		PANEL *panel = cmenuMe->panel;
//		vec_set ( &oldMousePos, &mouse_pos );
//		vec_set ( &oldPanelPos, &panel->pos_x );
//		while ( mouse_left )
//		{
//			vec_diff ( &vecTemp, &mouse_pos, &oldMousePos );
//			vec_add ( &vecTemp, &oldPanelPos );
//			panel->pos_x = vecTemp.x;
//			panel->pos_y = vecTemp.y;
//			wait(1);
//		}
//	}

void drwCMTitle ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );
	vecPos.x = cmmemberMe->tab + 1;
	vecPos.y = cmmemberMe->pos_y + cmmemberMe->size_y - 1;
	draw_line ( &vecPos, colCMOver, 0 );
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.x = cmenuMe->panel->size_x;
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.y -= 1;
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.x = cmmemberMe->tab + 1;
	draw_line ( &vecPos, colCMOver, 100 );
	draw_line ( &vecPos, colCMOver, 0 );
	
	cmmember_draw_name ();
}

CMCLASS cmclassTitle;

void fncCMTitle_startup ()
{
	cmclassTitle.event = NULL;
	cmclassTitle.draw = drwCMTitle;
	cmclassTitle.resize = fncCMTitleResize;
	cmclassTitle.remove = NULL;
}

void titleCMTypeCreate( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	cmmemberMe->flags = NULL;
	cmmemberMe->class = &cmclassTitle;
	cmmemberMe->count = size_y;
	cmmemberMe->child = NULL;
	cmmemberMe->tab -= CM_TAB_STEP;
	
	fncCMTitleResize ();
}

