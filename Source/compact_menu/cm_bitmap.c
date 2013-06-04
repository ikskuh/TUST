

void fncCMBitmapResize ()
{
	BMAP **bmpTemp = cmmemberMe->child;
	if ( *bmpTemp != NULL )
		cmmemberMe->size_y = ( bmap_height ( *bmpTemp ) * ( cmenuMe->panel->size_x - cmmemberMe->tab - CM_TAB_RIGHT ) ) / bmap_width ( *bmpTemp );
	else
		cmmemberMe->size_y = cmenuMe->style->font->dy;
}

void drwCMBitmap ()
{
	fncCMBitmapResize ();
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, cmenuMe->style->colBack, 100, 0 );
	BMAP **bmpTemp = cmmemberMe->child;
	if ( *bmpTemp != NULL )
	{
		vecPos.x = cmmemberMe->tab;
		var nProportion = ( cmenuMe->panel->size_x - vecPos.x - CM_TAB_RIGHT ) / bmap_width ( *bmpTemp );
		VECTOR vecScale;
		vec_set ( &vecScale, vector(nProportion,nProportion,0) );
		draw_quad ( *bmpTemp, &vecPos, NULL, vector(bmap_width(*bmpTemp),bmap_height(*bmpTemp),0), &vecScale, NULL, 100, 0 );
	}
	else
	{
		cmmember_draw_name ();
//		VECTOR vecOffset;
//		vec_set ( &vecOffset, vector ( 0, cmmemberMe->index * cmenuMe->style->font->dy, 0 ) );
//		draw_quad ( cmmemberMe->text->target_map, &vecPos, &vecOffset, vector(vecSize.x-CM_TAB_RIGHT,cmenuMe->style->font->dy,0), NULL, NULL, 100, 0 );
	}
}

void bmapCMTypeCreate ( STRING *strData )
{
	BMAP **bmapPtr = engine_getvar ( strData->chars, NULL );
	#ifdef CM_SAFE_MODE
		if ( bmapPtr == NULL )
		{
			str_cat ( strData, "\npointer to bitmap pointer not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	cmmemberMe->flags = 0;
	cmmemberMe->event = NULL;
	cmmemberMe->draw = drwCMBitmap;
	cmmemberMe->resize = fncCMBitmapResize;
	cmmemberMe->count = 0;
	cmmemberMe->child = bmapPtr;
	str_cpy ( (cmmemberMe->text->pstring)[cmmemberMe->index], "NULL" );
	
	fncCMBitmapResize ();
}

