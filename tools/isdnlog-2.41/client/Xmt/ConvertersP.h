/* 
 * Motif Tools Library, Version 2.0
 * $Id: ConvertersP.h,v 2.11 1994/07/04 03:03:50 david Exp $
 * 
 * Written by David Flanagan.
 * Copyright (c) 1992, 1993, 1994 by Dovetail Systems.
 * All Rights Reserved.  See the file COPYRIGHT for details.
 * This is not free software.  See the file SHAREWARE for details.
 * There is no warranty for this software.  See NO_WARRANTY for details.
 */

#ifndef _XmtConvertersP_h
#define _XmtConvertersP_h

#include <Xmt/Converters.h>

/*
 * The done() macro is used by all the Xt converters as the standard way
 * to correctly return their values.  Note that this macro requires
 * that the converter have an argument named 'to'.
 */

#define	done(type, value)                         \
	{					  \
	    if (to->addr != NULL) {		  \
		if (to->size < sizeof(type)) {	  \
		    to->size = sizeof(type);	  \
		    return False;		  \
		}				  \
		*(type*)(to->addr) = (value);	  \
	    }					  \
	    else {				  \
		static type static_val;		  \
		static_val = (value);		  \
		to->addr = (XPointer)&static_val; \
	    }					  \
	    to->size = sizeof(type);		  \
	    return True;			  \
	}

/*
 * an argument list that passes only the widget requesting the conversion.
 * used in a couple of places.
 */
externalref XtConvertArgRec _XmtWidgetConvertArg[];

/*
 * These variables are defined so that Xmt functions can call Xmt
 * converters directly, if and only if they have been registered by
 * the programmer.  We use this level of indirection so that the
 * pixmap converter doesn't automatically link in the colortable
 * converter, for example.
 */
externalref XtTypeConverter _XmtCallbackConverter;
externalref XtTypeConverter _XmtColorTableConverter;

_XFUNCPROTOBEGIN
/*
 * an XtConvertArgProc used in several places
 */
#if NeedFunctionPrototypes
extern void _XmtFetchColorTable(Widget, Cardinal *, XrmValue *);
extern void _XmtFetchRootWidget(Widget, Cardinal *, XrmValue *);
extern void _XmtFetchVisual(Widget, Cardinal *, XrmValue *);
#else
extern void _XmtFetchColorTable();
extern void _XmtFetchRootWidget();
extern void _XmtFetchVisual();
#endif
_XFUNCPROTOEND    

#endif
