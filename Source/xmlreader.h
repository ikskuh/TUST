//Created by: Firoball

#ifndef XMLREADER_H
#define XMLREADER_H

#include "list.h"

// This define can be evaluated to check if this module is included and active
#define XMLREADER_ACTIVE


// These constants are used as identifiers in certain functions.
#define XML_NOTAG -1
#define XML_CONTENT 0
#define XML_OPENTAG 1
#define XML_CLOSETAG 2
#define XML_EOF 255
#define EOF 255


//  Definition of a XML parameter. 
//  Each parameter (tag) found in a XML file is represented by this structure.
//  Use the xml_tag_create function for creation of a new XML parameter.
typedef struct XmlTag
{
	STRING* strTag;			// String pointer to tag name
	STRING* strContent;		// String pointer to tag content
	List* psTagList;		// Pointer to a list of sub tags
	List* psAttribList;		// Pointer to a list of tag attributes
	struct XmlTag* psParent;
}XmlTag;

// Definition of a XML file. 
// For reading data from an XML file, this object is required.
// Use the XmlFile_create function for creation of a new XML file.
typedef struct
{
	var vFHndl;			// File handle (Acknex format)
	STRING* vFName;		// String pointer to xml filename
	XmlTag* psAnchor;	// Pointer to root XML parameter ("XML_root")
		
	// temp variables
	char cTmp[2];		// For temporary use
	int iTmp;			// For temporary use
}XmlFile;

// Definition of a XML attribute. 
//  Each attribute  of a XML tag is represented by this structure.
//  Use the XmlAttributeute_create function for creation of a new XML attribute.
typedef struct
{
	STRING* strTag;		// String pointer to attribute name
	STRING* strContent;	// String pointer to attribute content
	XmlTag* psParent;	// Pointer to parent XML parameter (tag)
}XmlAttribute;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! Open new xml file.
 *  \param	pcFile Pointer: full name of xml file
 *  \return	Pointer to XmlFile
 */
XmlFile* xml_file_create(char* pcFile);

/*! Removes XmlFile from memory. Delete all allocated content.
 *  \param	psHost Pointer to XmlFile to be deleted
 */
void xml_file_remove(XmlFile* psHost);

/*! Read XML file into memory
 *  \param	psHost Pointer to XmlFile to be parsed
 *  \return	Pointer to root element of XML list
 */
XmlTag* xml_file_parse(XmlFile* psHost);


/*! Creates new XmlTag
 *  \return	Pointer to XmlTag
 */
XmlTag* xml_tag_create();

/*! Removes XmlTag from memory. Delete all allocated content.
 *  \param	psHost Pointer to XmlTag to be deleted
 */
void xml_tag_remove(XmlTag* psHost);

/*! Copy tag content to specified string.
 *  \param	psHost Pointer to XmlTag to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void xml_tag_get_content(XmlTag* psHost, STRING* strTarget);

/*! Copy tag name to specified string.
 *  \param	psHost Pointer to XmlTag to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void xml_tag_get_tag(XmlTag* psHost, STRING* strTarget);

/*! Retrieve pointer to tag content.
 *  \param	psHost Pointer to XmlTag to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* xml_tag_get_pcontent(XmlTag* psHost);

/*! Retrieve pointer to tag name.
 *  \param	psHost Pointer to XmlTag to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* xml_tag_get_ptag(XmlTag* psHost);

/*! Find specified tag inside parent XML parameter.	
 *  \param	psHost Pointer to parent XmlTag
 *  \param	strTag String pointer: name of xml tag
 *  \return	Pointer to found XmlTag or NULL
 */
XmlTag* xml_tag_get_element_by_tag(XmlTag* psHost, STRING* strTag);

/*! Find specified tag inside parent XML parameter.	
 *  \param	psHost Pointer to parent XmlTag
 *  \param	iIndex Index of xml tag
 *  \return	Pointer to found XmlTag or NULL
 */
XmlTag* xml_tag_get_element_by_index(XmlTag* psHost, int iIndex);

/*! Retrieve number of sub tags of XML parameter.	
 *  \param	psHost Pointer to XmlTag to be accessed
 *  \return	Number of sub tags
 */
int xml_tag_get_tag_elements(XmlTag* psHost);

/*! Retrieve number of attributes of XML parameter.
 *  \param	psHost Pointer to XmlTag to be accessed
 *  \return	Number of attributes
 */
int xml_tag_get_attribute_elements(XmlTag* psHost);

/*! Creates new XML attribute.
 *  \return	Pointer to XmlAttributeute
 */
XmlAttribute* xml_attribute_create();

/*! Removes XML attribute from memory. Delete all allocated content.
 *  \param	psHost Pointer to XmlAttributeute to be deleted
 */
void xml_attribute_remove(XmlAttribute* psHost);

/*! Copy tag content to specified string.
 *  \param	psHost XmlAttribute to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void xml_attribute_get_content(XmlAttribute* psHost, STRING* strTarget);

/*! Copy tag name to specified string.
 *  \param	psHost XmlAttribute to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void xml_attribute_get_attribute(XmlAttribute* psHost, STRING* strTarget);

/*! Retrieve pointer to attribute content.
 *  \param	psHost Pointer to XmlAttribute to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* xml_attribute_get_pcontent(XmlAttribute* psHost);

/*! Retrieve pointer to attribute name.
 *  \param	psHost Pointer to XmlAttribute to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* xml_attribute_get_pattribute(XmlAttribute* psHost);

/*! Find specified attribute inside parent XML parameter.	
 *  \param	psHost Pointer to parent XmlTag
 *  \param	strTag String pointer: name of xml attribute
 *  \return	Pointer to found XmlAttribute or NULL
 */
XmlAttribute* xml_attribute_get_elements_by_attribute(XmlTag* psHost, STRING* strTag);

/*! Find specified attribute inside parent XML parameter.
 *  \param	psHost Pointer to parent XmlTag
 *  \param	iIndex Index of xml attribute
 *  \return	Pointer to found XmlAttribute or NULL
 */
XmlAttribute* xml_attribute_get_elements_by_index(XmlTag* psHost, int iIndex);


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
int xml_file_read(XmlFile* psHost, XmlTag* parentPar, int ntag);
/*! \internal - Do not use! */
void xml_file_read_tag_attribute(XmlFile* psHost, XmlTag* par);
/*! \internal - Do not use! */
int xml_file_read_open_tag(XmlFile* psHost, XmlTag* par);
/*! \internal - Do not use! */
void xml_file_read_close_tag(XmlFile* psHost);
/*! \internal - Do not use! */
void xml_file_read_content(XmlFile* psHost, XmlTag* par);
/*! \internal - Do not use! */
int xml_file_find_next(XmlFile* psHost);

//Added by padmalcom
int xml_tag_compare_attributes(List* attr1, List* attr2);
int xml_tag_get_index_by_element(XmlTag* psHost, XmlTag* element);

#include "xmlreader.c"

#endif