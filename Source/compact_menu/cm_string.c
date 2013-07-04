
void fncCMStringResize ()
{
	cmmemberMe->size_y = cmenuMe->style->font->dy;
}

void drwCMString ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );
	STRING **cmTempStrimg = cmmemberMe->child;
	cmmember_string ( *cmTempStrimg );
	cmmember_name ();
}

CMCLASS cmclassString;

void fncCMString_startup ()
{
	cmclassString.event = NULL;
	cmclassString.draw = drwCMString;
	cmclassString.resize = fncCMStringResize;
	cmclassString.remove = NULL;
	cmclassString.select = NULL;
}

void stringCMTypeCreate ( STRING *strData )
{
	long type;
   STRING **ptrString = engine_getvar ( strData->chars,  &type );
	
	#ifdef CM_SAFE_MODE
		if ( ptrString == NULL )
		{
			str_cat ( strData, "\nstring not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	cmmemberMe->flags = NULL;
	cmmemberMe->class = &cmclassString;
	cmmemberMe->count = 0;
	cmmemberMe->child = ptrString;
	
	fncCMStringResize ();
}
