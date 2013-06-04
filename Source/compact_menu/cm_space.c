
void drwCMSpace ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );	
	vecPos.x += cmmemberMe->tab - CM_TAB_STEP;
	vecPos.y += cmmemberMe->count;
	draw_line ( &vecPos, cmenuMe->style->colOver, 0 );
	draw_line ( &vecPos, cmenuMe->style->colOver, 100 );
	vecPos.x = cmenuMe->panel->size_x * CM_TAB_LINE;
	draw_line ( &vecPos, cmenuMe->style->colBack, 100 );
	draw_line ( &vecPos, cmenuMe->style->colBack, 0 );
}

void spaceCMTypeCreate ( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	cmmemberMe->flags = 0;
	cmmemberMe->event = NULL;
	cmmemberMe->draw = drwCMSpace;
	cmmemberMe->count = size_y;
	cmmemberMe->resize = NULL;
	cmmemberMe->size_y = size_y+1;
	cmmemberMe->child = NULL;
}

