/* 
 * Motif Tools Library, Version 2.0
 * $Id: XmStringCvt.c,v 2.19 1995/05/05 18:12:32 david Exp $
 * 
 * Written by David Flanagan.
 * Copyright (c) 1992, 1993, 1994 by Dovetail Systems.
 * All Rights Reserved.	 See the file COPYRIGHT for details.
 * This is not free software.  See the file SHAREWARE for details.
 * There is no warranty for this software.  See NO_WARRANTY for details.
 */

#include <Xmt/Xmt.h>
#include <Xmt/ConvertersP.h>

#if XmVersion >= 2000
/**
 * Gary Merrill figured out how XmString parsing works in Motif 2.0, and
 * how rendition tables and tabs are handled, and wrote this code for
 * Motif 2.0.  David Flanagan tweaked the resulting code a bit.
 *
 * The following escape sequences are supported for strings in the
 * resource files:
 *
 *	    @t		   tab (relative to a tab list)
 *	    @f[tag]	   font
 *	    @b[tag]	   begin rendition
 *	    @e[tag]	   end rendition
 *	    
 * Keep in mind that renditions override font specifications.
 *
 **/

#if NeedFunctionPrototypes
static XmIncludeStatus XmtConvertStringEscape(XtPointer *text,
					      XtPointer text_end,
					      XmTextType type,
					      XmStringTag string_tag,
					      XmParseMapping entry,
					      int pattern_length,
					      XmString *str_include,
					      XtPointer call_data)
#else
static XmIncludeStatus XmtConvertStringEscape(text, text_end, type, string_tag,
					      entry, pattern_length,
					      str_include, call_data)
XtPointer *text;
XtPointer text_end;
XmTextType type;
XmStringTag string_tag;
XmParseMapping entry;
int pattern_length;
XmString *str_include;
XtPointer call_data;
#endif
{
    char *s, *start, *text_ptr, indicator, *tag, tag_buf[3];
    XmStringComponentType component_type;

    text_ptr = (char *) *text;
    start = s = XtNewString(*text);
    
    /* s[0] should be '@'.  Bump pointer to following char */
    s++;
    
    if (*s == '@') { /* Sequence is '@@' */
	*str_include = XmStringComponentCreate(XmSTRING_COMPONENT_TEXT, 1, s);
	text_ptr +=2;
	*text = text_ptr;
	XtFree(start);
	return XmINSERT;
    }

   switch (*s) {
   case 't':
       component_type = XmSTRING_COMPONENT_TAB;
       break;
   case 'f':  
       /*
	* treat @f the same as an unterminated @b.
	* Using XmSTRING_COMPONENT_TAG doesn't work quite right
	* with Motif 2.0.0, so we do it this way, which seems
	* to make as much sense, anyway.
	*/
   case 'b':
       component_type = XmSTRING_COMPONENT_RENDITION_BEGIN;
       break;
   case 'e':
       component_type = XmSTRING_COMPONENT_RENDITION_END;
       break;
   default:
       component_type = XmSTRING_COMPONENT_UNKNOWN;
       break;
   }

   indicator = *s;

   switch (component_type) {

   case XmSTRING_COMPONENT_UNKNOWN:
       s++;
       
       *str_include = XmStringComponentCreate(XmSTRING_COMPONENT_TEXT,
					      2, start);
       text_ptr += 2;
       *text = text_ptr;
       XtFree(start);
       return XmINSERT;
       break;
       
   case XmSTRING_COMPONENT_TAB:
       *str_include = XmStringComponentCreate(component_type, 0, NULL);
       s++;
       text_ptr += s - start;
       *text = text_ptr;
       XtFree(start);
       return XmINSERT;
       break;
       
   default:	/* just drop through */
       break;
   }
    
    s++;	 /* go past the component_type character */
    
    if (*s == '\0') {
	XtFree(start);
	return XmTERMINATE;
    }
    
    /*
     * Now get the tag in '[ ]', after '(', or immediately
     * after the component_type indicator.
     */
    switch (*s) {
    case '[':
	s = tag = s+1;
	while (*s && (*s != ']')) s++;
	if (*s == '\0') {
	    break;
	}
	*s = '\0'; /* null terminate the tag string */
	s++;
	break;
	
    case '(':
	if (!*(s+1) || !*(s+2)) {
	    XtFree(start);
	    return XmTERMINATE;
	}
	tag_buf[0] = *(s+1);
	tag_buf[1] = *(s+2);
	tag_buf[2] = '\0';
	tag = tag_buf;
	s += 3;
	break;
	
    default:
	tag_buf[0] = *s;
	tag_buf[1] = '\0';
	s++;
	tag = tag_buf;
	break;
    }
    
    *str_include = XmStringComponentCreate(component_type, strlen(tag), tag);
    text_ptr += s - start;
    *text = text_ptr;
    
    XtFree(start);
    return XmINSERT;
}


