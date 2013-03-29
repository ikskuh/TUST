//Created by: Firoball

XmlFile* xml_file_create(char* pcFile)
{
	XmlFile* psHost;
	psHost = (XmlFile*)malloc(sizeof(XmlFile));
	psHost->vFName = str_create(pcFile);
	psHost->vFHndl = NULL;
	psHost->psAnchor = NULL;
		
	return (psHost);
}

void xml_file_remove(XmlFile* psHost)
{
	int i;

	/* remove all allocated instances */
	str_remove(psHost->vFName);
	if (psHost->psAnchor != NULL)
	{
		xml_tag_remove(psHost->psAnchor);	
	}
	free (psHost);
}

XmlTag* xml_file_parse(XmlFile* psHost)
{
	psHost->vFHndl = file_open_game(psHost->vFName);
	/* only read file if filename is valid and psAnchor is not yet allocated */
	if ((psHost->vFHndl != NULL) && (psHost->psAnchor == NULL))
	{
		/* create psAnchor XmlTag */
		psHost->psAnchor = xml_tag_create();
		psHost->psAnchor->strTag = str_create("XML_root");
		psHost->iTmp = xml_file_find_next(psHost);
		do
		{
			psHost->iTmp = xml_file_read(psHost, psHost->psAnchor, psHost->iTmp);
		}while (psHost->iTmp == XML_OPENTAG);

		file_close(psHost->vFHndl);
		psHost->vFHndl = NULL;		
	}
	return (psHost->psAnchor);
}


/* ----- XmlTag ----- */
XmlTag* xml_tag_create()
{
	XmlTag* psHost;
	psHost = (XmlTag*)malloc(sizeof(XmlTag));
	psHost->psTagList = NULL;
	psHost->psAttribList = NULL;
	psHost->strTag = NULL;
	psHost->strContent = NULL;
	psHost->psParent = NULL; 
	
	return (psHost);
}

void xml_tag_remove(XmlTag* psHost)
{
	int i;
	str_remove(psHost->strTag);
	/* single par - remove content */
	if (psHost->strContent != NULL)
		str_remove(psHost->strContent);

	/* remove all XmlTags on list */
	if (psHost->psTagList != NULL)
	{
		for (i = 0; i < list_get_count(psHost->psTagList); i++)
			xml_tag_remove((XmlTag*)list_item_at(psHost->psTagList, i));
		list_delete(psHost->psTagList);	
	}
	
	/* remove Attribute list */
	if (psHost->psAttribList !=NULL)
	{
		for (i = 0; i < list_get_count(psHost->psAttribList); i++)
			xml_attribute_remove((XmlAttribute*)list_item_at(psHost->psAttribList, i));
		list_delete(psHost->psAttribList);	
	}

	free(psHost);
}

void xml_tag_get_content(XmlTag* psHost, STRING* strTarget)
{
	if (psHost->strContent == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strContent);
}

void xml_tag_get_tag(XmlTag* psHost, STRING* strTarget)
{
	if (psHost->strTag == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strTag);
}

STRING* xml_tag_get_pcontent(XmlTag* psHost)
{
	if (psHost->strContent != NULL)
		return (psHost->strContent);
	else
		return (NULL);
}

STRING* xml_tag_get_ptag(XmlTag* psHost)
{
	if (psHost->strTag != NULL)
		return (psHost->strTag);
	else
		return (NULL);
}

XmlTag* xml_tag_get_element_by_tag(XmlTag* psHost, STRING* strTag)
{
	int i;
	XmlTag* pXml;
	
	pXml = NULL;
	
	if (psHost->strTag != NULL)
	{
		/* at least one sub parameter found */
		if ((psHost->psTagList != NULL) && (psHost->strContent == NULL))
		{
			for (i = 0; i < list_get_count(psHost->psTagList); i++)
			{
				pXml = (XmlTag*)list_item_at(psHost->psTagList, i);
				if (!str_cmp(pXml->strTag, strTag))
					/* tag not found */
					pXml = NULL;
				else
					/* tag found */
					break;
			}
		}
	}
			
	return (pXml);	
}

