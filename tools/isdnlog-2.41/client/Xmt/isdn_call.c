/*
** X11/Motif interface for ISDN accounting for isdn4linux.
**
** Copyright 1996 by Oleg von Styp Rekowski (os@Kool.f.EUnet.de)
**
** This revision is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2, or (at your option)
** any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Revision: 0.01       Date: 10.02.96          Author: os
*/

#include <isdn_client.h>
#include <Xmt/Dialogs.h>
#include "isdn_help_strings.h"

void cb_call    (Widget w, XtPointer data, XmAnyCallbackStruct *reason)
{
}

void pb_call    (Widget w, XtPointer data, XmPushButtonCallbackStruct *reason)
{
 int    group = atoi (strrchr (XtName (XtParent (w)), '_') + 1), /* Menunr */
        call  = atoi (strrchr (XtName (w), '_') + 1);            /* Pushnr */

 switch	(group) {
   case 0: switch (call) {	/* Functions */
	case 0: isdn_closed_list_delete ();		break;	/* Delete */
	case 1:	XtManageChild (isdn_prot_wid_get ());	break;	/* Protokoll */
	case 2:	exit (0);				break;	/* End */
	}  break;

   case 1: /* Help */
	XmtDisplayInformation (toplevel, NULL, isdn_help_strings[2*call+1], isdn_help_strings[2*call]);
	break;
 }
}

void radio_call (Widget w, XtPointer data, XmToggleButtonCallbackStruct *reason)
{
}

void check_call (Widget w, XtPointer data, XmToggleButtonCallbackStruct *reason)
{
}

void isdn_error (int errnr)
{
 XmtDisplayError (toplevel, NULL, isdn_help_strings[errnr+8]);
}

char * isdn_error_get (int errnr)
{
 return (isdn_help_strings[errnr+8]);
}