#if NeedFunctionPrototypes
XmString XmtCreateXmString(StringConst str)
#else
XmString XmtCreateXmString(str)
StringConst str;
#endif
{
    static Boolean parse_table_registered = False;
    static XmParseMapping parse_table[3];
    
    if (!parse_table_registered) {
	Arg args[3];
	int n;
	
	n = 0;
	XtSetArg(args[n], XmNpattern, "\n"); n++;
	XtSetArg(args[n], XmNincludeStatus, XmINSERT); n++;
	XtSetArg(args[n], XmNsubstitute,XmStringSeparatorCreate());n++;
	parse_table[0] = XmParseMappingCreate(args, n);
	
	n = 0;
	XtSetArg(args[n], XmNpattern, "\t"); n++;
	XtSetArg(args[n], XmNincludeStatus, XmINSERT); n++;
	XtSetArg(args[n], XmNsubstitute,
		 XmStringComponentCreate(XmSTRING_COMPONENT_TAB, 0,NULL)); n++;
	parse_table[1] = XmParseMappingCreate(args, n);
	
	n = 0;
	XtSetArg(args[n], XmNpattern, "@"); n++;
	XtSetArg(args[n], XmNincludeStatus, XmINVOKE); n++;
	XtSetArg(args[n], XmNinvokeParseProc, XmtConvertStringEscape); n++;
	parse_table[2] = XmParseMappingCreate(args, n);
	
	parse_table_registered = True;
    }
    
    return XmStringParseText((XtPointer) str, NULL, NULL, XmCHARSET_TEXT,
			     parse_table, XtNumber(parse_table),
			     NULL);
}

#else  /* XmVersion < 2000 */

#if NeedFunctionPrototypes
static XmString appendstring(String txt, XmString to, int sep, char *charset,
			     int fixup)
#else
static XmString appendstring(txt, to, sep, charset, fixup)
String txt;
XmString to;
int sep;
char *charset;
int fixup;
#endif
{
    XmString s1, s2;
    char *s, *t;

    if (fixup) {
	for(s=t=txt; *s; s++, t++) {
	    *t = *s;
	    if ((*s == '@') && (*(s+1) == '@'))	s++;
	}
	*t = *s;  /* copy the terminating '\0' */
    }

    s1 = XmStringSegmentCreate(txt, charset, XmSTRING_DIRECTION_L_TO_R, sep);
    if (!to) return s1;
    s2 = XmStringConcat(to, s1);
    XmStringFree(to);
    XmStringFree(s1);
    return s2;
}