XmlTag* xml_tag_get_element_by_index(XmlTag* psHost, int iIndex)
{
	XmlTag* pXml;
	
	pXml = NULL;
	
	/* at least one sub parameter found */
	if (psHost->psTagList != NULL)	
	{
		/* limit index to valid range */
		//iIndex = clamp(iIndex, 0, LIST_items(psHost->psTagList) - 1);
		if (iIndex >= list_get_count(psHost->psTagList)) {
			return (NULL);
		}
		pXml = (XmlTag*)list_item_at(psHost->psTagList, iIndex);
	}
	return (pXml);
}

// Added by padmalcom
int xml_tag_get_index_by_element(XmlTag* psHost, XmlTag* element)
{
	int i;
	if (psHost->psTagList != NULL)
	{
		for (i = 0; i < list_get_count(psHost->psTagList); i++)
		{
			XmlTag* pXml = (XmlTag*)list_item_at(psHost->psTagList,i);
			
			if ((str_cmp(pXml->strTag,element->strTag) == 1) &&
				 (str_cmp(pXml->strContent,element->strContent) == 1) &&
				 (xml_tag_compare_attributes(pXml->psAttribList, element->psAttribList) == 1))
			{
			 	return i;
			}	
		}
		return -1;
	}
}

int xml_tag_compare_attributes(List* attr1, List* attr2)
{
	if (list_get_count(attr1) != list_get_count(attr2)) return 0;
	
	int i;
	XmlTag* pXml1 = NULL;
	XmlTag* pXml2 = NULL;
	
	for (i = 0; i < list_get_count(attr1); i++)
	{
		pXml1 = (XmlAttribute*)list_item_at(attr1, i);
		pXml2 = (XmlAttribute*)list_item_at(attr2, i);
			
		if (!str_cmp(pXml1->strTag, pXml2->strTag))
			return 0;
	}
	return 1;
}

int xml_tag_get_tag_elements(XmlTag* psHost)
{
	if (psHost->psTagList != NULL)
		return (list_get_count(psHost->psTagList));
	else
		return (0);	
}

int xml_tag_get_attribute_elements(XmlTag* psHost)
{
	if (psHost->psAttribList != NULL)
		return (list_get_count(psHost->psAttribList));	
	else
		return (0);	
}


/* ----- XmlAttribute ----- */
XmlAttribute* xml_attribute_create()
{
	XmlAttribute* psHost;
	psHost = (XmlAttribute*)malloc(sizeof(XmlAttribute));
	psHost->strTag = NULL;
	psHost->strContent = NULL;
	
	return (psHost);
}

void xml_attribute_remove(XmlAttribute* psHost)
{
	str_remove(psHost->strTag);
	str_remove(psHost->strContent);

	free(psHost);
}

XmlAttribute* xml_attribute_get_elements_by_attribute(XmlTag* psHost, STRING* strTag)
{
	int i;
	XmlAttribute* pXml;
	
	pXml = NULL;
	
	/* at least one sub parameter found */
	if (psHost->psAttribList != NULL)
	{
		for (i = 0; i < list_get_count(psHost->psAttribList); i++)
		{
			pXml = (XmlAttribute*)list_item_at(psHost->psAttribList, i);
			if (!str_cmp(pXml->strTag, strTag))
				/* tag not found */
				pXml = NULL;
			else
				/* tag found */
				break;
		}
	}
			
	return (pXml);	
}

XmlAttribute* xml_attribute_get_elements_by_index(XmlTag* psHost, int iIndex)
{
	XmlAttribute* pXml;
	
	pXml = NULL;
	
	/* at least one sub parameter found */
	if (psHost->psAttribList != NULL)	
	{
		/* limit index to valid range */
		iIndex = clamp(iIndex, 0, list_get_count(psHost->psAttribList) - 1);
		pXml = (XmlAttribute*)list_item_at(psHost->psAttribList, iIndex);
	}
	return (pXml);
}

