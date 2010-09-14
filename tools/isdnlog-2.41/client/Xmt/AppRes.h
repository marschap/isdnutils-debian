/* 
 * Motif Tools Library, Version 2.0
 * $Id: AppRes.h,v 1.6 1994/07/04 03:03:50 david Exp $
 * 
 * Written by David Flanagan.
 * Copyright (c) 1992, 1993, 1994 by Dovetail Systems.
 * All Rights Reserved.  See the file COPYRIGHT for details.
 * This is not free software.  See the file SHAREWARE for details.
 * There is no warranty for this software.  See NO_WARRANTY for details.
 */

#ifndef _XmtAppRes_h
#define _XmtAppRes_h

/*
 * names and classes for each of these resources
 */
#ifndef XmtNbusyCursor
#define XmtNbusyCursor		"busyCursor"
#endif
#ifndef XmtCBusyCursor
#define XmtCBusyCursor		"BusyCursor"
#endif
#ifndef XmtNbusyCursorForeground
#define XmtNbusyCursorForeground	"busyCursorForeground"
#endif
#ifndef XmtNbusyCursorBackground
#define XmtNbusyCursorBackground	"busyCursorBackground"
#endif
#ifndef XmtNhelpCursor
#define XmtNhelpCursor  	"helpCursor"
#endif
#ifndef XmtCHelpCursor
#define XmtCHelpCursor 		"HelpCursor"
#endif
#ifndef XmtNhelpCursorForeground
#define XmtNhelpCursorForeground  	"helpCursorForeground"
#endif
#ifndef XmtNhelpCursorBackground
#define XmtNhelpCursorBackground	"helpCursorBackground"
#endif
#ifndef XmtNcontextHelpPixmap
#define XmtNcontextHelpPixmap	"contextHelpPixmap"
#endif
#ifndef XmtCContextHelpPixmap
#define XmtCContextHelpPixmap	"ContextHelpPixmap"
#endif
#ifndef XmtNhelpFile
#define XmtNhelpFile		"helpFile"
#endif
#ifndef XmtCHelpFile
#define XmtCHelpFile		"HelpFile"
#endif
#ifndef XmtNcontextHelpFile
#define XmtNcontextHelpFile     "contextHelpFile"
#endif
#ifndef XmtCContextHelpFile
#define XmtCContextHelpFile     "ContextHelpFile"
#endif
#ifndef XmtNconfigDir
#define XmtNconfigDir		"configDir"
#endif
#ifndef XmtCConfigDir
#define XmtCConfigDir		"ConfigDir"
#endif
#ifndef XmtNhelpFilePath
#define XmtNhelpFilePath	"helpFilePath"
#endif
#ifndef XmtNresourceFilePath
#define XmtNresourceFilePath	"resourceFilePath"
#endif
#ifndef XmtNbitmapFilePath
#define XmtNbitmapFilePath	"bitmapFilePath"
#endif
#ifndef XmtNpixmapFilePath
#define XmtNpixmapFilePath	"pixmapFilePath"
#endif
#ifndef XmtCHelpFilePath
#define XmtCHelpFilePath	"HelpFilePath"
#endif
#ifndef XmtCResourceFilePath
#define XmtCResourceFilePath	"ResourceFilePath"
#endif
#ifndef XmtCBitmapFilePath
#define XmtCBitmapFilePath	"BitmapFilePath"
#endif
#ifndef XmtCPixmapFilePath
#define XmtCPixmapFilePath	"PixmapFilePath"
#endif
#ifndef XmtNconfigPath
#define XmtNconfigPath	        "configPath"
#endif
#ifndef XmtNuserConfigPath
#define XmtNuserConfigPath	"userConfigPath"
#endif
#ifndef XmtCConfigPath
#define XmtCConfigPath	"ConfigPath"
#endif
#ifndef XmtNcolorTable
#define XmtNcolorTable "colorTable"
#endif
#ifndef XmtCColorTable
#define XmtCColorTable "ColorTable"
#endif
#ifndef XmtNdefaultColorTable
#define XmtNdefaultColorTable "defaultColorTable"
#endif
#ifndef XmtCDefaultColorTable
#define XmtCDefaultColorTable "DefaultColorTable"
#endif
#ifndef XmtNreverseVideo
#define XmtNreverseVideo "reverseVideo"
#endif
#ifndef XmtCReverseVideo
#define XmtCReverseVideo "ReverseVideo"
#endif
#ifndef XmtNcursor
#define XmtNcursor "cursor"
#endif
#ifndef XmtCCursor
#define XmtCCursor "Cursor"
#endif
#ifndef XmtNcursorForeground
#define XmtNcursorForeground "cursorForeground"
#endif
#ifndef XmtCCursorForeground
#define XmtCCursorForeground "CursorForeground"
#endif
#ifndef XmtNcursorBackground
#define XmtNcursorBackground "cursorBackground"
#endif
#ifndef XmtCCursorBackground
#define XmtCCursorBackground "CursorBackground"
#endif

_XFUNCPROTOBEGIN
#if NeedFunctionPrototypes
extern void XmtParseCommandLine(Widget, int *, char **);
extern void XmtInitializeApplicationShell(Widget, ArgList, Cardinal);
extern void XmtSetApplicationValues(Widget, ArgList, Cardinal);
extern void XmtGetApplicationValues(Widget, ArgList, Cardinal);
extern Widget XmtLookupApplicationShell(XrmQuark);
#else
extern void XmtParseCommandLine();
extern void XmtInitializeApplicationShell();
extern void XmtSetApplicationValues();
extern void XmtGetApplicationValues();
extern Widget XmtLookupApplicationShell();
#endif
_XFUNCPROTOEND    

#endif
