/* 
 * Motif Tools Library, Version 2.0
 * $Id: Create.h,v 2.10 1994/07/04 03:03:50 david Exp $
 * 
 * Written by David Flanagan.
 * Copyright (c) 1992, 1993, 1994 by Dovetail Systems.
 * All Rights Reserved.  See the file COPYRIGHT for details.
 * This is not free software.  See the file SHAREWARE for details.
 * There is no warranty for this software.  See NO_WARRANTY for details.
 */

#ifndef _XmtCreate_h
#define _XmtCreate_h

/*
 * Names and classes for the pseudo-resources we read
 * during automatic widget creation.
 */
#define XmtNxmtChildren		"xmtChildren"
#define XmtCXmtChildren		"XmtChildren"
#define XmtNxmtType		"xmtType"
#define XmtCXmtType		"XmtType"
#define XmtNxmtCreationCallback	"xmtCreationCallback"
#define XmtCXmtCreationCallback "XmtCreationCallback"
#define XmtNxmtChildrenCreationCallback	"xmtChildrenCreationCallback"
#define XmtCXmtChildrenCreationCallback "XmtChildrenCreationCallback"
#define XmtNxmtManagedCreationCallback	"xmtManagedCreationCallback"
#define XmtCXmtManagedCreationCallback	"XmtManagedCreationCallback"


_XFUNCPROTOBEGIN
#if NeedFunctionPrototypes
extern void XmtCreateChildren(Widget);
extern Widget XmtCreateChild(Widget, StringConst);
extern Widget XmtBuildDialog(Widget, StringConst, XtResourceList, Cardinal);  
extern Widget XmtBuildToplevel(Widget, StringConst);
extern Widget XmtBuildApplication(StringConst, StringConst, Display *,
				       ArgList, Cardinal);
#else
extern void XmtCreateChildren();
extern Widget XmtCreateChild();
extern Widget XmtBuildDialog();  
extern Widget XmtBuildToplevel();
extern Widget XmtBuildApplication();
#endif

#if NeedVarargsPrototypes
extern void XmtCreateQueryChildren(Widget, ...);
extern Widget XmtCreateQueryChild(Widget, StringConst, ...);
extern Widget XmtBuildQueryDialog(Widget, StringConst, XtResourceList,
				  Cardinal, ...);
extern Widget XmtBuildQueryToplevel(Widget, StringConst, ...);
extern Widget XmtBuildQueryApplication(StringConst, StringConst, Display *,
				       ArgList, Cardinal, ...);
#else
extern void XmtCreateQueryChildren();
extern Widget XmtCreateQueryChild();
extern Widget XmtBuildQueryDialog();
extern Widget XmtBuildQueryToplevel();
extern Widget XmtBuildQueryApplication();
#endif
_XFUNCPROTOEND    

#endif