void xml_attribute_get_content(XmlAttribute* psHost, STRING* strTarget)
{
	if (psHost->strContent == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strContent);
}

void xml_attribute_get_attribute(XmlAttribute* psHost, STRING* strTarget)
{
	if (psHost->strTag == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strTag);
}

STRING* xml_attribute_get_pcontent(XmlAttribute* psHost)
{
	if (psHost->strContent != NULL)
		return (psHost->strContent);
	else
		return (NULL);
}

STRING* xml_attribute_get_pattribute(XmlAttribute* psHost)
{
	if (psHost->strTag != NULL)
		return (psHost->strTag);
	else
		return (NULL);
}


/* ----- INTERNAL FUNCTIONS ----- */


/* ----- XmlFile ----- */
int xml_file_read(XmlFile* psHost, XmlTag* parentPar, int ntag)
{
	/* create new XmlTag and add it to list of parent */
	XmlTag* par; 
	par = xml_tag_create();
	par->psParent = parentPar;
	if (parentPar->psTagList == NULL)
		parentPar->psTagList = list_create();
	list_add(parentPar->psTagList, (void*)par);

	if(ntag == XML_OPENTAG)
	{
		if (xml_file_read_open_tag(psHost, par))
		{
			/* no content was found */
			par->strContent = str_create("");
			par->psTagList = NULL; 
			ntag = xml_file_find_next(psHost);
		}
		else
		{
			ntag = xml_file_find_next(psHost);
			switch(ntag)
			{
				/* NOTE: readContent and read should be changed so text and tag mixtures can be read as well */
				case XML_CONTENT:
					xml_file_read_content(psHost, par);
					ntag = xml_file_find_next(psHost);
					break;
							
				case XML_OPENTAG:
					do
					{
						ntag = xml_file_read(psHost, par, ntag);
					}while(ntag == XML_OPENTAG);
					break;
							
				case XML_CLOSETAG:
					/* no content was found */
					par->strContent = str_create("");
					par->psTagList = NULL; 
					break;
							
				default:
					/* invalid file */
					ntag = XML_EOF;
					break;	
			}

			if (ntag == XML_CLOSETAG)
			{
				xml_file_read_close_tag(psHost);
				ntag = xml_file_find_next(psHost);
			}
			else
				/* invalid file */
				ntag = XML_EOF;
		}

	}
	else
		/* invalid file */
		ntag = XML_EOF;
			
	return (ntag);
}
	
void xml_file_read_tag_attribute(XmlFile* psHost, XmlTag* par)
{
	/* create new XmlAttribute and add it to list of parent */
	XmlAttribute *attrib;

	attrib = xml_attribute_create();
	attrib->psParent = par;
	attrib->strTag = str_create("");
	attrib->strContent = str_create("");
	if (par->psAttribList == NULL)
		par->psAttribList = list_create();
	list_add(par->psAttribList, (void*)attrib);
	psHost->cTmp[1] = '\0';
	
	/* read attribute tag */
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		if ((psHost->cTmp[0] > 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '='))
		{
			str_cat(attrib->strTag, psHost->cTmp);
		}
	}
	while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 32) && (psHost->cTmp[0] != '=') 
		&& (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));

	/* dummy read unnecessary bytes like spaces, " and = */ 
	if ((psHost->cTmp[0] == '=') || (psHost->cTmp[0] == 32))
	{
		do
		{
			psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		}
		while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 34) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));
	}

	/* read attribute strContent */
	if (psHost->cTmp[0] == 34)	/* 34 = " */
	{
		do
		{
			psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
			if ((psHost->cTmp[0] >= 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 34))
			{
				str_cat(attrib->strContent, psHost->cTmp);
			}
		}
		while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 34) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));
	}
