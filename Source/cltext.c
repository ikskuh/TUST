// CLTEXT.C ////////////////////////////////////////////////////////////////////

#ifndef __CLTEXT_C
#define __CLTEXT_C

#include <strio.c>
#include <cltext.h>

//////////////////
// Implementation


// for internal use

var 	cltext_valid_cltag(STRING* text, var pos)
{
	if (pos > (str_len(text)-10)) return 5;
	
	var res = 0;
	if ((text.chars)[pos+0] != '[') res += 1;
	if ((text.chars)[pos+1] != 'c') res += 1;
	if ((text.chars)[pos+2] != 'l') res += 1;
	if ((text.chars)[pos+9] != ']') res += 1;
	return res;
}


var 	cltext_search_tag(STRING* text, var from)
{
	var i;
	for (i = from; i < (str_len(text)-10); i++)
	{
		if (cltext_valid_cltag(text, i) == 0) return i;
	}
	return -1;
}


var 	cltext_search_space_backw(STRING* str, var from)
{
	var i;
	for (i=from-1; i > 0; i--)
	{
		if (str_getchr(str,i) == 32) return i;
	}
	return 0;
}



// for external use

var 	cltext_hextovar(STRING* hexs, var* i)
{
   var hexl = str_len(hexs);
   
   if (hexl <= -1)
      return -1;        // no string
   if (hexl > 2)
      return -2;        // too long string
   
   var c;
   char hex[2];
   for (c=0;c<hexl;c++)
      hex[c] = (hexs.chars)[c];
   
   var factor=1;
   var h = 0;
   
   for (c=hexl-1;c>=0;c--)
   {
      if ((hex[c] >= 48) && (hex[c] <= 57))         // 0 - 9
         h += (hex[c] - 48) * factor;
      else if ((hex[c] >= 65) && (hex[c] <= 70))    // A - F
         h += (hex[c] - 65 + 10) * factor;
      else if ((hex[c] >= 97) && (hex[c] <= 102))   // a - f
         h += (hex[c] - 97 + 10) * factor;
      else
         return -3;     // invalid char
      
      factor *= 16;
   }
   
   *i = h;
   
   return 0;            // success
}


var 	str_width_hack(STRING* str, FONT* font)
// This is a "hacked" version of the str_width function. The original does not
// take spaces at the end in account, this version does by adding a dot at the
// end and then removing the width of the dot again.
{
	STRING* wrk = str_create(str);
	str_cat(wrk,".");
	var strwidth = str_width(wrk,font) - str_width(".",font);
	ptr_remove(wrk);
	return strwidth;
}



var 	cltext_removetags(STRING* str)
{
	STRING* work = str_create("");
	STRING* newstr = str_create("");
	var i=0, count = 0;
	
	do
	{
		i = cltext_search_tag(str, i);
		if (i > -1)
		{
			if (i > 0)
				str_cut(newstr, str, 0, i);
			else
				str_cpy(newstr, "");
			str_cut(work, str, i+11, 0);
			str_cat(newstr,work);
			str_cpy(str,newstr);
			count += 1;
		}			
	} while (i > -1);
	ptr_remove(newstr);
	ptr_remove(work);
	return count;
}

