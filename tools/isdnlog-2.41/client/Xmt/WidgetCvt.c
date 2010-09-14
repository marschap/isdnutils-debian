/* 
 * Motif Tools Library, Version 2.0
 * $Id: WidgetCvt.c,v 2.10 1994/07/04 03:03:50 david Exp $
 * 
 * Written by David Flanagan.
 * Copyright (c) 1992, 1993, 1994 by Dovetail Systems.
 * All Rights Reserved.  See the file COPYRIGHT for details.
 * This is not free software.  See the file SHAREWARE for details.
 * There is no warranty for this software.  See NO_WARRANTY for details.
 */

#include <Xmt/Xmt.h>
#include <Xmt/ConvertersP.h>

/*ARGSUSED*/
#if NeedFunctionPrototypes
Boolean XmtConvertStringToWidget(Display *dpy,
				 XrmValue *args, Cardinal *num_args,
				 XrmValue *from, XrmValue *to,
				 XtPointer *converter_data)
#else
Boolean XmtConvertStringToWidget(dpy, args, num_args, from, to, converter_data)
Display *dpy;
XrmValue *args;
Cardinal *num_args;
XrmValue *from;
XrmValue *to;
XtPointer *converter_data;
#endif
{
    Widget w;

    if (*num_args != 1)
	XmtWarningMsg("XmtConvertStringToWidget","wrongParameters",
		      "converter needs reference widget argument.");

    w = XmtNameToWidget(*(Widget *)args[0].addr, from->addr);
    
    if (w == NULL) {
	XtDisplayStringConversionWarning(dpy, (char *)from->addr, XtRWidget);
	return False;
    }

    /* see ConvertersP.h for this macro */
    done(Widget, w);

}

#if NeedFunctionPrototypes
void XmtRegisterWidgetConverter(void)
#else
void XmtRegisterWidgetConverter()
#endif
{
    static Boolean registered = False;

    if (!registered) {
	/*
	 * We register this converter three times: once to convert XtRWidget,
	 * and also for to XmRWindow, and XmRMenuWidget, because Motif widgets
	 * mis-named the type of their widget resoruces and the Cascade button
	 * has yet another type.
	 */
	registered = True;
	XtSetTypeConverter(XtRString, XtRWidget,
			   XmtConvertStringToWidget,
			   _XmtWidgetConvertArg, (Cardinal) 1,
			   XtCacheNone, NULL);

	XtSetTypeConverter(XtRString, XmRWindow,
			   XmtConvertStringToWidget,
			   _XmtWidgetConvertArg, (Cardinal) 1,
			   XtCacheNone, NULL);

	XtSetTypeConverter(XtRString, XmRMenuWidget,
			   XmtConvertStringToWidget,
			   _XmtWidgetConvertArg, (Cardinal) 1,
			   XtCacheNone, NULL);
    }
}