//	printf("attribute: [%s], content: [%s]", (attrib->strTag)->chars, (attrib->strContent)->chars);	
}

int xml_file_read_open_tag(XmlFile* psHost, XmlTag* par)
{
	par->strTag = str_create("");
	psHost->cTmp[1] = '\0';
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		/* check for valid byte and add it to tag string */
		
 		if (psHost->cTmp[0] == 32 || psHost->cTmp[0] == '\n') 
		{
			/* possible attribute tag detected */
			do
			{
				/* dummy read unnecessary bytes */
				do
				{
					psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
				}
				while((psHost->cTmp[0] != EOF) && psHost->cTmp[0] <= 32);
	
				/* check whether next character is attribute name or tag element */
				if((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'))
				{
					file_seek(psHost->vFHndl, -1, 1);
	 				xml_file_read_tag_attribute(psHost, par);
	 			}
 			}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));
		}
 		else
 		{
			if ((psHost->cTmp[0] >= 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'))
			{
				str_cat(par->strTag, psHost->cTmp);
			}
		}
	}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>')); 
//printf ("tagname: [%s]", (par->strTag)->chars);			

	switch ((int)psHost->cTmp[0])
	{
		case '/':
			do
			{
				psHost->cTmp[0]  = file_asc_read(psHost->vFHndl);
			}while((psHost->cTmp[0]  != '>') && (psHost->cTmp[0]  != EOF)); 
		
			psHost->iTmp = 1;	/* tag is empty */
			break;
			
		case EOF:
			psHost->iTmp = 1;	/* tag is empty */
			break;
		
		default:
			psHost->iTmp = 0;	/* tag has content */ 			
			break;
	}
			
	return (psHost->iTmp);
}

void xml_file_read_close_tag(XmlFile* psHost)
{
	/* just read in the closetag - no further action */
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
	}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '>')); 
}

void xml_file_read_content(XmlFile* psHost, XmlTag* par)
{
	par->strContent = str_create("");
	psHost->cTmp[1] = '\0';
	
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		/* check for valid byte and add it to tag string */
		if ((psHost->cTmp[0] >= 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '<'))
		{
			str_cat(par->strContent, psHost->cTmp);
		}			
	}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '<')); 
	str_cat(par->strContent, "\0");		
	file_seek(psHost->vFHndl, -1, 1);
	
}

int xml_file_find_next(XmlFile* psHost)
{
	psHost->iTmp = XML_NOTAG;
	
	while(psHost->iTmp == XML_NOTAG)
	{
		/* search first useful sign */
		do
		{
			psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		}while((psHost->cTmp[0] < 32) && (psHost->cTmp[0] != EOF)); 
		
		/* evaluate finding*/
		if (psHost->cTmp[0] == '<')
		{
			/* skip control bytes */
			do
			{
				psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
			}while((psHost->cTmp[0] < 32) && (psHost->cTmp[0] != EOF));
			switch ((int)psHost->cTmp[0])
			{
				case '/':	/* closing tag */
					psHost->iTmp = XML_CLOSETAG;
					break;
					
				case '!':
				case '?':	/* comment tag - skip and continue searching */
					xml_file_read_close_tag(psHost);	//ugly
					psHost->iTmp = XML_NOTAG;
					break;
					
				case EOF:	/* end of file */
					psHost->iTmp = XML_EOF;
					break;
					
				default:		/* opening tag */
					psHost->iTmp = XML_OPENTAG;	
					/* set file pointer to previous byte */
					file_seek(psHost->vFHndl, -1, 1);
					break;
			}
		}
		else
		{
			/* end of file check */
			if (psHost->iTmp != EOF)
			{
				/* tag content found */
				psHost->iTmp = XML_CONTENT;
				file_seek(psHost->vFHndl, -1, 1);
			}
			else
				psHost->iTmp = XML_EOF;
		}
	}
	
	return (psHost->iTmp);
}