CLTEXT* cltext_create(STRING* text, FONT* font, var indent, var sizex, var flags, COLOR* defaultcolor)
{
	if (sizex < (indent+1)) return NULL;
	
	CLTEXT* cltext = sys_malloc(sizeof(CLTEXT));
	cltext.label = str_create(text);
	cltext.texts = NULL;
	cltext.font = font;
	cltext.indent = indent;
	cltext.sizex = sizex;
	cltext.flags = flags;
	
	// First element
	CLLIST* current = sys_malloc(sizeof(CLLIST));
	current.element = NULL;
	current.next = NULL;
	cltext.texts = current;
	
	STRING* newline = str_create("");
	STRING* curline = str_create("");
	STRING* worktext = str_create("");
	var found = -1, cutstart = 0, cutend = -1, text_len = str_len(text);
	
	while (cutstart < text_len)
	{
		current.element = txt_create(1,0);
		
		found = cltext_search_tag(text, cutstart);
		if (found == cutstart)
		{	// Tagged Text
			cutstart = found + 11;
			cutend = cltext_search_tag(text, cutstart);
			if (cutend < cutstart) cutend = text_len;
			
			str_cut(worktext, text, found+4, found+5);
			cltext_hextovar(worktext, current.element.red);
			str_cut(worktext, text, found+6, found+7);
			cltext_hextovar(worktext, current.element.green);
			str_cut(worktext, text, found+8, found+9);
			cltext_hextovar(worktext, current.element.blue);
		}
		else if (found > cutstart)
		{	// Untagged text
			cutend = found;
			vec_set(current.element.blue, defaultcolor);
		}
		else
		{	// No Tags found
			cutend = text_len;
		}
		
		str_cut((current.element.pstring)[0], text, cutstart, cutend);
		
		// If the text in the line is too long, it has to be wrapped
		while ( (current.linex + str_width((current.element.pstring)[0], font)) > sizex )
		{
			var current_len = str_len((current.element.pstring)[0]);
			var validspace = 0, possiblespace = 0;
			var lastspace = current_len;
			while ((lastspace > 0) && (validspace == 0))
			{
				lastspace = cltext_search_space_backw((current.element.pstring)[0], lastspace);
				if (lastspace > 0)
				{
					possiblespace = lastspace;
					str_cut(worktext, (current.element.pstring)[0], 0, possiblespace);
					if (str_width(worktext,font)+current.linex <= sizex)
					{
						validspace = lastspace;
					}
				}
			}

			// we now know if there is a valid space and where it is
			if (validspace > 0)
			{	// great, we have a space to lf
				str_cut(curline, (current.element.pstring)[0], 0, validspace);
				str_cut(newline, (current.element.pstring)[0], validspace+1, current_len);
			}
			else
			{	// too bad, there is no valid space
				if ( ((current.linex == indent) && (current.liney > 0)) || ((current.linex == 0) && (current.liney == 0)) )
				{	// text at the beginning of the line
					if (possiblespace > 0)
					{	// just use the next space possible
						str_cut(curline, (current.element.pstring)[0], 0, possiblespace);
						str_cut(newline, (current.element.pstring)[0], possiblespace+1, current_len);
					}
					else
					{	// no chance for a lf
						str_cpy(curline, (current.element.pstring)[0]);
						str_cpy(newline, "");
					}
				}
				else
				{	// text at the end of the line, so just lf it.
					str_cpy(curline, "");
					str_cpy(newline, (current.element.pstring)[0]);
				}
			}

			// so time for a new line
			current.next = sys_malloc(sizeof(CLLIST));
			current.next.next = NULL;
			current.next.element = txt_create(1,0);
			current.next.linex = indent;
			current.next.liney = current.liney + font.dy;
			vec_set(current.next.element.blue, current.element.blue);
			
			str_cpy((current.element.pstring)[0], curline);
			str_cpy((current.next.element.pstring)[0], newline);
			
			current.width = str_width_hack((current.element.pstring)[0], font);
			current = current.next;
		}
		
		// Create new entry for next cycle
		current.next = sys_malloc(sizeof(CLLIST));
		current.next.linex = current.linex + str_width_hack((current.element.pstring)[0], font);
		current.next.liney = current.liney;
		current.next.element = NULL;
		current.next.width = 0;
		current.width = current.next.linex - current.linex;
		current.next.next = NULL;
		current = current.next;
		
		cutstart = cutend;
		
	}
	ptr_remove(worktext);
	ptr_remove(curline);
	ptr_remove(newline);
	return cltext;
}

var 	cltext_getheight(CLTEXT* cltext)
{
	CLLIST* current = cltext.texts;
	var maxliney = 0;
	if (cltext)
	{
		while (current != NULL)
		{
			maxliney = maxv(maxliney, current.liney);
			current = current.next;
		}
		if (cltext.font)
			maxliney += cltext.font.dy;
		else
			maxliney = 0;
	}
	return maxliney;
}
		

void* 	cltext_remove(CLTEXT* cltext)
{
	CLLIST* temp;
	CLLIST* current = cltext.texts;
	

	while (current != NULL)
	{
		if (current.element != NULL)
		{
			ptr_remove((current.element.pstring)[0]);
			ptr_remove(current.element);
		}
		temp = current;
		current = current.next;
		sys_free(temp);
	}
		

	ptr_remove(cltext.label);
	sys_free(cltext);
	return NULL;
}

void 	cltext_show(CLTEXT* cltext, var posx, var posy, var alpha, var layer)
{
	CLLIST* current = cltext.texts;
	cltext.posx = posx;
	cltext.posy = posy;

	do
	{
		current.element.pos_x = posx + current.linex;
		current.element.pos_y = posy + current.liney;
		current.element.font = cltext.font;
		layer_sort(current.element,layer);
		current.element.alpha = clamp(alpha,0,100);
		if (alpha == 0)
		{
			reset(current.element, SHOW | LIGHT | TRANSLUCENT);
		}
		else if (alpha == 100)
		{
			reset(current.element, TRANSLUCENT);
			set(current.element, LIGHT | SHOW);
		}
		else
		{
			set(current.element, LIGHT | SHOW | TRANSLUCENT);
		}
		if (current.next != NULL) current = current.next;	
	} while (current.next != NULL);
}

void 	cltext_align(CLTEXT* cltext, var factor)
{
	factor = clamp(factor, 0, 1);
	var i;
	CLLIST* from = cltext.texts;
	CLLIST* to = NULL;
	CLLIST* current = cltext.texts;
	var currline = 0;
	var offset = 0;
	var width = 0;
	while (current)
	{
		currline = current.liney;
		if (current.next)
		{
			if (current.next.liney > currline)
			{
				to = current;
			}
		}
		else
		{
			to = current;
		}

		if (to)
		{
			if (to.element)
			{
				width = str_width((to.element.pstring)[0], cltext.font);
			}
			else
			{
				width = to.width;
			}
			offset = (cltext.sizex - (to.linex + width)) * factor;
			
			if (currline > 0)
				offset -= cltext.indent * factor;
			
			
			current = from;
			while (current != to)
			{
				if (current.element) current.element.pos_x = current.linex + offset + cltext.posx;
				current = current.next;
			}
			if (current.element) current.element.pos_x = current.linex + offset + cltext.posx;
			
			from = to.next;
			current = to;
			to = NULL;
		}
		current = current.next;
	}
}

#endif

// CLTEXT.C /////////////////////////////////////////////////////////// E O F //