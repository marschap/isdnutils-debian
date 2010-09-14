/* 
 * Motif Tools Library, Version 2.0
 * $Id: All.c,v 1.2 1994/07/04 03:03:50 david Exp $
 * 
 * Written by David Flanagan.
 * Copyright (c) 1992, 1993, 1994 by Dovetail Systems.
 * All Rights Reserved.  See the file COPYRIGHT for details.
 * This is not free software.  See the file SHAREWARE for details.
 * There is no warranty for this software.  See NO_WARRANTY for details.
 */

#include <Xmt/Xmt.h>
#include <Xmt/WidgetType.h>
#include <Xmt/Procedures.h>
#include <Xmt/Converters.h>
#include <Xmt/Layout.h>

#if NeedFunctionPrototypes
void XmtRegisterAll(void)
#else
void XmtRegisterAll()
#endif
{
    XmtRegisterMotifWidgets();
    XmtRegisterXmtWidgets();

    XmtRegisterUnixProcedures();
    XmtRegisterXtProcedures();
    XmtRegisterXmtProcedures();

    /* these are also registered by XmtInitialize() */
    XmtRegisterBitmapConverter();
    XmtRegisterBitmaskConverter();
    XmtRegisterPixmapConverter();
    XmtRegisterColorTableConverter();
    XmtRegisterWidgetConverter();
    XmtRegisterCallbackConverter();
    XmtRegisterXmStringConverter(); 
    XmtRegisterXmFontListConverter();
    XmtRegisterStringListConverter();
    XmtRegisterMenuItemsConverter();
    XmtRegisterPixmapListConverter();
    XmtRegisterPixelConverter();

    XmtRegisterLayoutParser();
    XmtRegisterLayoutCreateMethod();
}