#if NeedFunctionPrototypes
XmString XmtCreateXmString(StringConst str)
#else
XmString XmtCreateXmString(str)
StringConst str;
#endif
{
    String r, s, t;
#ifdef XMSTRING_TO_COMPOUND_TEXT_BUG
    String charset = XmSTRING_DEFAULT_CHARSET;
#else	 
#  if XmVersion < 1002
    String charset = XmSTRING_DEFAULT_CHARSET;
#  else
    String charset = XmFONTLIST_DEFAULT_TAG;
#  endif
#endif	  
    char charset_buf[3];
    XmString result = NULL;
    Boolean fixup = False;
    
    if (!str) return NULL;
    else if (!*str)
	return XmStringSegmentCreate((String)str, charset,
				     XmSTRING_DIRECTION_L_TO_R, False);

    /*
     * make a copy of the string so we can frob with it
     * We keep pointer r so we can free it when done
     */
    r = s = XtNewString(str);

    for(t = s; *s; s++) {
	switch(*s) {
	case '\n':
	    *s = '\0';	/* null terminate the string */
	    result = appendstring(t, result, True, charset, fixup);
	    fixup = False;
	    t = s+1;
	    break;
	case '@':
	    if ((*s == '@') && (*(s+1) == '@')) {
		/* set a flag to strip doubled escapes later */
		fixup = True;
		s++;
		break;
	    }
	    else if (*(s+1) != 'f') break;
	    /* its a font change, so add the string up to here, if any */
	    *s = '\0'; /* null terminate the string */
	    if (s != t)
		result = appendstring(t, result, False, charset, fixup);
	    fixup = False;

	    /* and now get the new charset */
	    if (*(s+2) == '\0') break;
	    s+=2;
	    if (*s == '[') {  /* read till close ']' */
		s = charset = s+1;
		while (*s && (*s != ']')) s++;
		if (*s == '\0') break;
		*s = '\0';  /* null-terminate the charset string */
	    }
	    else if (*s == '(') { /* charset is next 2 chars */
		if (*(s+1)) s++; else break;
		charset_buf[0] = *s;
		if (*(s+1)) s++; else break;
		charset_buf[1] = *s;
		charset_buf[2] = '\0';
		charset = charset_buf;
	    }
	    else if (*s) { /* charset is single current character */
		charset_buf[0] = *s;
		charset_buf[1] = '\0';
		charset = charset_buf;
	    }

	    t = s+1;
	    break;
	}
    }

    if (s != t) result = appendstring(t, result, False, charset, fixup);
    
    XtFree(r);
    return result;
}

#endif /* XmVersion >= 2000 */

/* ARGSUSED */
#if NeedFunctionPrototypes
Boolean XmtConvertStringToXmString(Display *dpy,
				   XrmValue *args, Cardinal *num_args,
				   XrmValue *from, XrmValue *to,
				   XtPointer *converter_data)
#else
Boolean XmtConvertStringToXmString(dpy, args, num_args,
				   from, to, converter_data)
Display *dpy;
XrmValue *args;
Cardinal *num_args;
XrmValue *from;
XrmValue *to;
XtPointer *converter_data;
#endif
{
    String s = (String) from->addr;
    Screen *screen = *(Screen **)args[0].addr;
    String category, tag;
    String free_me = NULL;
    XmString value;
#if NeedFunctionPrototypes
    extern String _XmtLocalize(Screen *, StringConst,StringConst,StringConst);
#else
    extern String _XmtLocalize();
#endif
    
    /* if this string has a tag, localize it first */
    if ((s[0] == '@') && (s[1] == '[')) {
	s = XtNewString(s);
	free_me = s;
	s += 2;
	category = NULL;
	tag = s;
	while(*s) {
	    if (*s == '.' && !category) {
		*s = '\0';
		category = tag;
		tag = s+1;
	    }
	    if (*s == ']') {
		*s = '\0';
		s++;
		break;
	    }
	    s++;
	}
	if (!tag[0]) goto error;
	if (category && !category[0]) goto error;
	s = _XmtLocalize(screen, s, category, tag);
    }

    value = XmtCreateXmString(s);
    if (!value) goto error;
    if (free_me) XtFree(free_me);
    done(XmString, value);

 error:
    if (free_me) XtFree(free_me);
    XtDisplayStringConversionWarning(dpy, (String)from->addr, XmRXmString);
    return False;
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void FreeConvertedXmString(XtAppContext app, XrmValue *to,
				  XtPointer closure,
				  XrmValue *args, Cardinal *num_args)
#else
static void FreeConvertedXmString(app, to, closure, args, num_args)
XtAppContext app;
XrmValue *to;
XtPointer closure;
XrmValue *args;
Cardinal *num_args;
#endif
{
    XmStringFree(*((XmString *) to->addr));
}

#if NeedFunctionPrototypes
void XmtRegisterXmStringConverter(void)
#else
void XmtRegisterXmStringConverter()
#endif
{
    static Boolean registered = False;

    if (!registered) {
	XtSetTypeConverter(XtRString, XmRXmString,
			   XmtConvertStringToXmString,
			   (XtConvertArgRec *)screenConvertArg, (Cardinal) 1,
			   XtCacheAll | XtCacheRefCount,
			   FreeConvertedXmString);
	registered = True;
    }
}

