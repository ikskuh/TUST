
void drwCMSpace ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );	
	vecPos.x += cmmemberMe->tab - CM_TAB_STEP;
	vecPos.y += cmmemberMe->count;
	draw_line ( &vecPos, colCMOver, 0 );
	draw_line ( &vecPos, colCMOver, 100 );
	vecPos.x = cmenuMe->panel->size_x * CM_TAB_LINE;
	draw_line ( &vecPos, colCMBack, 100 );
	draw_line ( &vecPos, colCMBack, 0 );
}

CMCLASS cmclassSpace;

void fncCMSpace_startup ()
{
	cmclassSpace.event = NULL;
	cmclassSpace.draw = drwCMSpace;
	cmclassSpace.resize = NULL;
	cmclassSpace.remove = NULL;
	cmclassSpace.select = NULL;
}


void spaceCMTypeCreate ( STRING *strData )
{
	int size_y = str_to_int ( strCMData );
	cmmemberMe->flags = NULL;
	cmmemberMe->class = &cmclassSpace;
	cmmemberMe->count = size_y;
	cmmemberMe->size_y = size_y+1;
	cmmemberMe->child = NULL;
}

