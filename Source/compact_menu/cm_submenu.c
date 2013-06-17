
CMCLASS cmclassSubmenu;

void fncCMSubmenuRemove ()
{
	CMMEMBER **cmmemberArray = cmmemberMe->child;
	CMMEMBER **cmmemberTemp = cmmemberArray;
	CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
	for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
	{
		cmmemberMe = *cmmemberTemp;
		if ( cmmemberMe->class->remove )
			cmmemberMe->class->remove ();
		str_remove ( (*cmmemberTemp)->name );
		sys_free ( *cmmemberTemp );
	}
	sys_free ( cmmemberArray );
}

void fncCMSubmenuResize ()
{
	cmmemberMe->size_y = cmenuMe->style->font->dy;
	
	CMMEMBER **cmmemberTemp = cmmemberMe->child;
	CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
	for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
	{
		cmmemberMe = *cmmemberTemp;
		if ( cmmemberMe->class->resize != NULL )
			cmmemberMe->class->resize ();
	}
}

void evnCMSubmenu ()
{
	cmmemberMe->flags ^= CM_OPENED;
	if ( cmmemberMe->flags & CM_OPENED )
	{
		CMMEMBER **cmmemberTemp = cmmemberMe->child;
		CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
		for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
			(*cmmemberTemp)->flags &= ~CM_OPENED;
		cmmemberTemp = cmmemberMe->parent->child;
		cmmemberTempLast = cmmemberTemp + cmmemberMe->parent->count;
		for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp++ )
		{
			if ( *cmmemberTemp == cmmemberMe )
				continue;
			(*cmmemberTemp)->flags &= ~CM_OPENED;
		}
	}
	cmmemberMe->flags |= CM_RESIZE;
}

void drwCMSubmenu ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, vector ( 0, cmmemberMe->pos_y, 0 ) );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );
	VECTOR vecOffset;
	if ( cmmemberMe->flags & CM_OPENED )
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->pos_y+cmmemberMe->size_y-3, 0 ) );
		vec_set ( &vecSize, vector(cmenuMe->panel->size_x-vecPos.x,3,0) );
		draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMOver, 100, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab+2, cmmemberMe->pos_y+(cmmemberMe->size_y/2)-2, 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.y += 5;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
		vec_set ( &vecPos, vector ( cmmemberMe->tab+1, cmmemberMe->pos_y+(cmmemberMe->size_y/2), 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x += 3;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
	}
	
	cmmember_name ();
	
	if ( cmmemberMe->flags & CM_OPENED )
	{
		var old_position = cmenuMe->panel->size_y;
		VECTOR vecPos;
		var nMouseY = mouse_pos.y - cmenuMe->panel->pos_y;
		CMMEMBER *parent = cmmemberMe;
		CMMEMBER **cmmemberTemp = cmmemberMe->child;
		CMMEMBER **cmmemberTempLast = cmmemberTemp + cmmemberMe->count;
		for ( ; cmmemberTemp<cmmemberTempLast; cmmemberTemp ++ )
		{
			cmmemberMe = *cmmemberTemp;
			cmmemberMe->pos_y = cmenuMe->panel->size_y;
			if ( cmmemberMe->flags & CM_INVISIBLE )
				continue;
			
			if ( cmmemberMe->flags & CM_RESIZE )
				cmmemberMe->class->resize ();
			
			cmenuMe->panel->size_y += cmmemberMe->size_y;
			
			cmmemberMe->next = NULL;
			cmmemberPrev->next = cmmemberMe;
			cmmemberMe->prev = cmmemberPrev;
			cmmemberPrev = cmmemberMe;
			
			cmmemberMe->class->draw ();
			
			vec_set ( &vecPos, vector ( parent->tab, old_position, 0 ) );
			draw_line ( &vecPos, colCMText, 0 );
			draw_line ( &vecPos, colCMText, 100 );
			vecPos.y = cmenuMe->panel->size_y;
			draw_line ( &vecPos, colCMText, 100 );
			draw_line ( &vecPos, colCMText, 0 );
		}
	}
}


void drwCMSubmenuSelect ()
{
	VECTOR vecPos;
	vec_set ( &vecPos, nullvector );
	VECTOR vecSize;
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x, cmmemberMe->size_y, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMBack, 100, 0 );
	vec_set ( &vecPos, vector ( cmmemberMe->tab, cmmemberMe->size_y-3, 0 ) );
	vec_set ( &vecSize, vector ( cmenuMe->panel->size_x - vecPos.x, 3, 0 ) );
	draw_quad ( NULL, &vecPos, NULL, &vecSize, NULL, colCMOver, 100, 0 );
	if ( cmmemberMe->flags & CM_OPENED )
	{
		vec_set ( &vecPos, vector ( cmenuMe->panel->size_x-1-CM_TAB_RIGHT, cmmemberMe->size_y-4, 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x -= 3;
		vecPos.y -= 6;
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x -= 3;
		vecPos.y += 6;
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x += 6;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
	}
	else
	{
		vec_set ( &vecPos, vector ( cmmemberMe->tab+2, (cmmemberMe->size_y/2)-2, 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.y += 5;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
		vec_set ( &vecPos, vector ( cmmemberMe->tab+1, (cmmemberMe->size_y/2), 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x += 3;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
		
		vec_set ( &vecPos, vector ( cmenuMe->panel->size_x-1-CM_TAB_RIGHT, cmmemberMe->size_y-9, 0 ) );
		draw_line ( &vecPos, colCMText, 0 );
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x -= 3;
		vecPos.y += 6;
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x -= 3;
		vecPos.y -= 6;
		draw_line ( &vecPos, colCMText, 100 );
		vecPos.x += 6;
		draw_line ( &vecPos, colCMText, 100 );
		draw_line ( &vecPos, colCMText, 0 );
	}
}

void fncCMSubmenu_startup ()
{
	cmclassSubmenu.event = evnCMSubmenu;
	cmclassSubmenu.draw = drwCMSubmenu;
	cmclassSubmenu.resize = fncCMSubmenuResize;
	cmclassSubmenu.remove = fncCMSubmenuRemove;
	cmclassSubmenu.select = drwCMSubmenuSelect;
}

void submenuCMTypeCreate ( STRING *strData )
{
	TEXT *txtSub = var_for_name ( strData );
	#ifdef CM_SAFE_MODE
		if ( txtSub == NULL )
		{
			str_cat ( strData, "\nTEXT struct not found" );
			error ( strData );
			sys_exit ( NULL );
		}
	#endif
	
	if ( cmmemberMe->parent != NULL )
		cmmemberMe->flags = CM_ACTIVE;
	else
		cmmemberMe->flags = CM_OPENED;
	
	CMMEMBER *parent = cmmemberMe;
	int iMembersCount = txtSub->strings;
	CMMEMBER **cmmembers = sys_malloc ( sizeof(CMMEMBER*) * iMembersCount );
	parent->child = cmmembers;
	int i = 0;
	for ( ; i<iMembersCount; i++ )
	{
		cmmemberMe = sys_malloc ( sizeof(CMMEMBER) );
		*(cmmembers+i) = cmmemberMe;
		cmmemberMe->name = str_create ( (txtSub->pstring)[i] );
		cmmemberMe->index = i;
		cmmemberMe->tab = parent->tab + CM_TAB_STEP;
		cmmemberMe->parent = parent;
		cmmemberCMStringParse ();
	}
	
	cmmemberMe = parent;
	cmmemberMe->class = &cmclassSubmenu;
	cmmemberMe->count = txtSub->strings;
	
	fncCMSubmenuResize ();
}

